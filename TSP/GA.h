#pragma once
#ifndef _GA_H_
#define _GA_H_
#define CITY_NUM 150				// TSP_城市个数
#define GROUP_NUM 30				// 群体规模
#define SON_NUM 32				// 产生儿子的个数	SON_NUM = GROUP_NUM + 2

const double P_INHERIATANCE = 0.01;	// 变异概率
const double P_COPULATION = 0.8;	// 杂交概率
const int ITERATION_NUM = 1500;		// 遗传次数(迭代次数)
const double MAX_INT = 9999999.0;

typedef struct {
	int vex_num, arc_num;			// 顶点数 边数
	int vexs[CITY_NUM];			// 顶点向量
	double arcs[CITY_NUM][CITY_NUM];	// 邻接矩阵
}Graph;

typedef struct {
	double length_path;
	int path[CITY_NUM];
	double P_Reproduction;
}TSP_solution;

TSP_solution TSP_Groups[GROUP_NUM];		// 存储群体
TSP_solution Son_solution[SON_NUM];		// 存储杂交后的个体
int Length_SonSoliton = 0;			// 遗传产生的孩子的个数

void CreateGraph(Graph &G);
void InitialGroup(Graph G);
double CalculateLength(Graph G, TSP_solution newSolution);
void TSP_Evolution(Graph G);	// 模拟生物进化 - 解决TSP问题	

int Evo_Select(Graph G);		// 选择函数
void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother);	// 杂交函数
void Evo_Variation(Graph G, int Index_Variation);	// 变异函数
void Evo_UpdateGroup(Graph G);
void TSP_Evaluate(Graph G);		// TSP - 评价函数	

int *Get_Conflict(int Conflict_Father[], int Conflict_Mother[], int Length_Cross, int &Length_Conflict);	// 返回冲突的数组
TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int *Detection_Conflict, int *Model_Conflict, int Length_Conflict);	// 解决冲突

void Calc_Probablity(Graph G, double total_length);	// 计算概率
bool Check_path(Graph G, TSP_solution CurrentSolution);
void Display(Graph G);
#endif
