#pragma once

#include "UTF.h"
#include <QtWidgets/QWidget>
#include "ui_QtSGS.h"
#include "Navigation.h"
#include <QUdpSocket>

#define Speed 2 //���������ٶ�
struct Road
{
    int NodeNum=-1;//�����
    //int Count;//���н������
    float LX, LY;//���λ��
    float MoveDirectionX=0;//����һ�������ƶ�����
    float MoveDirectionY=0;//����һ�������ƶ�����
};
//���͵����ݰ�
struct SendInfo
{
    double x;//Ŀ�������
    double y;//Ŀ���������
    double theta;
};
//���յ����ݰ�
struct RecvInfo
{
    double x;//Ŀ�������
    double y;//Ŀ���������
};
class QtSGS : public QWidget
{
    Q_OBJECT

public:
    QtSGS(QWidget *parent = Q_NULLPTR);
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);
    void closeEvent(QCloseEvent*);
    bool SearchJudge();//������ǰ���ж�
    QUdpSocket* m_receiver; //udp����
    QUdpSocket* m_sender; //udp����

private:
    Ui::QtSGSClass* ui;
    QImage Img;//��ͼ
    QRect ImgRec;//��ͼ��ͼƬλ��
    Car car[MaxCarNum];
    int carnum;
    Task task[MaxTaskNum];
    int hour, minute, second;//ʱ����
    int TimerID;//��ʱ��ID
    bool Simulation[MaxCarNum];//ģ���н�״̬
    bool udpconnect;//udp�Ƿ�����
    Road paintpath[MaxCarNum][AllNodeCount*MaxTaskNum];//��Ҫ���Ƶ�·����Ϣ
    int currentroad[MaxCarNum];//��¼�����ߵ��ڼ�������
    RecvInfo data;
    bool udprecv;//�Ƿ���յ����ص���������
public slots:
    void TimerFaster();//�ӿ�����ʱ��
    void TimerSlower();//��������ʱ��
    void clickStartSearch();//����������ť�󴥷��ĺ���
    void clickStopSearchButton();//����ֹͣ���������ĺ���
    void readData();
    void clickUDPconnectButton();//��������UDP�����ĺ���
    void changeCarStartNodeNum1();//������������κβ���������¶�Ӧ����Ϣ
    void changeCarStartNodeNum2();//������������κβ���������¶�Ӧ����Ϣ
    void changeCarStartNodeNum3();//������������κβ���������¶�Ӧ����Ϣ
    void changeCarStartNodeNum4();//������������κβ���������¶�Ӧ����Ϣ
    void changeCarStartNodeNum5();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum1();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum2();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum3();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum4();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum5();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum6();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum7();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum8();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum9();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskStartNodeNum10();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum1();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum2();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum3();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum4();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum5();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum6();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum7();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum8();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum9();//������������κβ���������¶�Ӧ����Ϣ
    void changeTaskTargetNodeNum10();//������������κβ���������¶�Ӧ����Ϣ
};
