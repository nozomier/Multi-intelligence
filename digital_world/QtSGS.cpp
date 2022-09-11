#include "QtSGS.h"

QtSGS::QtSGS(QWidget* parent)
    : QWidget(parent), ui(new Ui::QtSGSClass)
{
    ui->setupUi(this);
    Img = QImage("./map.png");
    ImgRec = QRect(500, 40, 971, 573);
    hour = 6;
    minute = 0;
    second = 0;
    for (int i = 0; i < MaxCarNum; i++)
    {
        Simulation[i] = false;
        currentroad[i] = 0;
    }
    udpconnect = false;
    udprecv = false;
    ui->changeTimeF->setEnabled(true);
    ui->changeTimeS->setEnabled(false);
    ui->StopSearch->setEnabled(false);

    m_sender = new QUdpSocket(this);
    m_receiver = new QUdpSocket(this);
    connect(ui->changeTimeF, SIGNAL(clicked()), this, SLOT(TimerFaster()));
    connect(ui->changeTimeS, SIGNAL(clicked()), this, SLOT(TimerSlower()));
    connect(ui->StartSearch, SIGNAL(clicked()), this, SLOT(clickStartSearch()));
    connect(ui->StopSearch, SIGNAL(clicked()), this, SLOT(clickStopSearchButton()));
    connect(ui->CarStartBox_1, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCarStartNodeNum1()));
    connect(ui->CarStartBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCarStartNodeNum2()));
    connect(ui->CarStartBox_3, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCarStartNodeNum3()));
    connect(ui->CarStartBox_4, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCarStartNodeNum4()));
    connect(ui->CarStartBox_5, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeCarStartNodeNum5()));
    connect(ui->TaskStartBox_1, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum1()));
    connect(ui->TaskStartBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum2()));
    connect(ui->TaskStartBox_3, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum3()));
    connect(ui->TaskStartBox_4, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum4()));
    connect(ui->TaskStartBox_5, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum5()));
    connect(ui->TaskStartBox_1, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum6()));
    connect(ui->TaskStartBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum7()));
    connect(ui->TaskStartBox_3, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum8()));
    connect(ui->TaskStartBox_4, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum9()));
    connect(ui->TaskStartBox_5, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskStartNodeNum10()));
    connect(ui->TaskTargetBox_1, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum1()));
    connect(ui->TaskTargetBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum2()));
    connect(ui->TaskTargetBox_3, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum3()));
    connect(ui->TaskTargetBox_4, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum4()));
    connect(ui->TaskTargetBox_5, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum5()));
    connect(ui->TaskTargetBox_1, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum6()));
    connect(ui->TaskTargetBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum7()));
    connect(ui->TaskTargetBox_3, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum8()));
    connect(ui->TaskTargetBox_4, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum9()));
    connect(ui->TaskTargetBox_5, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeTaskTargetNodeNum10()));
    connect(ui->UDPconnectButton, SIGNAL(clicked()), this, SLOT(clickUDPconnectButton()));
    TimerID = startTimer(1000);
    Floyd();//floyd算法求最短路
}

//QByteArray SendInfo2Byte(float data)
//{
//    QByteArray byte_data;
//
//    char* data_char = (char*)&data;
//    for (int index = 3; index >= 0; index--)
//    {
//        byte_data.append(data_char[index]);
//    }
//    return byte_data;
//}


void QtSGS::timerEvent(QTimerEvent* e)
{
    if (e->timerId() == TimerID)
    {
        second++;
        if (second == 60)
        {
            second = 0;
            minute++;
            if (minute == 60)
            {
                minute = 0;
                hour++;
            }
        }

        for (int i = 0; i < MaxCarNum; i++)
        {
            if (Simulation[i])
            {
                int j = currentroad[i];
                if (paintpath[i][j].NodeNum == -1)
                {
                    Simulation[i] = false;             
                    break;
                }
                car[i].X = car[i].X + paintpath[i][j].MoveDirectionX * Speed;
                car[i].Y = car[i].Y + paintpath[i][j].MoveDirectionY * Speed;

               
                //下面进行车辆运行到哪个点的判断
                if (paintpath[i][j].MoveDirectionX > 0 && paintpath[i][j].MoveDirectionY < 0)
                {
                    if (car[i].X > paintpath[i][j + 1].LX && car[i].Y < paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX > 0 && paintpath[i][j].MoveDirectionY == 0)
                {
                    if (car[i].X > paintpath[i][j + 1].LX)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX > 0 && paintpath[i][j].MoveDirectionY > 0)
                {
                    if (car[i].X > paintpath[i][j + 1].LX && car[i].Y > paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX < 0 && paintpath[i][j].MoveDirectionY < 0)
                {
                    if (car[i].X < paintpath[i][j + 1].LX && car[i].Y < paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX < 0 && paintpath[i][j].MoveDirectionY == 0)
                {
                    if (car[i].X < paintpath[i][j + 1].LX)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX < 0 && paintpath[i][j].MoveDirectionY > 0)
                {
                    if (car[i].X < paintpath[i][j + 1].LX && car[i].Y > paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX == 0 && paintpath[i][j].MoveDirectionY < 0)
                {
                    if (car[i].Y < paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
                else if (paintpath[i][j].MoveDirectionX == 0 && paintpath[i][j].MoveDirectionY > 0)
                {
                    if (car[i].Y > paintpath[i][j + 1].LY)
                    {
                        currentroad[i]++;
                        car[i].X = paintpath[i][j + 1].LX;
                        car[i].Y = paintpath[i][j + 1].LY;
                    }
                }
            }
        }
        if (Simulation[0]&&udpconnect)
        {
            SendInfo data = {(car[0].X-Location[car[0].CarNodeNum-1][0])/10,(Location[car[0].CarNodeNum-1][1] - car[0].Y)/10 ,atan(-paintpath[0][currentroad[0]].MoveDirectionY/ paintpath[0][currentroad[0]].MoveDirectionX)};
            //SendInfo data = {(paintpath[0][currentroad[0]+1].LX- Location[car[0].CarNodeNum - 1][0])/10,(Location[car[0].CarNodeNum - 1][1]-paintpath[0][currentroad[0] + 1].LY)/10 ,-atan(paintpath[0][currentroad[0]].MoveDirectionY/ paintpath[0][currentroad[0]].MoveDirectionX)};

            char* pdata = (char*)&data;
            quint16 port = ui->textEdit_4->toPlainText().toUInt();
            QHostAddress address;//接收端ip
            address.setAddress(ui->textEdit_3->toPlainText());
            m_sender->writeDatagram(pdata, sizeof(SendInfo), address, port);
            //四个参数分别是要发送的数据（将其转换一下类型），发送数据的大小，第三个参数是目标的ip，最后一个参数是端口号
        }
    }
    repaint();
}




void QtSGS::paintEvent(QPaintEvent * e)
{
    QPainter pt(this);
    QString str = QString::number(hour) + ":" + QString::number(minute) + ":" + QString::number(second);
    ui->VirtualTime->setText(str);
    if (1)
        pt.drawImage(ImgRec, Img);
    QBrush br;
    br.setStyle(Qt::SolidPattern);
    for (int i = 0; i < MaxCarNum; i++)
    {
        if (Simulation[i])
        {
            str = "";
            int j;
            for (j = 0; paintpath[i][j + 1].NodeNum != -1; j++)
            {
                if (paintpath[i][j].NodeNum <= NodeCount)
                {
                    str = str + QString::number(paintpath[i][j].NodeNum) + "-";
                }
            }
            str = str + QString::number(paintpath[i][j].NodeNum);
            switch (i)
            {
            case 0: 
                br.setColor(Qt::GlobalColor::red);
                pt.setBrush(br); 
                pt.setPen(QPen(br, 2, Qt::SolidLine));
                ui->CarpathTipInfo1->setText(str);
                if (udprecv)
                {
                    pt.drawRect(data.x - 5, data.y - 5, 10, 10);
                }
                break;
            case 1:
                br.setColor(Qt::GlobalColor::darkRed);
                pt.setBrush(br);
                pt.setPen(QPen(br, 2, Qt::SolidLine));
                ui->CarpathTipInfo2->setText(str);
                break;
            case 2:
                br.setColor(Qt::GlobalColor::blue);
                pt.setBrush(br);
                pt.setPen(QPen(br, 2, Qt::SolidLine));
                ui->CarpathTipInfo3->setText(str);
                break;
            case 3:
                br.setColor(Qt::GlobalColor::green);
                pt.setBrush(br);
                pt.setPen(QPen(br, 2, Qt::SolidLine));
                ui->CarpathTipInfo4->setText(str);
                break;
            default:
                br.setColor(Qt::GlobalColor::black);
                pt.setBrush(br);
                pt.setPen(QPen(br, 2, Qt::SolidLine));
                ui->CarpathTipInfo5->setText(str);
                break;
            }
            j = currentroad[i];
            pt.drawEllipse(car[i].X - 5, car[i].Y - 5, 10, 10);
            if (paintpath[i][j + 1].NodeNum != -1)
            {
                pt.drawLine(car[i].X, car[i].Y, paintpath[i][j + 1].LX, paintpath[i][j + 1].LY);
                for (; paintpath[i][j + 2].NodeNum != -1; j++)
                {
                    pt.drawLine(paintpath[i][j + 1].LX, paintpath[i][j + 1].LY, paintpath[i][j + 2].LX, paintpath[i][j + 2].LY);
                }
            }
        }
    }
}

void QtSGS::closeEvent(QCloseEvent* e)
{
}
//
bool QtSGS::SearchJudge()//搜索的前置判断
{

    if (ui->CarStartBox_1->currentIndex() == 0 && ui->CarStartBox_2->currentIndex() == 0 && ui->CarStartBox_3->currentIndex() == 0 && ui->CarStartBox_4->currentIndex() == 0 && ui->CarStartBox_5->currentIndex() == 0)
    {
        QMessageBox::information(this, "警告", QString("请选择车辆"));
        return false;
    }
    if((ui->TaskStartBox_1->currentIndex() != 0 || ui->TaskTargetBox_1->currentIndex() != 0) && ui->TaskStartBox_1->currentIndex() == ui->TaskTargetBox_1->currentIndex())
    {
        task[0].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_2->currentIndex() != 0 || ui->TaskTargetBox_2->currentIndex() != 0) && ui->TaskStartBox_2->currentIndex() == ui->TaskTargetBox_2->currentIndex())
    {
        task[1].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_3->currentIndex() != 0 || ui->TaskTargetBox_3->currentIndex() != 0) && ui->TaskStartBox_3->currentIndex() == ui->TaskTargetBox_3->currentIndex())
    {
        task[2].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_4->currentIndex() != 0 || ui->TaskTargetBox_4->currentIndex() != 0) && ui->TaskStartBox_4->currentIndex() == ui->TaskTargetBox_4->currentIndex())
    {
        task[3].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_5->currentIndex() != 0 || ui->TaskTargetBox_5->currentIndex() != 0) && ui->TaskStartBox_5->currentIndex() == ui->TaskTargetBox_5->currentIndex())
    {
        task[4].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_6->currentIndex() != 0 || ui->TaskTargetBox_6->currentIndex() != 0) && ui->TaskStartBox_6->currentIndex() == ui->TaskTargetBox_6->currentIndex())
    {
        task[5].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_7->currentIndex() != 0 || ui->TaskTargetBox_7->currentIndex() != 0) && ui->TaskStartBox_7->currentIndex() == ui->TaskTargetBox_7->currentIndex())
    {
        task[6].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_8->currentIndex() != 0 || ui->TaskTargetBox_8->currentIndex() != 0) && ui->TaskStartBox_8->currentIndex() == ui->TaskTargetBox_8->currentIndex())
    {
        task[7].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_9->currentIndex() != 0 || ui->TaskTargetBox_9->currentIndex() != 0) && ui->TaskStartBox_9->currentIndex() == ui->TaskTargetBox_9->currentIndex())
    {
        task[8].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if ((ui->TaskStartBox_10->currentIndex() != 0 || ui->TaskTargetBox_10->currentIndex() != 0 )&& ui->TaskStartBox_10->currentIndex() == ui->TaskTargetBox_10->currentIndex())
    {
        task[9].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，起始点和终点不能相同"));
        return false;
    }
    if (0==ui->TaskStartBox_1->currentIndex() + ui->TaskTargetBox_1->currentIndex()+ ui->TaskStartBox_2->currentIndex() + ui->TaskTargetBox_2->currentIndex()+ ui->TaskStartBox_3->currentIndex() + ui->TaskTargetBox_3->currentIndex()+ ui->TaskStartBox_4->currentIndex() + ui->TaskTargetBox_4->currentIndex()+ ui->TaskStartBox_5->currentIndex() + ui->TaskTargetBox_5->currentIndex()+ ui->TaskStartBox_6->currentIndex() + ui->TaskTargetBox_6->currentIndex()+ ui->TaskStartBox_7->currentIndex() + ui->TaskTargetBox_7->currentIndex()+ ui->TaskStartBox_8->currentIndex() + ui->TaskTargetBox_8->currentIndex() + ui->TaskStartBox_9->currentIndex() + ui->TaskTargetBox_9->currentIndex() + ui->TaskStartBox_10->currentIndex() + ui->TaskTargetBox_10->currentIndex())
    {
        QMessageBox::information(this, "警告", QString("请设置任务"));
        return false;
    }
    if ((ui->TaskStartBox_1->currentIndex() != 0 || ui->TaskTargetBox_1->currentIndex() != 0) && 0==ui->TaskStartBox_1->currentIndex() * ui->TaskTargetBox_1->currentIndex())
    {
        task[0].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_2->currentIndex() != 0 || ui->TaskTargetBox_2->currentIndex() != 0) && ui->TaskStartBox_2->currentIndex() * ui->TaskTargetBox_2->currentIndex()==0)
    {
        task[1].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_3->currentIndex() != 0 || ui->TaskTargetBox_3->currentIndex() != 0) && ui->TaskStartBox_3->currentIndex() * ui->TaskTargetBox_3->currentIndex()==0)
    {
        task[2].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_4->currentIndex() != 0 || ui->TaskTargetBox_4->currentIndex() != 0) && ui->TaskStartBox_4->currentIndex() * ui->TaskTargetBox_4->currentIndex()==0)
    {
        task[3].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_5->currentIndex() != 0 || ui->TaskTargetBox_5->currentIndex() != 0) && ui->TaskStartBox_5->currentIndex() * ui->TaskTargetBox_5->currentIndex()==0)
    {
        task[4].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_6->currentIndex() != 0 || ui->TaskTargetBox_6->currentIndex() != 0) && ui->TaskStartBox_6->currentIndex() * ui->TaskTargetBox_6->currentIndex()==0)
    {
        task[5].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_7->currentIndex() != 0 || ui->TaskTargetBox_7->currentIndex() != 0) && ui->TaskStartBox_7->currentIndex() * ui->TaskTargetBox_7->currentIndex()==0)
    {
        task[6].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_8->currentIndex() != 0 || ui->TaskTargetBox_8->currentIndex() != 0) && ui->TaskStartBox_8->currentIndex() * ui->TaskTargetBox_8->currentIndex()==0)
    {
        task[7].TaskNum = -1;        
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_9->currentIndex() != 0 || ui->TaskTargetBox_9->currentIndex() != 0) && ui->TaskStartBox_9->currentIndex() * ui->TaskTargetBox_9->currentIndex()==0)
    {
        task[8].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }
    if ((ui->TaskStartBox_10->currentIndex() != 0 || ui->TaskTargetBox_10->currentIndex() != 0) && ui->TaskStartBox_10->currentIndex() * ui->TaskTargetBox_10->currentIndex()==0)
    {
        task[9].TaskNum = -1;
        QMessageBox::information(this, "警告", QString("任务设置失败，设置未完全"));
        return false;
    }

    if (ui->NavigationBox->currentIndex() == 0)
    {
        QMessageBox::information(this, "警告", QString("请选择导航策略"));
        return false;
    }
    return true;
}

void QtSGS::TimerFaster()//加快虚拟时间
{
    killTimer(TimerID);
    TimerID = startTimer(50);
    ui->changeTimeF->setEnabled(false);
    ui->changeTimeS->setEnabled(true);
}

void QtSGS::TimerSlower()//减慢虚拟时间
{
    killTimer(TimerID);
    TimerID = startTimer(1000);
    ui->changeTimeF->setEnabled(true);
    ui->changeTimeS->setEnabled(false);
}
//
void QtSGS::clickStartSearch()//单击搜索按钮后触发的函数
{
    udprecv = false;
    if (!SearchJudge())//前置判断，判断输入信息是否有误
        return;
    srand((unsigned)time(NULL));
    Strategy s;
    if (ui->NavigationBox->currentIndex() == 1)//如果是最短路径策略
    {
        s = MINDISTANCE;
    }
    else if (ui->NavigationBox->currentIndex() == 2)//如果是时间最短策略
    {
        s = MINTIME;
    }
    SA* sa = new SA(s, car, task);
    sa->StartSA();
    int i = 0, j = 0, k = 0,m=0;
    //初始化paintpath
    for (i = 0; i < MaxCarNum; i++)
    {
        currentroad[i] = 0;
        for (j = 0; j < AllNodeCount * MaxTaskNum; j++)
        {
            paintpath[i][j].NodeNum = -1;
        }
    }
    for (i = 0; i < sa->CarNum; i++)
    {
        if (sa->bestallpath[i][0] == -1)
        {
            break;
        }
        for (m = 0; m < MaxCarNum; m++)
        {
            if (car[m].Status==FREE && car[m].CarNodeNum == sa->bestallpath[i][0])
            {
                car[m].Status = OCCUPIED;
                break;//Simulation[m] = true;
            }
        }
        k = 0;
        for (j = 0; sa->bestallpath[i][j + 1] != -1; j++)
        {
            paintpath[m][k++].NodeNum = sa->bestallpath[i][j];//出发点
            if (sa->bestallpath[i][j] == sa->bestallpath[i][j + 1])
            {
                j = j + 1;//如果车辆起始点与任务起始点重合
            }
            int temp = Path[sa->bestallpath[i][j] - 1][sa->bestallpath[i][j + 1] - 1] + 1;
            while (temp != sa->bestallpath[i][j + 1])
            {
                paintpath[m][k++].NodeNum = temp;
                temp = Path[temp - 1][sa->bestallpath[i][j + 1] - 1] + 1;
            }
        }
        paintpath[m][k++].NodeNum = sa->bestallpath[i][j];//最后抵达的
        for (j = 0; j < k; j++)//赋坐标
        {
            paintpath[m][j].LX = Location[paintpath[m][j].NodeNum - 1][0];
            paintpath[m][j].LY = Location[paintpath[m][j].NodeNum - 1][1];
        }
        for (j = 0; j < k-1; j++)//赋方向
        {
            float d = CalculateD(paintpath[m][j + 1].LX, paintpath[m][j + 1].LY, paintpath[m][j].LX, paintpath[m][j].LY);
            paintpath[m][j].MoveDirectionX = (paintpath[m][j + 1].LX - paintpath[m][j].LX) / d;
            paintpath[m][j].MoveDirectionY = (paintpath[m][j + 1].LY - paintpath[m][j].LY) / d;

            paintpath[m][j].MoveDirectionY = (paintpath[m][j + 1].LY - paintpath[m][j].LY) / d;
        }
        //paintpath[m][j].MoveDirectionX = paintpath[m][j-1].MoveDirectionX;
        //paintpath[m][j].MoveDirectionY = paintpath[m][j - 1].MoveDirectionY;


    }
    carnum = sa->CarNum;
    delete sa;
    for (i = 0; i < MaxCarNum; i++)
    {
        for (m = 0; m < MaxCarNum; m++)
        {
            if (car[m].Status == OCCUPIED)
            {
                Simulation[m] = true;
            }
        }
    }
    ui->StartSearch->setEnabled(false);
    ui->StopSearch->setEnabled(true);
    repaint();
    return;
}

 
void QtSGS::clickStopSearchButton()
{
    for (int i = 0; i < MaxCarNum; i++)
    {
        this->Simulation[i] = false;
    }
    ui->StartSearch->setEnabled(true);
    ui->StopSearch->setEnabled(false);
    //全部初始化
    ui->CarStartBox_1->setCurrentIndex(0);
    ui->CarStartBox_2->setCurrentIndex(0);
    ui->CarStartBox_3->setCurrentIndex(0);
    ui->CarStartBox_4->setCurrentIndex(0);
    ui->CarStartBox_5->setCurrentIndex(0);
    ui->TaskStartBox_1->setCurrentIndex(0);
    ui->TaskStartBox_2->setCurrentIndex(0);
    ui->TaskStartBox_3->setCurrentIndex(0);
    ui->TaskStartBox_4->setCurrentIndex(0);
    ui->TaskStartBox_5->setCurrentIndex(0);
    ui->TaskStartBox_6->setCurrentIndex(0);
    ui->TaskStartBox_7->setCurrentIndex(0);
    ui->TaskStartBox_8->setCurrentIndex(0);
    ui->TaskStartBox_9->setCurrentIndex(0);
    ui->TaskStartBox_10->setCurrentIndex(0);
    ui->TaskTargetBox_1->setCurrentIndex(0);
    ui->TaskTargetBox_2->setCurrentIndex(0);
    ui->TaskTargetBox_3->setCurrentIndex(0);
    ui->TaskTargetBox_4->setCurrentIndex(0);
    ui->TaskTargetBox_5->setCurrentIndex(0);
    ui->TaskTargetBox_6->setCurrentIndex(0);
    ui->TaskTargetBox_7->setCurrentIndex(0);
    ui->TaskTargetBox_8->setCurrentIndex(0);
    ui->TaskTargetBox_9->setCurrentIndex(0);
    ui->TaskTargetBox_10->setCurrentIndex(0);
    ui->NavigationBox->setCurrentIndex(0);
    QString str = "未启用";
    ui->CarpathTipInfo1->setText(str);
    ui->CarpathTipInfo2->setText(str);
    ui->CarpathTipInfo3->setText(str);
    ui->CarpathTipInfo4->setText(str);
    ui->CarpathTipInfo5->setText(str);
    for (int i = 0; i < MaxCarNum; i++)
    {
        car[i].CarNum = -1;
        car[i].CarNodeNum = -1;
        car[i].Status=FREE;
    }
    for (int i = 0; i < MaxTaskNum; i++)
    {
        task[i].TaskNum = -1;
        task[i].StartNum = -1;
        task[i].TargetNum = -1;
    }
}

void QtSGS::changeCarStartNodeNum1()
{
    if (ui->CarStartBox_1->currentIndex() == 0)
    {
        car[0].CarNodeNum = -1;
        car[0].CarNum = -1;
    }
    else
    {
        car[0].CarNodeNum = ui->CarStartBox_1->currentIndex();
        car[0].CarNum = 1;
        car[0].X = Location[car[0].CarNodeNum - 1][0];
        car[0].Y = Location[car[0].CarNodeNum - 1][1];
    }
}
void QtSGS::changeCarStartNodeNum2()
{
    if (ui->CarStartBox_2->currentIndex() == 0)
    {
        car[1].CarNodeNum = -1;
        car[1].CarNum = -1;
    }
    else
    {
        car[1].CarNodeNum = ui->CarStartBox_2->currentIndex();
        car[1].CarNum = 2;
        car[1].X = Location[car[1].CarNodeNum - 1][0];
        car[1].Y = Location[car[1].CarNodeNum - 1][1];
    }
}
void QtSGS::changeCarStartNodeNum3()
{
    if (ui->CarStartBox_3->currentIndex() == 0)
    {
        car[2].CarNodeNum = -1;
        car[2].CarNum = -1;
    }
    else
    {
        car[2].CarNodeNum = ui->CarStartBox_3->currentIndex();
        car[2].CarNum = 3;
        car[2].X = Location[car[2].CarNodeNum - 1][0];
        car[2].Y = Location[car[2].CarNodeNum - 1][1];
    }
}
void QtSGS::changeCarStartNodeNum4()
{
    if (ui->CarStartBox_4->currentIndex() == 0)
    {
        car[3].CarNodeNum = -1;
        car[3].CarNum = -1;
    }
    else
    {
        car[3].CarNodeNum = ui->CarStartBox_4->currentIndex();
        car[3].CarNum = 4;
        car[3].X = Location[car[3].CarNodeNum - 1][0];
        car[3].Y = Location[car[3].CarNodeNum - 1][1];
    }
}
void QtSGS::changeCarStartNodeNum5()
{
    if (ui->CarStartBox_5->currentIndex() == 0)
    {
        car[4].CarNodeNum = -1;
        car[4].CarNum = -1;
    }
    else
    {
        car[4].CarNodeNum = ui->CarStartBox_5->currentIndex();
        car[4].CarNum = 5;
        car[4].X = Location[car[4].CarNodeNum - 1][0];
        car[4].Y = Location[car[4].CarNodeNum - 1][1];
    }
}

void QtSGS::changeTaskStartNodeNum1()
{
    if (ui->TaskStartBox_1->currentIndex() == 0)
    {
        task[0].StartNum = -1;
        task[0].TaskNum = -1;
    }
    else
    {
        task[0].StartNum = ui->TaskStartBox_1->currentIndex();
        task[0].TaskNum = 1;
    }
}
void QtSGS::changeTaskStartNodeNum2()
{
    if (ui->TaskStartBox_2->currentIndex() == 0)
    {
        task[1].StartNum = -1;
        task[1].TaskNum = -1;
    }
    else
    {
        task[1].StartNum = ui->TaskStartBox_2->currentIndex();
        task[1].TaskNum = 2;
    }
}
void QtSGS::changeTaskStartNodeNum3()
{
    if (ui->TaskStartBox_3->currentIndex() == 0)
    {
        task[2].StartNum = -1;
        task[2].TaskNum = -1;
    }
    else
    {
        task[2].StartNum = ui->TaskStartBox_3->currentIndex();
        task[2].TaskNum = 3;
    }
}
void QtSGS::changeTaskStartNodeNum4()
{
    if (ui->TaskStartBox_4->currentIndex() == 0)
    {
        task[3].StartNum = -1;
        task[3].TaskNum = -1;
    }
    else
    {
        task[3].StartNum = ui->TaskStartBox_4->currentIndex();
        task[3].TaskNum = 4;
    }
}
void QtSGS::changeTaskStartNodeNum5()
{
    if (ui->TaskStartBox_5->currentIndex() == 0)
    {
        task[4].StartNum = -1;
        task[4].TaskNum = -1;
    }
    else
    {
        task[4].StartNum = ui->TaskStartBox_5->currentIndex();
        task[4].TaskNum = 5;
    }
}
void QtSGS::changeTaskStartNodeNum6()
{
    if (ui->TaskStartBox_6->currentIndex() == 0)
    {
        task[5].StartNum = -1;
        task[5].TaskNum = -1;
    }
    else
    {
        task[5].StartNum = ui->TaskStartBox_6->currentIndex();
        task[5].TaskNum = 6;
    }
}
void QtSGS::changeTaskStartNodeNum7()
{
    if (ui->TaskStartBox_7->currentIndex() == 0)
    {
        task[6].StartNum = -1;
        task[6].TaskNum = -1;
    }
    else
    {
        task[6].StartNum = ui->TaskStartBox_7->currentIndex();
        task[6].TaskNum = 7;
    }
}
void QtSGS::changeTaskStartNodeNum8()
{
    if (ui->TaskStartBox_8->currentIndex() == 0)
    {
        task[7].StartNum = -1;
        task[7].TaskNum = -1;
    }
    else
    {
        task[7].StartNum = ui->TaskStartBox_8->currentIndex();
        task[7].TaskNum = 8;
    }
}
void QtSGS::changeTaskStartNodeNum9()
{
    if (ui->TaskStartBox_9->currentIndex() == 0)
    {
        task[8].StartNum = -1;
        task[8].TaskNum = -1;
    }
    else
    {
        task[8].StartNum = ui->TaskStartBox_9->currentIndex();
        task[8].TaskNum = 9;
    }
}
void QtSGS::changeTaskStartNodeNum10()
{
    if (ui->TaskStartBox_10->currentIndex() == 0)
    {
        task[9].StartNum = -1;
        task[9].TaskNum = -1;
    }
    else
    {
        task[9].StartNum = ui->TaskStartBox_10->currentIndex();
        task[9].TaskNum = 10;
    }
}

void QtSGS::changeTaskTargetNodeNum1()
{
    if (ui->TaskTargetBox_1->currentIndex() == 0)
    {
        task[0].TargetNum = -1;
        task[0].TaskNum = -1;
    }
    else
    {
        task[0].TargetNum = ui->TaskTargetBox_1->currentIndex();
        task[0].TaskNum = 1;
    }
}
void QtSGS::changeTaskTargetNodeNum2()
{
    if (ui->TaskTargetBox_2->currentIndex() == 0)
    {
        task[1].TargetNum = -1;
        task[1].TaskNum = -1;
    }
    else
    {
        task[1].TargetNum = ui->TaskTargetBox_2->currentIndex();
        task[1].TaskNum = 2;
    }
}
void QtSGS::changeTaskTargetNodeNum3()
{
    if (ui->TaskTargetBox_3->currentIndex() == 0)
    {
        task[2].TargetNum = -1;
        task[2].TaskNum = -1;
    }
    else
    {
        task[2].TargetNum = ui->TaskTargetBox_3->currentIndex();
        task[2].TaskNum = 3;
    }
}
void QtSGS::changeTaskTargetNodeNum4()
{
    if (ui->TaskTargetBox_4->currentIndex() == 0)
    {
        task[3].TargetNum = -1;
        task[3].TaskNum = -1;
    }
    else
    {
        task[3].TargetNum = ui->TaskTargetBox_4->currentIndex();
        task[3].TaskNum = 4;
    }
}
void QtSGS::changeTaskTargetNodeNum5()
{
    if (ui->TaskTargetBox_5->currentIndex() == 0)
    {
        task[4].TargetNum = -1;
        task[4].TaskNum = -1;
    }
    else
    {
        task[4].TargetNum = ui->TaskTargetBox_5->currentIndex();
        task[4].TaskNum = 5;
    }
}
void QtSGS::changeTaskTargetNodeNum6()
{
    if (ui->TaskTargetBox_6->currentIndex() == 0)
    {
        task[5].TargetNum = -1;
        task[5].TaskNum = -1;
    }
    else
    {
        task[5].TargetNum = ui->TaskTargetBox_6->currentIndex();
        task[5].TaskNum = 6;
    }
}
void QtSGS::changeTaskTargetNodeNum7()
{
    if (ui->TaskTargetBox_7->currentIndex() == 0)
    {
        task[6].TargetNum = -1;
        task[6].TaskNum = -1;
    }
    else
    {
        task[6].TargetNum = ui->TaskTargetBox_7->currentIndex();
        task[6].TaskNum = 7;
    }
}
void QtSGS::changeTaskTargetNodeNum8()
{
    if (ui->TaskTargetBox_8->currentIndex() == 0)
    {
        task[7].TargetNum = -1;
        task[7].TaskNum = -1;
    }
    else
    {
        task[7].TargetNum = ui->TaskTargetBox_8->currentIndex();
        task[7].TaskNum = 8;
    }
}
void QtSGS::changeTaskTargetNodeNum9()
{
    if (ui->TaskTargetBox_9->currentIndex() == 0)
    {
        task[8].TargetNum = -1;
        task[8].TaskNum = -1;
    }
    else
    {
        task[8].TargetNum = ui->TaskTargetBox_9->currentIndex();
        task[8].TaskNum = 9;
    }
}
void QtSGS::changeTaskTargetNodeNum10()
{
    if (ui->TaskTargetBox_10->currentIndex() == 0)
    {
        task[9].TargetNum = -1;
        task[9].TaskNum = -1;
    }
    else
    {
        task[9].TargetNum = ui->TaskTargetBox_10->currentIndex();
        task[9].TaskNum = 10;
    }
}
void QtSGS::clickUDPconnectButton()
{
    if (!udpconnect)
    {
        QHostAddress address;//ip
        address.setAddress(ui->textEdit_1->toPlainText());
        if (!ui->textEdit_1->toPlainText().isEmpty() && !ui->textEdit_2->toPlainText().isEmpty() && !ui->textEdit_3->toPlainText().isEmpty() && !ui->textEdit_4->toPlainText().isEmpty())
        {
            m_receiver->bind(address, ui->textEdit_2->toPlainText().toUInt());
            connect(m_receiver, SIGNAL(readyRead()), this, SLOT(readData()));
            QMessageBox::information(this, "提示", "UDP连接成功");
            ui->UDPconnectButton->setText("断开连接");
            udpconnect = true;
        }
        else {

            QMessageBox::information(this, "提示", "UDP连接失败");
        }
    }
    else 
    {
        m_receiver->abort();
        ui->UDPconnectButton->setText("建立连接");
        udpconnect = false;
        udprecv = false;
    }
}
void QtSGS::readData()
{
    QByteArray datagram;//设置data的大小为等待处理的数据报的大小，这样才能接收到完整的数据
    //datagram.resize(m_receiver->pendingDatagramSize());
    //m_receiver->readDatagram(datagram.data(), datagram.size());
    //QString str = datagram.data();//此处data()函数返回char*类型
    //ui->UDPconnectButton->setText(str);//这里用qDebug()是显示在程序输出中

    char* pdata = (char*)&data;
    m_receiver->readDatagram(pdata, sizeof(RecvInfo));
    udprecv = true;
    //ui->UDPconnectButton->setText(QString::number(data.x, 'f', 2));
    data.x = Location[car[0].CarNodeNum - 1][0] + data.x * 10;
    data.y = Location[car[0].CarNodeNum - 1][1] - data.y * 10;
  
}