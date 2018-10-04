#include <iostream>
#include <fstream>
#include <iomanip>	// ���������������
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

	TSP_Evolution(G);	// �Ŵ��㷨

	time_t T_end = clock();
	double RunningTime = double(T_end - T_begin) / CLOCKS_PER_SEC;
	cout << endl << "�� ��������ʱ�� RunningTime = " << RunningTime << " ��" << endl;


	system("pause");
	return 0;
}

void CreateGraph(Graph &G) {
	ifstream read_in;
	read_in.open("L:\\Coding\\TSP_�Ŵ��㷨\\TSP_�Ŵ��㷨\\city_150.txt");
	if (!read_in.is_open())
	{
		cout << "�ļ���ȡʧ��." << endl;
		return;
	}

	read_in >> G.vex_num;
	// read_in >> G.arc_num;
	G.arc_num = 0;
	for (int i = 0; i < G.vex_num; i++)
	{
		read_in >> G.vexs[i];
	}
	G.vexs[G.vex_num] = '\0';	// char�Ľ�����.

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
	cout << "����ͼ������ϣ������Ϣ���£�" << endl;
	cout << "���������� G.vex_num = " << G.vex_num << endl;
	cout << "�������� G.arc_num = " << G.arc_num << endl;
	cout << "������������ vexs[max_vexNum] = ";

	for (int i = 0; i < G.vex_num; i++)
	{
		cout << G.vexs[i] << " ";
	}
}

void InitialGroup(Graph G) {

	cout << "----------------------���Ŵ��㷨������-----------------------" << endl;
	cout << "�����и����� CITY_NUM =" << CITY_NUM << endl;
	cout << "��Ⱥ���ģ�� GROUP_NUM = " << GROUP_NUM << endl;
	cout << "���Ӵ���ģ�� SON_NUM = " << SON_NUM << endl;
	cout << "��������ʡ� P_INHERIATANCE = " << P_INHERIATANCE << endl;
	cout << "���ӽ����ʡ� P_COPULATION = " << P_COPULATION << endl;
	cout << "������������ ITERATION_NUM = " << ITERATION_NUM << endl;

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
			cout << "��error������·�������ظ����У���" << endl;
			TSP_Groups[i].length_path = MAX_INT;
			TSP_Groups[i].P_Reproduction = 0;
		}
	}

	Calc_Probablity(G, total_length);
	TSP_Evaluate(G);
}

// �������ÿ���²�����Ⱥ��, ����ÿ������ĸ���
// ���⣺���TSP����, ·��Խ�̸���Ӧ��Խ��
// ���������ڵ�ǰ��Ⱥ, �����и���·��ȡ����, Ȼ����Ը���Ⱥ����·���õ�����1��ֵ, Ȼ����й�һ��, ȡ�ø���
// ��һ�����ۼӵ�ǰ���д���1�ĸ����α����, �õ�TempTotal_P, ÿ�������ٷֱ���� TempTotal_P ���й�һ��
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
		// cout<<"***********************���ڴ�"<<(iter + 1)<<"������*************************"<<endl;

		// 1. ѡ��
		int Father_index = Evo_Select(G);
		int Mother_index = Evo_Select(G);

		while (Mother_index == Father_index)
		{
			// ��ֹFather��Mother����ͬһ������ -> �Խ�( ��ĸΪͬһ������ʱ, ĸ������ѡ��, ֱ����ĸΪ��ͬ�ĸ���Ϊֹ )
			// cout<<"Warning!��Father_index = Mother_index��"<<endl;
			Mother_index = Evo_Select(G);
		}

		// TSP_Groups[]Ϊ��ǰ��Ⱥ
		TSP_solution Father = TSP_Groups[Father_index];
		TSP_solution Mother = TSP_Groups[Mother_index];

		// 2. ����, �洢��ȫ�ֱ��� Son_solution[] ���� - ͨ��M���ӽ�, ����2M���¸���, 2M >= GROUP_NUM
		int M = GROUP_NUM - GROUP_NUM / 2;
		Length_SonSoliton = 0;	// �Ŵ������ĸ������, ���������ۼ�
		while (M) {
			double Is_COPULATION = ((rand() % 100 + 0.0) / 100);
			if (Is_COPULATION > P_COPULATION)
			{
				// cout<<"[ ������Ⱦɫ�岻�����ӽ� ]Is_COPULATION = "<<Is_COPULATION<<endl;
			}
			else {
				// �ӽ�, ������洢���Ŵ�������Ⱥ,ȫ�ֱ���Son_solution[]
				Evo_Cross(G, Father, Mother);
				M--;
			}
		}

		// 3. ���죺��� Son_solution[]
		double total_length = 0.0;	// �����¸���ĸ���

		for (int IndexVariation = 0; IndexVariation < Length_SonSoliton; IndexVariation++)
		{
			double RateVariation = float(rand() % 100) / 100;
			// �����������С�ڱ������ ��ø�����б���
			if (RateVariation < P_INHERIATANCE)
			{
				Evo_Variation(G, IndexVariation);
			}

			// �������촦��� ���¼���·��ֵ
			if (!Check_path(G, Son_solution[IndexVariation]))
			{
				cout << "��Error! ·�����ظ�!��" << endl;
			}

			// �����¸���, ������·�����¸���
			Son_solution[IndexVariation].length_path = CalculateLength(G, Son_solution[IndexVariation]);
			total_length += Son_solution[IndexVariation].length_path;
		}

		Calc_Probablity(G, total_length);

		/*
		cout<<"���Ŵ��������Ӵ���������...��"<<endl;
		for (int i = 0; i < Length_SonSoliton; i++)
		{
		for (int j = 0;j < G.vex_num;j++)
		{
		cout<<Son_solution[i].path[j]<<" -> ";
		}
		cout<<Son_solution[i].path[0]<<"    length_path = "<<Son_solution[i].length_path<<"    P_Reproduction = "<<Son_solution[i].P_Reproduction<<endl;
		}
		*/

		// 4. ����Ⱥ��
		// ������󣺸��� + �Ŵ����Ӵ�
		Evo_UpdateGroup(G);

		iter++;
	}
}

// ѡ��
/*
���룺��ǰ��Ⱥ
���������һ������, ����ӵ�ǰ��Ⱥɸѡ���ӽ�����, ������ÿ�η���һ������
ѡ�񷽰�������ѡ�����, [���̶�ѡ��]
���ƣ���ӳ�ڶԸ�����Ⱥ��ÿһ���������������ֳ���ʼ����2M���м���������ѡ���Ӵ���Ⱥ�Ļ����ϣ�
*/
/*
[���̶�ѡ��] -  ���̶�ѡ���Ǵ�Ⱦɫ��Ⱥ����ѡ��һЩ��Ա�ķ�������ѡ�еĻ��ʺ����ǵ���Ӧ�Է����ɱ�����Ⱦɫ�����Ӧ�Է������ߣ���ѡ�еĸ���Ҳ����.
1. �������һ������ selection_P
2. [���ʷֲ�����]�������� distribution_P = 0, ����ÿ������, �����ۼӸ���ĸ��ʵ�distribution_P��, �жϵ�ǰ�������selection_P�Ƿ�С��distribution_P, �������и�Ⱦɫ��, ����ѭ��
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
	cout << "��ERROR!��Evo_Select() ���̶�ѡ������..." << endl;
	return 0;
}

// ����
/*
���룺[TSP_Father , TSP_Mother]����������Ϊ��ĸ, �����ӽ�
�����ͨ���ӽ������¸���(�Ŵ��㷨����2���¸���, �ݻ��㷨����1���¸���)
�ӽ�������[���ӻ��ѡ��][��Ȼѡ�� - ��ĸ�����뾺��]
-- [�ݻ�����]��ʹ�õ��ӽ������Ǵ�������������һ������Ĳ���
-- [�Ŵ��㷨]���������¸��塣�����ġ��м��ӽ�����intermediate crossover����������ӽ�����random crossover����!
*/
/*
TSP_�ӽ����巽����
1. ���ѡȡ���������i��j,��Ϊ Father_Cross �� Mother_Cross
2. ����������м�Ļ���λ���
3. �ֱ��Father��Mother��·�����г�ͻ����
-- ��FatherΪ��, ����Father_Cross����β���, ���������Ĳ�����Father_Cross����γ�ͻ�ĳ���, ��Father_Cross��Mother_Cross��Ӧ��λ��ȥ����, ֱ��û�г�ͻ.
-- ��ͻ���е�ȷ��: Father_Cross �� Mother_Crossȥ����,��������� Conflict[] ��.
*/
void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother) {
	// �ӽ����̣���������ӽ���λ��, ��֤ IndexCross_i < IndexCross_j��ȫ�ֱ�����
	IndexCross_i = rand() % (CITY_NUM - 1) + 1;	// ����ȡ����ʼ����
	IndexCross_j = rand() % (CITY_NUM - 1) + 1;	//
	if (IndexCross_i > IndexCross_j)
	{
		int temp = IndexCross_i;
		IndexCross_i = IndexCross_j;
		IndexCross_j = temp;
	}
	if (IndexCross_j == CITY_NUM || IndexCross_i == 0)
	{
		cout << "[ �ӽ����̵����������������... ]" << endl;
	}

	// �ӽ������
	int Father_Cross[CITY_NUM];	// �����Ŵ������
	int Mother_Cross[CITY_NUM];	// ĸ���Ŵ������
	int Length_Cross = 0;		// �ӽ��ĸ���
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		Father_Cross[Length_Cross] = TSP_Father.path[i];
		Mother_Cross[Length_Cross] = TSP_Mother.path[i];
		Length_Cross++;
	}

	// ��ʼ�ӽ� - ���� TSP_Father���ҵ�Father_Cross[]�л������ͻ�ĳ���
	int *Conflict_Father;		// �洢��ͻ��λ��
	int *Conflict_Mother;
	int Length_Conflict = 0;	// ��ͻ�ĸ���
	Conflict_Father = Get_Conflict(Father_Cross, Mother_Cross, Length_Cross, Length_Conflict);
	Conflict_Mother = Get_Conflict(Mother_Cross, Father_Cross, Length_Cross, Length_Conflict);

	// Father and Mother ���������
	int city_temp;
	for (int i = IndexCross_i; i <= IndexCross_j; i++)
	{
		city_temp = TSP_Father.path[i];
		TSP_Father.path[i] = TSP_Mother.path[i];
		TSP_Mother.path[i] = city_temp;
	}

	// ��ʼ�ӽ� - ���� TSP_Mother, ����Length_Conflict���ں���Get_Conflict()�иı䲢����
	TSP_solution Descendant_ONE = Handle_Conflict(G, TSP_Father, Conflict_Father, Conflict_Mother, Length_Conflict);	// ��� TSP_Father �ĳ�ͻ
	TSP_solution Descendant_TWO = Handle_Conflict(G, TSP_Mother, Conflict_Mother, Conflict_Father, Length_Conflict);	// ��� TSP_Mother �ĳ�ͻ

	Son_solution[Length_SonSoliton++] = Descendant_ONE;
	Son_solution[Length_SonSoliton++] = Descendant_TWO;
}

TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int *Detection_Conflict, int *Model_Conflict, int Length_Conflict) {
	for (int i = 0; i <= Length_Conflict; i++)
	{
		bool flag_FindCity = false;
		int index = 0;
		// [0, IndexCross_i) Ѱ�ҳ�ͻ
		for (index = 0; index < IndexCross_i; index++)
		{
			if (Model_Conflict[i] == ConflictSolution.path[index])
			{
				flag_FindCity = true;
				break;
			}
		}

		// ��һ��û�ҵ�, ��ʣ��Ĳ��֡����˽����Ļ�����⡿
		if (!flag_FindCity)
		{
			// [IndexCross_i + 1, G.vex_num) Ѱ�ҳ�ͻ
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
		// �����ͻ, index Ϊ��ǰi��ͻ��λ��, ��Model_Conflictȥ�滻.
		// cout<<"index = "<<index<<endl;
		ConflictSolution.path[index] = Detection_Conflict[i];
	}

	/*
	cout<<endl<<"�������ͻ��������Ϊ��";
	for (int i = 0;i < G.vex_num;i++)
	{
	cout<<ConflictSolution.path[i]<<" -> ";
	}
	cout<<ConflictSolution.path[0]<<endl;

	// ���¼�����·���ĳ���
	// CalculateLength(G, ConflictSolution);
	*/
	if (!Check_path(G, ConflictSolution))
	{
		cout << "��error - ��ͻδ���......��" << endl;
	}
	// cout<<"  length_path = "<<ConflictSolution.length_path<<"    P_Reproduction = "<<ConflictSolution.P_Reproduction<<endl;

	return ConflictSolution;
}

int *Get_Conflict(int Detection_Cross[], int Model_Cross[], int Length_Cross, int &Length_Conflict) {
	// ͬʱ������ Father_Cross �� Mother_Cross Ϊ����ͻ�ĳ���, ��֮�ǳ�ͻ�ĳ���.
	// Detection_Cross[]:��ʾ��ǰ�����ĸ���, ���ҳ�ͻ�Ķ���
	// Model_Cross[]:	
	// int Conflict[CITY_NUM];
	int *Conflict = new int[CITY_NUM];
	Length_Conflict = 0;
	for (int i = 0; i < Length_Cross; i++)
	{
		bool flag_Conflict = true;	// �ж��Ƿ����ڳ�ͻ
		for (int j = 0; j < Length_Cross; j++)
		{
			if (Detection_Cross[i] == Model_Cross[j])
			{
				// �����ڶ���ѭ��
				j = Length_Cross;
				flag_Conflict = false;	// �ó��в����ڳ�ͻ
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

// ����
/*
���룺�ӽ��õ������и��壨������Ⱥ��ģ��
�����ͨ���������, ��һ���ı�����ʣ�ȷ��������������ѡ�������б���
������ԣ��������Ⱦɫ���Ƭ��, TSP - ��������������е�λ��
*/
void Evo_Variation(Graph G, int Index_Variation) {
	// ������������������ʾ�������е�λ��, ������λ�ý���
	int City_i = (rand() % (CITY_NUM - 1)) + 1;	// [1, CITY_NUM - 1]��ʼ���в�����
	int City_j = (rand() % (CITY_NUM - 1)) + 1;	// 

	while (City_i == City_j) {
		City_j = (rand() % (CITY_NUM - 1)) + 1;
	}

	// ��������λ�� - ����
	int temp_City = Son_solution[Index_Variation].path[City_i];
	Son_solution[Index_Variation].path[City_i] = Son_solution[Index_Variation].path[City_j];
	Son_solution[Index_Variation].path[City_j] = temp_City;
}

// ���� - TSP_Groups[]
// �Ӵ� - Son_solution[]
void Evo_UpdateGroup(Graph G) {
	TSP_solution tempSolution;
	// �ȶ��Ӵ� - Son_solution[] ����·�����Ƚ������� - ����[��·���Ӵ�С]
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
	cout<<"��ð�������...��"<<endl;
	for (int i = 0; i < Length_SonSoliton; i++)
	{
	cout<<"length_path = "<<Son_solution[i].length_path<<endl;
	}
	*/

	// ����
	for (int i = 0; i < Length_SonSoliton; i++)	// �Ӵ� - ��·���Ӵ�С����
	{
		for (int j = 0; j < GROUP_NUM; j++)	// ����
		{
			if (Son_solution[i].length_path < TSP_Groups[j].length_path)
			{
				TSP_Groups[j] = Son_solution[i];	// ��Ⱥ����
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
		int _startCity = newSolution.path[i] - 1;	// ·���±��Ǵ� 1 ��ʼ�洢
		int _endCity = newSolution.path[i + 1] - 1;
		if (G.arcs[_startCity][_endCity] == -1)
		{
			return MAX_INT;
		}
		else {
			_length += G.arcs[_startCity][_endCity];
		}
	}

	// �жϸ�·���Ƿ��ܻص���ʼ����
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
// TSP - ���ۺ���
// ���룺��ǰ��Ⱥ TSP_Groups[] - ���� ÿ�������·��������ĳ���
// �������ǰ��Ⱥ��, ���ŵĸ��壺bestSolution
// ���۷�����·����̵�Ϊ����
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

	cout << "��ǰ���Ÿ��� bsetSolution = ";
	for (int i = 0; i < G.vex_num; i++)
	{
		cout << bsetSolution.path[i] << " -> ";
	}
	cout << bsetSolution.path[0] << "    length = " << bsetSolution.length_path << endl;

}
