#include "myfunction.h"
#include "mainwindow.h"
#include "axismovethread.h"
#include "gts.h"

#include <QDebug>
#include <QMessageBox>


MyFunction::MyFunction(MainWindow * mainWindow)
    : m_mainWindow(mainWindow){

}

MyFunction::~MyFunction() {}

void MyFunction::onJogMotionClicked() {
    // Handle jog motion logic
    rtn = GTN_AxisOn(1, 5);
    m_mainWindow ->commandhandler("GTN_AxisOn", rtn, m_mainWindow ->motionListWidget);

    TJogPrm jog;
    rtn = GTN_PrfJog(1, AXIS5);
    m_mainWindow ->commandhandler("GTN_PrfJog", rtn, m_mainWindow ->motionListWidget);
    rtn = GTN_GetJogPrm(1, AXIS5, &jog);
    m_mainWindow ->commandhandler("GTN_GetJogPrm", rtn, m_mainWindow ->motionListWidget);
    jog.acc = 20;
    jog.dec = 10;
    rtn = GTN_SetJogPrm(1, AXIS5, &jog);
    m_mainWindow ->commandhandler("GTN_SetJogPrm", rtn, m_mainWindow ->motionListWidget);

    rtn = GTN_SetVel(1, AXIS5, 250);
    m_mainWindow ->commandhandler("GTN_SetVel", rtn, m_mainWindow ->motionListWidget);

    rtn = GTN_Update(1, 1 << (AXIS5 - 1));
    m_mainWindow ->commandhandler("GTN_Update", rtn, m_mainWindow ->motionListWidget);
    // QMessageBox::information(this, "Jog Motion", "开锤子动作完成");
}

void MyFunction::onJogStopClicked() {
    // Handle jog stop logic
    rtn = GTN_Stop(1, 0x10, 0);         //锤子平滑停止
    m_mainWindow ->commandhandler("GTN_Stop", rtn, m_mainWindow ->motionListWidget);

    rtn = GTN_AxisOff(1, 5);
    m_mainWindow ->commandhandler("GTN_AxisOff", rtn, m_mainWindow ->motionListWidget);
    QMessageBox::information(m_mainWindow, "Jog Stop", "关锤子动作完成");
}

void MyFunction::onStopClicked() {
    // Handle stop logic
    rtn = GTN_Stop(1, 0x1f, 0);         //五个轴全部平滑停止
    m_mainWindow ->commandhandler("GTN_Stop", rtn, m_mainWindow ->motionListWidget);
    QMessageBox::information(m_mainWindow, "Stop", "已停止运动");
}

void MyFunction::onAxisOffClicked() {
    // Handle axis off logic
    for (int i = 1; i <= 5; i++)
    {
        rtn = GTN_AxisOff(1, i);
    }
    m_mainWindow ->commandhandler("GTN_AxisOff", rtn, m_mainWindow ->motionListWidget);
    QMessageBox::information(m_mainWindow, "Axis Off", "关伺服完成");
}

void MyFunction::onTestingClicked() {
    // Handle testing logic
    QMessageBox::information(m_mainWindow, "Testing", "Y_Motion 测试");
}

void MyFunction::onPrintClicked(){
    bool sOk = 0, h0Ok = 0, lOk =0, pOk = 0, num_wOk = 0, num_lOk = 0, hpOk = 0, osOk = 0;
    double speed   = m_mainWindow->speedInput->text().toDouble(&sOk)  *10  ;
    long h0   =static_cast<long>( m_mainWindow->height_0Input->text().toDouble(&h0Ok) * 10000);
    long l   = static_cast<long> (m_mainWindow->lengthInput->text().toDouble(&lOk) * 10000);
    short p   = static_cast<short>( (m_mainWindow->powerInput->text().toDouble(&pOk) / 300) * 3276.7 );
    int num_w   = m_mainWindow->number_way_Input->text().toInt(&num_wOk);
    int num_l   = m_mainWindow->number_layerInput->text().toInt(&num_lOk);
    long hp   = static_cast<long> (m_mainWindow->height_perInput->text().toDouble(&hpOk) * 10000);
    long offset   = static_cast<long> (m_mainWindow->offsetInput->text().toDouble(&osOk) * 10000);

    switch (m_mainWindow->currentIndex) {
    case 0:
        // S-L-S-W
        this->printSLSW(speed, l ,h0 , p);
        break;
    case 1:
        // S-L-M-W
        this->printSLMW(speed, l ,h0 , p, num_w , offset);
        break;
    case 2:
        // M-L-S-W
        this->printMLSW(speed, l ,h0 , p , num_l , hp);
        break;
    case 3:
        // M-L-M-W
        this->printMLMW(speed, l ,h0 , p,num_l , num_w , offset , hp);
        break;
    default:
        break;
    }
    qDebug()<<"print finished";
    QMessageBox::information(m_mainWindow, "工艺试验", "打印完成");
}

void MyFunction::printSLSW(double speed, long length, long h0, short p){
    // S-L-S-W
    m_mainWindow->setCrd();

    rtn = GTN_CrdClear(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdClear", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 0, 0, h0, speed, 20, 0, 0);         //设置h0
    m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 60000, 0, h0, speed, 20, speed, 0);  //减少加速阶段带来的影响
    m_mainWindow->commandhandler("GTN_LnXY", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDA(1, 1, 1, p, 0);                   //开激光
    m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, length + 60000, 0, h0, speed, 20, speed, 0);
    m_mainWindow->commandhandler("GTN_LnXY", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDA(1, 1, 1, 0, 0);                       //关激光
    m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, length + 60000 + 100000, 0, h0, speed, 20, 0, 0);    //减少减速阶段带来的影响
    m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 0, 0, h0, speed, 20, 0, 0);

    m_mainWindow->commandhandler("GTN_LnXY", rtn, m_mainWindow-> motionListWidget);

    m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    //启动插补运动
    rtn = GTN_CrdStart(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdStart", rtn, m_mainWindow-> motionListWidget);
    emit MyFunction::printCompleted();

}

void MyFunction::printSLMW(double speed, long length, long h0, short p, int way_number, long offset){
    m_mainWindow->setCrd();

    rtn = GTN_CrdClear(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdClear", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 0, 0, h0, speed, 20, 0, 0);         //设置h0
    m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 0, 0);  //缓存区指定
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);

    for (int i = 0; i < way_number; i++)
    {
        if((i + 1) % 2 == 1)
        {//打印奇数层时,也就是offset方向为Y轴正方向 向下走
            rtn = GTN_LnXYZ(1, 1, 0, offset * i, h0, speed, 20, 0, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, 100000, offset * i, h0, speed, 20, speed, 0);  //优化加速影响
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, p, 0);                   //开激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, length + 100000, offset * i, h0, speed, 20, speed, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, 0, 0);                   //关激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, length + 100000 + 100000, offset * i, h0, speed, 20, 0, 0);  //优化减速影响
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            ////若达到终点，则不走偏移量了 停留在该道原点
            if (i == way_number - 1)
            {
                break;
            }

            rtn = GTN_LnXYZ(1, 1, length + 100000 + 100000, offset * (i + 1), h0, speed, 20, 0, 0); //走一个偏移量（正向下，负向上），到达下一道偶数道的起点位置
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);
        }
        else {
            rtn = GTN_LnXYZ(1, 1, length + 100000 + 100000, offset * i, h0, speed, 20, 0, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, length + 100000, offset * i, h0, speed, 20, speed, 0);  //优化加速影响
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, p, 0);                   //开激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, 100000, offset * i, h0, speed, 20, speed, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, 0, 0);                   //关激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, 0, offset * i, h0, speed, 20, 0, 0);  //优化减速影响
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);
            if (i == way_number - 1) break; //若达到终点，则不走偏移量了 停留在该道原点

            rtn = GTN_LnXYZ(1, 1, 0, offset * (i + 1), h0, speed, 20, 0, 0); //走一个偏移量（正向下，负向上），到达下一道偶数道的起点位置
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);
        }
    }

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 1, 0);
    //启动插补运动
    rtn = GTN_CrdStart(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdStart", rtn, m_mainWindow-> motionListWidget);
    emit MyFunction::printCompleted();

};

void MyFunction::printMLSW(double speed, long length, long h0, short p, int layer_number, long h){

    short p_array[5] ={
        static_cast<short>(round((2000 / 300.0) * 3276.7)),
        static_cast<short>(round((2000 / 300.0) * 3276.7)),
        static_cast<short>(round((2000 / 300.0) * 3276.7)),
        static_cast<short>(round((2000 / 300.0) * 3276.7)),
        static_cast<short>(round((2000 / 300.0) * 3276.7))
    };

    m_mainWindow->setCrd();

    rtn = GTN_CrdClear(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdClear", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 0, 0, h0, speed, 20, 0, 0);         //设置h0
    m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);

    for (int i = 0; i < layer_number; i++)
    {
        rtn = GTN_LnXYZ(1, 1, 0, 0, h0 + h * i, speed, 20, 0, 0);
        m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

        rtn = GTN_LnXYZ(1, 1, 60000, 0, h0 + h * i, speed, 20, speed, 0);             //优化加速影响
        m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

        rtn = GTN_BufDA(1, 1, 1, p_array[i], 0);                   //开激光
        m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

        rtn = GTN_LnXYZ(1, 1, length + 60000, 0, h0 + h * i, speed, 20, speed, 0);
        m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

        rtn = GTN_BufDA(1, 1, 1, 0, 0);                   //关激光
        m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

        rtn = GTN_LnXYZ(1, 1, length + 60000 + 160000, 0, h0 + h * i, speed, 20, 0, 0);   //优化减速影响
        m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);
    }

    //启动插补运动
    rtn = GTN_CrdStart(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdStart", rtn, m_mainWindow-> motionListWidget);
    emit MyFunction::printCompleted();

};

void MyFunction::printMLMW(double speed, long length, long h0, short p, int layer_number, int way_number, long offset, long h){
    m_mainWindow->setCrd();

    rtn = GTN_CrdClear(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdClear", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_LnXYZ(1, 1, 0, 0, h0, speed, 20, 0, 0);         //设置h0
    m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 0, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);

    for (int i = 0; i < layer_number; i++)
    {
        for (int j = 0; j < way_number; j++)
        {
            rtn = GTN_LnXYZ(1, 1, 0, offset*j, h0 + h*i , speed, 20, speed, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, 60000, offset * j, h0 + h * i, speed, 20, speed, 0);        //优化加速影响
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, p, 0);                   //开激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);
            rtn = GTN_LnXYZ(1, 1, length+60000, offset * j, h0 + h * i, speed, 20, speed, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_BufDA(1, 1, 1, 0, 0);                   //关激光
            m_mainWindow->commandhandler("GTN_BufDA", rtn, m_mainWindow-> motionListWidget);

            rtn = GTN_LnXYZ(1, 1, length + 60000 + 100000, offset * j, h0 + h * i, speed, 20, 0, 0);
            m_mainWindow->commandhandler("GTN_LnXYZ", rtn, m_mainWindow-> motionListWidget);
        }
    }

    rtn = GTN_BufDoBit(1, 1, MC_GPO, 2, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 4, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);
    rtn = GTN_BufDoBit(1, 1, MC_GPO, 5, 1, 0);
    m_mainWindow->commandhandler("GTN_BufDoBit", rtn, m_mainWindow-> motionListWidget);

    //启动插补运动
    rtn = GTN_CrdStart(1, 1, 0);
    m_mainWindow->commandhandler("GTN_CrdStart", rtn, m_mainWindow-> motionListWidget);
    emit MyFunction::printCompleted();
};


// void MyFunction::recordUpdate(){
//     m_mainWindow->recordListWidget->addItem(QString::fromStdString(enc_result[i]));

// };
