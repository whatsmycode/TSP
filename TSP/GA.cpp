#include <iostream>
#include <fstream>
#include <iomanip>	// 本文用于输出对齐
#include <stdlib.h> 
#include <ctime>
#include <algorithm>

#include "GA.h"

using namespace std;

int IndexCross_i;
int IndexCross_j;

int main() {
	time_t T_begin = clock();
	Graph G;
	CreateGraph(G);

	srand(unsigned(time(0)));
	InitialGroup(G);

	TSP_Evolution(G);	// 遗传算法

	time_t T_end = clock();
	double RunningTime = double(T_end - T_begin) / CLOCKS_PER_SEC;
	cout << endl << "【 程序运行时间 RunningTime = " << RunningTime << " 】" << endl;


	system("pause");
	return 0;
}

void CreateGraph(Graph &G) {
	ifstream read_in;
	read_in.open("L:\\Coding\\TSP_遗传算法\\TSP_遗传算法\\city_150.txt");
	if (!read_in.is_open())
	{
		cout << "文件读取失败." << endl;
		return;
	}

	read_in >> G.vex_num;
	// read_in >> G.arc_num;
	G.arc_num = 0;
	for (int i = 0; i < G.vex_num; i++)
	{
		read_in >> G.vexs[i];
	}
	G.vexs[G.vex_num] = '\0';	// char的结束符.

	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = 0; j < G.vex_num; j++)
		{
			read_in >> G.arcs[i][j];

			// calculate the arc_num
			if (G.arcs[i][j] > 0)
			{
				G.arc_num++;
			}
		}
	}

	// display
	cout << "无向图创建完毕，相关信息如下：" << endl;
	cout << "【顶点数】 G.vex_num = " << G.vex_num << endl;
	cout << "【边数】 G.arc_num = " << G.arc_num << endl;
	cout << "【顶点向量】 vexs[max_vexNum] = ";

	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}
}

void InitialGroup(Graph G) {

	cout << "----------------------【遗传算法参数】-----------------------" << endl;
	cout << "【城市个数】 CITY_NUM =" << CITY_NUM << endl;
	cout << "【群体规模】 GROUP_NUM = " << GROUP_NUM << endl;
	cout << "【子代规模】 SON_NUM = " << SON_NUM << endl;
	cout << "【变异概率】 P_INHERIATANCE = " << P_INHERIATANCE << endl;
	cout << "【杂交概率】 P_COPULATION = " << P_COPULATION << endl;
	cout << "【迭代次数】 ITERATION_NUM = " << ITERATION_NUM << endl;

	double total_length = 0.0;
	for (int i = 0; i < GROUP_NUM; i++) {
		for (int j = 0; j < G.vex_num; j++)
		{
			TSP_Groups[i].path[j] = G.vexs[j];
		}
		random_shuffle(TSP_Groups[i].path + 1, TSP_Groups[i].path + G.vex_num);
		if (Check_path(G, TSP_Groups[i]))
		{
			TSP_Groups[i].length_path = CalculateLength(G, TSP_Groups[i]);
			total_length += TSP_Groups[i].length_path;
		}
		else {
			cout << "【error！城市路径产生重复城市！】" << endl;
			TSP_Groups[i].length_path = MAX_INT;
			TSP_Groups[i].P_Reproduction = 0;
		}
	}

	Calc_Probablity(G, total_length);
	TSP_Evaluate(G);
}

// 处理对象：每次新产生的群体, 计算每个个体的概率
// 问题：解决TSP问题, 路径越短概率应该越高
// 方案：对于当前总群, 将所有个体路径取倒数, 然后乘以该总群的总路径得到大于1的值, 然后进行归一化, 取得概率
// 归一化：累加当前所有大于1的个体的伪概率, 得到TempTotal_P, 每个概率再分别除以 TempTotal_P 进行归一化
void Calc_Probablity(Graph G, double total_length) {
	double TempTotal_P = 0.0;

	for (int i = 0; i < GROUP_NUM; i++)
	{
		TSP_Groups[i].P_Reproduction = (1.0 / TSP_Groups[i].length_path) * total_length;
		TempTotal_P += TSP_Groups[i].P_Reproduction;
	}

	for (int i = 0; i < GROUP_NUM; i++)
	{
		TSP_Groups[i].P_Reproduction = TSP_Groups[i].P_Reproduction / TempTotal_P;
	}
}

void TSP_Evolution(Graph G) {
	/* */
	int iter = 0;
	while (iter < ITERATION_NUM) {
		// cout<<"***********************【第次"<<(iter + 1)<<"迭代】*************************"<<endl;

		// 1. 选择
		int Father_index = Evo_Select(G);
		int Mother_index = Evo_Select(G);

		while (Mother_index == Father_index)
		{
			// 防止Father和Mother都是同一个个体 -> 自交( 父母为同一个个体时, 母亲重新选择, 直到父母为不同的个体为止 )
			// cout<<"Warning!【Father_index = Mother_index】"<<endl;
			Mother_index = Evo_Select(G);
		}

		// TSP_Groups[]为当前总群
		TSP_solution Father = TSP_Groups[Father_index];
		TSP_solution Mother = TSP_Groups[Mother_index];

		// 2. 交叉, 存储在全局变脸 Son_solution[] 数组 - 通过M次杂交, 产生2M个新个体, 2M >= GROUP_NUM
		int M = GROUP_NUM - GROUP_NUM / 2;
		Length_SonSoliton = 0;	// 遗传产生的个体个数, 置零重新累加
		while (M) {
			double Is_COPULATION = ((rand() % 100 + 0.0) / 100);
			if (Is_COPULATION > P_COPULATION)
			{
				// cout<<"[ 这两个染色体不进行杂交 ]Is_COPULATION = "<<Is_COPULATION<<endl;
			}
			else {
				// 杂交, 将结果存储于遗传个体总群,全局变量Son_solution[]
				Evo_Cross(G, Father, Mother);
				M--;
			}
		}

		// 3. 变异：针对 Son_solution[]
		double total_length = 0.0;	// 更新新个体的概率

		for (int IndexVariation = 0; IndexVariation < Length_SonSoliton; IndexVariation++)
		{
			double RateVariation = float(rand() % 100) / 100;
			// 产生的随机数小于变异概率 则该个体进行变异
			if (RateVariation < P_INHERIATANCE)
			{
				Evo_Variation(G, IndexVariation);
			}

			// 经过变异处理后 重新计算路径值
			if (!Check_path(G, Son_solution[IndexVariation]))
			{
				cout << "【Error! 路径有重复!】" << endl;
			}

			// 产生新个体, 计算新路径和新概率
			Son_solution[IndexVariation].length_path = CalculateLength(G, Son_solution[IndexVariation]);
			total_length += Son_solution[IndexVariation].length_path;
		}

		Calc_Probablity(G, total_length);

		/*
		cout<<"【遗传产生的子代个体如下...】"<<endl;
		for (int i = 0; i < Length_SonSoliton; i++)
		{
		for (int j = 0;j < G.vex_num;j++)
		{
		cout<<Son_solution[i].path[j]<<" -> ";
		}
		cout<<Son_solution[i].path[0]<<"    length_path = "<<Son_solution[i].length_path<<"    P_Reproduction = "<<Son_solution[i].P_Reproduction<<endl;
		}
		*/

		// 4. 更新群体
		// 参与对象：父代 + 遗传的子代
		Evo_UpdateGroup(G);

		iter++;
	}
}

// 选择
/*
输入：当前总群
输出：按照一个评价, 随机从当前总群筛选出杂交对象, 本程序每次返回一个个体
选择方案：比例选择规则, [轮盘赌选择]
机制：反映在对父代种群中每一个体所赋予的允许繁殖概率及其从2M个中间个体中如何选择子代种群的机制上！
*/
/*
[轮盘赌选择] -  轮盘赌选择是从染色体群体中选择一些成员的方法，被选中的机率和它们的适应性分数成比例，染色体的适应性分数愈高，被选中的概率也愈多.
1. 随机产生一个概率 selection_P
2. [概率分布函数]声明变量 distribution_P = 0, 对于每个个体, 依次累加个体的概率到distribution_P上, 判断当前随机概率selection_P是否小于distribution_P, 若是则中该染色体, 结束循环
*/
int Evo_Select(Graph G) {
	double selection_P = ((rand() % 100 + 0.0) / 100);
	// cout<<"selection_P = "<<selection_P<<endl;

	double distribution_P = 0.0;
	for (int i = 0; i < GROUP_NUM; i++)
	{
		distribution_P += TSP_Groups[i].P_Reproduction;
		if (selection_P < distribution_P)
		{
			return i;
		}
	}
	cout << "【ERROR!】Evo_Select() 轮盘赌选择有误..." << endl;
	return 0;
}

// 交叉
/*
输入：[TSP_Father , TSP_Mother]两个个体作为父母, 进行杂交
输出：通过杂交产生新个体(遗传算法产生2个新个体, 演化算法产生1个新个体)
杂交方案：[父子混合选择][自然选择 - 父母不参与竞争]
-- [演化策略]所使用的杂交算子是从两个个体生成一个个体的操作
-- [遗传算法]生成两个新个体。常见的“中间杂交”（intermediate crossover）及“随机杂交”（random crossover）等!
*/
/*
TSP_杂交具体方法：
1. 随机选取两个交叉点i和j,记为 Father_Cross 和 Mother_Cross
2. 将两交叉点中间的基因段互换
3. 分别对Father和Mother的路径进行冲突处理：
-- 以Father为例, 保持Father_Cross基因段不变, 基因段以外的部分与Father_Cross基因段冲突的城市, 用Father_Cross和Mother_Cross对应的位置去互换, 直到没有冲突.
-- 冲突城市的确定: Father_Cross 和 Mother_Cross去补集,存放于数组 Conflict[] 中.
*/
void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother) {
	// 杂交过程：随机产生杂交的位置, 保证 IndexCross_i < IndexCross_j【全局变量】
	IndexCross_i = rand() % (CITY_NUM - 1) + 1;	// 不能取到起始城市
	IndexCross_j = rand() % (CITY_NUM - 1) + 1;	//
	if (IndexCross_i > IndexCross_j)
	{
		int temp = IndexCross_i;
		IndexCross_i = IndexCross_j;
		IndexCross_j = temp;
	}
	if (IndexCross_j == CITY_NUM || IndexCross_i == 0)
	{
		cout << "[ 杂交过程的随机数产生有问题... ]" << endl;
	}

	// 杂交基因段
	int Father_Cross[CITY_NUM];	// 父亲遗传基因段
	int Mother_Cross[CITY_NUM];	// 母亲遗传基因段
	int Length_Cross = 0;		// 杂交的个数
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		Father_Cross[Length_Cross] = TSP_Father.path[i];
		Mother_Cross[Length_Cross] = TSP_Mother.path[i];
		Length_Cross++;
	}

	// 开始杂交 - 处理 TSP_Father：找到Father_Cross[]中会产生冲突的城市
	int *Conflict_Father;		// 存储冲突的位置
	int *Conflict_Mother;
	int Length_Conflict = 0;	// 冲突的个数
	Conflict_Father = Get_Conflict(Father_Cross, Mother_Cross, Length_Cross, Length_Conflict);
	Conflict_Mother = Get_Conflict(Mother_Cross, Father_Cross, Length_Cross, Length_Conflict);

	// Father and Mother 交换基因段
	int city_temp;
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		city_temp = TSP_Father.path[i];
		TSP_Father.path[i] = TSP_Mother.path[i];
		TSP_Mother.path[i] = city_temp;
	}

	// 开始杂交 - 处理 TSP_Mother, 其中Length_Conflict会在函数Get_Conflict()中改变并保存
	TSP_solution Descendant_ONE = Handle_Conflict(G, TSP_Father, Conflict_Father, Conflict_Mother, Length_Conflict);	// 解决 TSP_Father 的冲突
	TSP_solution Descendant_TWO = Handle_Conflict(G, TSP_Mother, Conflict_Mother, Conflict_Father, Length_Conflict);	// 解决 TSP_Mother 的冲突

	Son_solution[Length_SonSoliton++] = Descendant_ONE;
	Son_solution[Length_SonSoliton++] = Descendant_TWO;
}

TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int *Detection_Conflict, int *Model_Conflict, int Length_Conflict) {
	for (int i = 0; i <= Length_Conflict; i++)
	{
		bool flag_FindCity = false;
		int index = 0;
		// [0, IndexCross_i) 寻找冲突
		for (index = 0; index < IndexCross_i; index++)
		{
			if (Model_Conflict[i] == ConflictSolution.path[index])
			{
				flag_FindCity = true;
				break;
			}
		}

		// 第一段没找到, 找剩余的部分【除了交换的基因段外】
		if (!flag_FindCity)
		{
			// [IndexCross_i + 1, G.vex_num) 寻找冲突
			for (index = IndexCross_j + 1; index < G.vex_num; index++)
			{
				if (Model_Conflict[i] == ConflictSolution.path[index])
				{
					break;
				}
			}
		}

		// 9 8 [1 4 0 3 2] 3 2 0 --> ConflictSolution
		// 8 7 [4 5 6 7 1] 9 6 5
		// [0 3 2] --> Detection_Conflict
		// [4 5 6] --> Model_Conflict
		// 解决冲突, index 为当前i冲突的位置, 用Model_Conflict去替换.
		// cout<<"index = "<<index<<endl;
		ConflictSolution.path[index] = Detection_Conflict[i];
	}

	/*
	cout<<endl<<"【解决冲突】基因组为：";
	for (int i = 0;i < G.vex_num;i++)
	{
	cout<<ConflictSolution.path[i]<<" -> ";
	}
	cout<<ConflictSolution.path[0]<<endl;

	// 重新计算新路径的长度
	// CalculateLength(G, ConflictSolution);
	*/
	if (!Check_path(G, ConflictSolution))
	{
		cout << "【error - 冲突未解决......】" << endl;
	}
	// cout<<"  length_path = "<<ConflictSolution.length_path<<"    P_Reproduction = "<<ConflictSolution.P_Reproduction<<endl;

	return ConflictSolution;
}

int *Get_Conflict(int Detection_Cross[], int Model_Cross[], int Length_Cross, int &Length_Conflict) {
	// 同时存在于 Father_Cross 和 Mother_Cross 为不冲突的城市, 反之是冲突的城市.
	// Detection_Cross[]:表示当前搜索的个体, 即找冲突的对象
	// Model_Cross[]:	
	// int Conflict[CITY_NUM];
	int *Conflict = new int[CITY_NUM];
	Length_Conflict = 0;
	for (int i = 0; i < Length_Cross; i++)
	{
		bool flag_Conflict = true;	// 判断是否属于冲突
		for (int j = 0; j < Length_Cross; j++)
		{
			if (Detection_Cross[i] == Model_Cross[j])
			{
				// 结束第二层循环
				j = Length_Cross;
				flag_Conflict = false;	// 该城市不属于冲突
			}
		}
		if (flag_Conflict)
		{
			Conflict[Length_Conflict] = Detection_Cross[i];
			Length_Conflict++;
		}
	}
	return Conflict;
}

// 变异
/*
输入：杂交得到的所有个体（大于总群规模）
输出：通过变异策略, 以一定的变异概率（确定变异个数）随机选择个体进行变异
变异策略：随机交换染色体的片段, TSP - 随机交换两个城市的位置
*/
void Evo_Variation(Graph G, int Index_Variation) {
	// 随机产生两个随机数表示两个城市的位置, 并进行位置交换
	int City_i = (rand() % (CITY_NUM - 1)) + 1;	// [1, CITY_NUM - 1]起始城市不变异
	int City_j = (rand() % (CITY_NUM - 1)) + 1;	// 

	while (City_i == City_j) {
		City_j = (rand() % (CITY_NUM - 1)) + 1;
	}

	// 交换城市位置 - 变异
	int temp_City = Son_solution[Index_Variation].path[City_i];
	Son_solution[Index_Variation].path[City_i] = Son_solution[Index_Variation].path[City_j];
	Son_solution[Index_Variation].path[City_j] = temp_City;
}

// 父代 - TSP_Groups[]
// 子代 - Son_solution[]
void Evo_UpdateGroup(Graph G) {
	TSP_solution tempSolution;
	// 先对子代 - Son_solution[] 依据路径长度进行排序 - 降序[按路径从大到小]
	for (int i = 0; i < Length_SonSoliton; i++)
	{
		for (int j = Length_SonSoliton - 1; j > i; j--)
		{
			if (Son_solution[i].length_path > Son_solution[j].length_path)
			{
				tempSolution = Son_solution[i];
				Son_solution[i] = Son_solution[j];
				Son_solution[j] = tempSolution;
			}
		}
	}

	/*
	cout<<"【冒泡排序后...】"<<endl;
	for (int i = 0; i < Length_SonSoliton; i++)
	{
	cout<<"length_path = "<<Son_solution[i].length_path<<endl;
	}
	*/

	// 更新
	for (int i = 0; i < Length_SonSoliton; i++)	// 子代 - 按路径从大到小排序
	{
		for (int j = 0; j < GROUP_NUM; j++)	// 父代
		{
			if (Son_solution[i].length_path < TSP_Groups[j].length_path)
			{
				TSP_Groups[j] = Son_solution[i];	// 种群更新
				break;
			}
		}
	}

	TSP_Evaluate(G);
}

double CalculateLength(Graph G, TSP_solution newSolution) {
	double _length = 0;

	for (int i = 0; i < G.vex_num - 1; i++)
	{
		int _startCity = newSolution.path[i] - 1;	// 路径下标是从 1 开始存储
		int _endCity = newSolution.path[i + 1] - 1;
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

	// 判断该路径是否能回到起始城市
	if (G.arcs[newSolution.path[G.vex_num - 1]][newSolution.path[0] - 1] == -1)
	{
		return MAX_INT;
	}
	else {
		_length += G.arcs[newSolution.path[G.vex_num - 1] - 1][newSolution.path[0] - 1];
		// cout<<"_length = "<<_length<<endl;
		return _length;
	}
}

bool Check_path(Graph G, TSP_solution CurrentSolution) {
	for (int i = 0; i < G.vex_num; i++)
	{
		for (int j = i + 1; j < G.vex_num; j++)
		{
			if (CurrentSolution.path[i] == CurrentSolution.path[j])
			{
				return false;
			}
		}
	}
	return true;
}

/*
// TSP - 评价函数
// 输入：当前总群 TSP_Groups[] - 包括 每个个体的路径和所需的长度
// 输出：当前总群中, 最优的个体：bestSolution
// 评价方法：路径最短的为最优
*/
void TSP_Evaluate(Graph G) {
	TSP_solution bsetSolution;
	bsetSolution = TSP_Groups[0];
	for (int i = 1; i < GROUP_NUM; i++)
	{
		if (bsetSolution.length_path > TSP_Groups[i].length_path)
		{
			bsetSolution = TSP_Groups[i];
		}
	}

	cout << "当前最优个体 bsetSolution = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bsetSolution.path[i] << " -> ";
	}
	cout << bsetSolution.path[0] << "    length = " << bsetSolution.length_path << endl;

}
