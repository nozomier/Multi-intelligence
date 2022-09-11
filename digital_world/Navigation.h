#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "stdlib.h"

#define NodeCount 21 //������
#define AllNodeCount 69 //����·���ܽ�����
#define MaxTaskNum 10 //�����������
#define MaxCarNum 5 //�������


//����״̬����
typedef enum
{
	FREE = 0,
	OCCUPIED = 1,
	OCCUPIED_but_FREE = 2,
} CarStatus;

//���䳵������
typedef enum
{
	MINDISTANCE = 0,//��С�����
	MINTIME = 1,//����������ʱ��
} Strategy;

//�����ṹ��
struct Car
{
	int CarNum = -1;//�������
	int CarNodeNum = -1;//����λ�����ڱ��
	float X, Y;//����λ��
	CarStatus Status = FREE;//����״̬
};

extern int Location[AllNodeCount][2];//�ⲿ��������
extern int Path[AllNodeCount][AllNodeCount];//�ⲿ����·����Ϣ
//��������ṹ��
struct Task
{
	int TaskNum = -1;//������
	int StartNum=-1;//������ʼ�ر��
	int TargetNum=-1;//����Ŀ�ĵر��
	int shareflag = 0;//�Ƿ����ƴ��,0�����ܣ�1����
};

class SA
{
public:
	Strategy strategy;
	Car car[MaxCarNum];
	Task task[MaxTaskNum];

	//ģ���˻��㷨��������ʼ�¶ȡ���ǰ�¶ȡ�������������ÿ���¶��µĵ����������¶�˥��ϵ��
	double T0, T, alpha;
	int max_iter, Lk;

	int CarIndex[MaxCarNum] = { -1,-1,-1,-1,-1 }, TaskIndex[MaxTaskNum] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };//�㷨�м���
	int BestTaskIndex[MaxTaskNum] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }, BestCarIndex[MaxCarNum] = { -1,-1,-1,-1,-1};//���·�����䷽ʽ
	int CarNum = 0, TaskNum = 0;//ʵ�ʳ�����������
	int** bestallpath;;//������䷽��

	SA(Strategy s, Car* c, Task* t);
	~SA();
	void StartSA();//��ʼ����ģ���˻��㷨
	float CalculateCost(int carIndex[], int taskIndex[]);//���ۼ��㣬ͨ��������Ծ���
	float NewPath(float* Cost0);//ģ���˻��㷨�����½�
	void ShowResult();//�ܽ�����
	void InitPath();//��ʼ������·��
};
float CalculateD(float x0, float y0, float x1, float y1);//�����������
void Floyd();//floyd�㷨�����·
#endif


//#ifndef _NAVIGATION_H_
//#define _NAVIGATION_H_
//
//#include <iostream>
//#include <fstream>
//#include <iomanip>
//#include <math.h>
//#include <time.h>
//
//#define B_NodeNum			132		//0-131Ϊ�������
//#define NodeCount			181		//132-180Ϊɳ�ӽ��
//#define B_SchoolBusNodeNum	114		//����У�������
//#define B_BusNodeNum		131		//�����������������
//#define S_SchoolBusNodeNum	165		//ɳ��У�������
//#define S_BusNodeNum		180		//ɳ�ӹ������������
//#define MaxValue			99999999//��ʼ��̾�������ʱ��
//#define IteCount			1000	//��������
//#define AntCount			100		//����������ѭ������
//
//struct Node
//{
//	std::string Name;//�������
//	int LX, LY, PX, PY;//�߼�λ�ú�ʵ��λ��
//	int ConnectionNodeNum[4];//��Χ�Ľ����
//	double ConnectionNodeDistance[4];//����Χ���ľ���
//	double Crowed;//�ý���ӵ���̶�
//	bool RideAllow;//�ý���Ƿ������������г�
//	bool ServiceNode;
//};
//
//struct Road
//{
//	int NodeNum;//�����
//	int LX, LY;//���λ��
//	int MoveDirection;//����һ�������ƶ�����8246��Ӧ��������
//	double Crowed;//�ýڵ��ӵ���̶�
//	int WaitCount;//�ȴ�ʱ����������������ʾģ���н��������ڶ�Ӧλ�õĵȴ�ʱ��
//};
//
//class Ant
//{
//public:
//	Node MapNodes[NodeCount];//���еĽڵ���Ϣ
//	bool CityVisited[NodeCount];//���н��ķ�����Ϣ
//	int TravelCityNum[NodeCount * 2];//�߹��Ľ����Ϣ
//	int TravelCityCount;//�߹������еĽ������----�洢���·��ʱ����������Ϊ�жϱ�־�����ж��У��֮��ĵ�������
//	int CurrentCity;//��ǰ���ڽ����
//	double MoveSpeed;//�ƶ��ٶ�--���ڼ���ʱ��------�����ٶ�ΪĬ���ƶ��ٶȳ��Խ�㷽���ϵ�ӵ���̶�
//	double MoveDistance;//�ƶ��ľ���
//	double MoveTime;//�ƶ����ѵ�ʱ��
//	Ant();
//	~Ant();
//	void ResetAnt();//����������Ϣ---���˵�ͼ�����Ϣ��ȫ������
//	int FindMinTime(int, int);
//	int SelectNextCityNum(bool);//ѡ����һ��ǰ���Ľ��----��Ѱ�ҵ�һ��·�������յ㣬Ȼ���ټ�������ʱ������ȳ̶�
//	void MoveAnt(bool, int);//�ƶ�����λ�ã��������ƶ�����ͻ���ʱ��
//	void Search(int, int, bool, int);//����һ����㵽�յ��·�����������ݲ��������ж��Ƿ�����ƶ��ٶ�
//};
//
//class Navigation
//{
//public:
//	Ant SearchAnt;//��������·��������
//	Ant BestAnt;//�����洢���·��������
//	Navigation();
//	~Navigation();
//	bool returnCross(int, int);
//	void ResetResult();//����и��̵�·�����֡������һ�������ڵĽ����Ϣ
//	void Search1(int, int, bool, int);//����һ�����·��������ͬһУ�������ⲻͬ����֮������·��·�߳���
//	void Search2(int, int, bool, int);//���Զ������ʱ�䡣����ͬһУ�������ⲻͬ����֮������ʱ��·�߳���
//	void SearchTime(int, int, bool, int);
//};
//
//#endif