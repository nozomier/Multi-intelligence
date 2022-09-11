#pragma once

#include "UTF.h"
#include <QtWidgets/QWidget>
#include "ui_QtSGS.h"
#include "Navigation.h"
#include <QUdpSocket>

#define Speed 2 //车辆运行速度
struct Road
{
    int NodeNum=-1;//结点编号
    //int Count;//所有结点总数
    float LX, LY;//结点位置
    float MoveDirectionX=0;//向下一个结点的移动向量
    float MoveDirectionY=0;//向下一个结点的移动向量
};
//发送的数据包
struct SendInfo
{
    double x;//目标点坐标
    double y;//目标点纵坐标
    double theta;
};
//接收的数据包
struct RecvInfo
{
    double x;//目标点坐标
    double y;//目标点纵坐标
};
class QtSGS : public QWidget
{
    Q_OBJECT

public:
    QtSGS(QWidget *parent = Q_NULLPTR);
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);
    void closeEvent(QCloseEvent*);
    bool SearchJudge();//搜索的前置判断
    QUdpSocket* m_receiver; //udp接收
    QUdpSocket* m_sender; //udp发送

private:
    Ui::QtSGSClass* ui;
    QImage Img;//地图
    QRect ImgRec;//地图的图片位置
    Car car[MaxCarNum];
    int carnum;
    Task task[MaxTaskNum];
    int hour, minute, second;//时分秒
    int TimerID;//计时器ID
    bool Simulation[MaxCarNum];//模拟行进状态
    bool udpconnect;//udp是否连接
    Road paintpath[MaxCarNum][AllNodeCount*MaxTaskNum];//需要绘制的路径信息
    int currentroad[MaxCarNum];//记录车辆走到第几个点了
    RecvInfo data;
    bool udprecv;//是否接收到发回的坐标数据
public slots:
    void TimerFaster();//加快虚拟时间
    void TimerSlower();//减慢虚拟时间
    void clickStartSearch();//单击搜索按钮后触发的函数
    void clickStopSearchButton();//单击停止导航触发的函数
    void readData();
    void clickUDPconnectButton();//单击连接UDP触发的函数
    void changeCarStartNodeNum1();//如果下拉框有任何操作。则更新对应的信息
    void changeCarStartNodeNum2();//如果下拉框有任何操作。则更新对应的信息
    void changeCarStartNodeNum3();//如果下拉框有任何操作。则更新对应的信息
    void changeCarStartNodeNum4();//如果下拉框有任何操作。则更新对应的信息
    void changeCarStartNodeNum5();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum1();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum2();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum3();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum4();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum5();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum6();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum7();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum8();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum9();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskStartNodeNum10();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum1();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum2();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum3();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum4();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum5();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum6();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum7();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum8();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum9();//如果下拉框有任何操作。则更新对应的信息
    void changeTaskTargetNodeNum10();//如果下拉框有任何操作。则更新对应的信息
};
