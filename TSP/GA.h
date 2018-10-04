#pragma once
#ifndef _GA_H_
#define _GA_H_
#define CITY_NUM 150				// TSP_���и���
#define GROUP_NUM 30				// Ⱥ���ģ
#define SON_NUM 32				// �������ӵĸ���	SON_NUM = GROUP_NUM + 2

const double P_INHERIATANCE = 0.01;	// �������
const double P_COPULATION = 0.8;	// �ӽ�����
const int ITERATION_NUM = 1500;		// �Ŵ�����(��������)
const double MAX_INT = 9999999.0;

typedef struct {
	int vex_num, arc_num;			// ������ ����
	int vexs[CITY_NUM];			// ��������
	double arcs[CITY_NUM][CITY_NUM];	// �ڽӾ���
}Graph;

typedef struct {
	double length_path;
	int path[CITY_NUM];
	double P_Reproduction;
}TSP_solution;

TSP_solution TSP_Groups[GROUP_NUM];		// �洢Ⱥ��
TSP_solution Son_solution[SON_NUM];		// �洢�ӽ���ĸ���
int Length_SonSoliton = 0;			// �Ŵ������ĺ��ӵĸ���

void CreateGraph(Graph &G);
void InitialGroup(Graph G);
double CalculateLength(Graph G, TSP_solution newSolution);
void TSP_Evolution(Graph G);	// ģ��������� - ���TSP����	

int Evo_Select(Graph G);		// ѡ����
void Evo_Cross(Graph G, TSP_solution TSP_Father, TSP_solution TSP_Mother);	// �ӽ�����
void Evo_Variation(Graph G, int Index_Variation);	// ���캯��
void Evo_UpdateGroup(Graph G);
void TSP_Evaluate(Graph G);		// TSP - ���ۺ���	

int *Get_Conflict(int Conflict_Father[], int Conflict_Mother[], int Length_Cross, int &Length_Conflict);	// ���س�ͻ������
TSP_solution Handle_Conflict(Graph G, TSP_solution ConflictSolution, int *Detection_Conflict, int *Model_Conflict, int Length_Conflict);	// �����ͻ

void Calc_Probablity(Graph G, double total_length);	// �������
bool Check_path(Graph G, TSP_solution CurrentSolution);
void Display(Graph G);
#endif
