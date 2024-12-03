#include "mainwindow.h"
#include "myfunction.h"
#include "axismovethread.h"
// #include "./ui_mainwindow.h"
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QListView>
#include <QListWidget>
#include <QJsonObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QStandardItemModel>
#include <QTimer>
#include <QThread>
#include <QCoreApplication>
#include <QString>

#include "gts.h"
// #include "windows.h"
// #include "stdio.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout_v = new QHBoxLayout(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVBoxLayout *midmotionLayout = new QVBoxLayout();
    QVBoxLayout *recordLayout = new QVBoxLayout();

    //!水平、垂直间隔尺寸设置
    QSpacerItem *spacer_v = new QSpacerItem(20,40,QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *spacer_h = new QSpacerItem(10,20,QSizePolicy::Expanding, QSizePolicy::Minimum);

    mainLayout_v->addLayout(mainLayout);
    mainLayout_v->addItem(spacer_v);
    mainLayout_v->addLayout(midmotionLayout);
    mainLayout_v->addItem(spacer_v);
    mainLayout_v->addLayout(recordLayout);

    // 运动控制按钮
    QGroupBox *motionControlGroupBox = new QGroupBox("运动控制",this);
    QVBoxLayout *motionControlLayout = new QVBoxLayout(motionControlGroupBox);
    initializeButton =  new QPushButton("初始化",this);
    clearPosButton =    new QPushButton("清除位置",this);
    jogMotionButton =   new QPushButton("开锤子", this);
    jogStopButton =     new QPushButton("关锤子", this);
    stopButton    =     new QPushButton("停止运动", this);
    axisOffButton =     new QPushButton("关伺服", this);
    testingButton =     new QPushButton("Y_Motion", this);

    motionControlLayout ->addWidget(initializeButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout ->addWidget(clearPosButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout ->addWidget(jogMotionButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout ->addWidget(jogStopButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout ->addWidget(stopButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout->addWidget(axisOffButton);
    motionControlLayout->addItem(spacer_h);
    motionControlLayout->addWidget(testingButton);
    mainLayout->addWidget(motionControlGroupBox);

    // 模式选择
    QGroupBox *modeSelectionGroupBox = new QGroupBox("模式选择",this);
    QVBoxLayout *modeLayout = new QVBoxLayout(modeSelectionGroupBox);
    modeComboBox = new QComboBox(this);
    modeComboBox->addItems({"S-L-S-R", "S-L-M-R", "S-R-M-L", "M-R-M-L", "S-l-S-R(Forth-Back)"});
    modeLayout->addWidget(modeComboBox);
    mainLayout->addWidget(modeSelectionGroupBox);
    mainLayout->addItem(spacer_h);
    //!**************************************************************************************
    // 列表视图显示

    QGroupBox *axisPositionGroupBox = new QGroupBox ("轴规划位置",this);
    stsPrfPosView = new QListWidget(this);
    // stsPrfPosView->addItem("X实际位置：0");
    // stsPrfPosView->addItem("Y实际位置：0");
    // stsPrfPosView->addItem("Z实际位置：0");
    axisPositionGroupBox->setLayout(new QVBoxLayout());
    axisPositionGroupBox->layout()->addWidget(stsPrfPosView);
    mainLayout->addWidget(axisPositionGroupBox);
    mainLayout->addItem(spacer_h);

    QGroupBox *axisRealPositionGroupBox = new QGroupBox("轴实际位置", this);
    stsEncPosView = new QListWidget(this);
    // stsEncPosView->addItem("X实际位置：0");
    // stsEncPosView->addItem("Y实际位置：0");
    // stsEncPosView->addItem("Z实际位置：0");
    axisRealPositionGroupBox->setLayout(new QVBoxLayout());
    axisRealPositionGroupBox->layout()->addWidget(stsEncPosView);
    mainLayout->addWidget(axisRealPositionGroupBox);

    //!**************************************************************************************
    //!
    QGroupBox *motionGroupBox = new QGroupBox("运动控制", this);
    motionListWidget = new QListWidget(this);
    // rcsModel = new QStandardItemModel(this);
    // QStandardItem* item1 = new QStandardItem("Item 1");
    // rcsModel->appendRow(item1);
    // motionListView->QAbstractItemView::setModel(rcsModel);

    QVBoxLayout *motionLayout = new QVBoxLayout(motionGroupBox);
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    QHBoxLayout *hlayout3 = new QHBoxLayout();
    QHBoxLayout *hlayout4 = new QHBoxLayout();
    QHBoxLayout *hlayout5 = new QHBoxLayout();
    QHBoxLayout *hlayout6 = new QHBoxLayout();
    QHBoxLayout *hlayout7 = new QHBoxLayout();
    QHBoxLayout *hlayout8 = new QHBoxLayout();

    QLabel *labspeed    = new QLabel("打印速度：",motionGroupBox);
    QLabel *labheight_0   = new QLabel("初始层高：",motionGroupBox);
    QLabel *lablength   = new QLabel("打印道长：",motionGroupBox);
    QLabel *labpower    = new QLabel("激光功率：",motionGroupBox);
    QLabel *labnumber_way    = new QLabel("打印道数：",motionGroupBox);
    QLabel *labnumber_layer    = new QLabel("打印层数：",motionGroupBox);
    QLabel *labheight_per    = new QLabel("打印层高：",motionGroupBox);
    QLabel *laboffset    = new QLabel("道偏移量：",motionGroupBox);

    speedInput   = new QLineEdit("请输入速度",motionGroupBox);
    height_0Input  = new QLineEdit("请输入初高",motionGroupBox);
    lengthInput  = new QLineEdit("请输入道长",motionGroupBox);
    powerInput   = new QLineEdit("请输入功率",motionGroupBox);
    number_way_Input   = new QLineEdit("请输入打印道数",motionGroupBox);
    number_layerInput   = new QLineEdit("请输入打印层数",motionGroupBox);
    height_perInput   = new QLineEdit("请输入打印层高",motionGroupBox);
    offsetInput   = new QLineEdit("请输入偏移量",motionGroupBox);

    printButton = new QPushButton("开始打印",this);

    hlayout1->addWidget(labspeed);
    hlayout1->addWidget(speedInput);
    hlayout2->addWidget(labheight_0);
    hlayout2->addWidget(height_0Input);
    hlayout3->addWidget(lablength);
    hlayout3->addWidget(lengthInput);
    hlayout4->addWidget(labpower);
    hlayout4->addWidget(powerInput);
    hlayout5->addWidget(labnumber_way);
    hlayout5->addWidget(number_way_Input);
    hlayout6->addWidget(labnumber_layer);
    hlayout6->addWidget(number_layerInput);
    hlayout7->addWidget(labheight_per);
    hlayout7->addWidget(height_perInput);
    hlayout8->addWidget(laboffset);
    hlayout8->addWidget(offsetInput);

    motionLayout->addLayout(hlayout1);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout2);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout3);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout4);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout5);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout6);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout7);
    // motionLayout->addItem(spacer_h);
    motionLayout->addLayout(hlayout8);
    motionLayout->addWidget(printButton);

    motionLayout      ->addWidget(motionListWidget);
    midmotionLayout   ->addWidget(motionGroupBox);

    QGroupBox *eptRunGroupBox = new QGroupBox("空载运动/mm", this);
    QVBoxLayout *eptRunLayout = new QVBoxLayout(eptRunGroupBox);
    QLabel *eptRun_X    = new QLabel("X方向移动：", eptRunGroupBox);
    QLabel *eptRun_Y    = new QLabel("Y方向移动：", eptRunGroupBox);
    QLabel *eptRun_Z    = new QLabel("Z方向移动：", eptRunGroupBox);
    QLabel *eptRun_S    = new QLabel("空载速度 ：", eptRunGroupBox);
    inputEptRun_X   = new QLineEdit("请输入移动距离", eptRunGroupBox);
    inputEptRun_Y   = new QLineEdit("请输入移动距离", eptRunGroupBox);
    inputEptRun_Z   = new QLineEdit("请输入移动距离", eptRunGroupBox);
    inputEptRun_S   = new QLineEdit("请输入移动速度", eptRunGroupBox);
    QPushButton *eptRunButton  = new QPushButton("开始空载运动",this);
    QHBoxLayout *eptRunLayout1 = new QHBoxLayout();
    QHBoxLayout *eptRunLayout2 = new QHBoxLayout();
    QHBoxLayout *eptRunLayout3 = new QHBoxLayout();
    QHBoxLayout *eptRunLayout4 = new QHBoxLayout();
    eptRunLayout1->addWidget(eptRun_X);
    eptRunLayout1->addWidget(inputEptRun_X);
    eptRunLayout2->addWidget(eptRun_Y);
    eptRunLayout2->addWidget(inputEptRun_Y);
    eptRunLayout3->addWidget(eptRun_Z);
    eptRunLayout3->addWidget(inputEptRun_Z);
    eptRunLayout4->addWidget(eptRun_S);
    eptRunLayout4->addWidget(inputEptRun_S);
    eptRunLayout->addLayout(eptRunLayout1);
    eptRunLayout->addLayout(eptRunLayout2);
    eptRunLayout->addLayout(eptRunLayout3);
    eptRunLayout->addLayout(eptRunLayout4);
    eptRunLayout->addWidget(eptRunButton);

    midmotionLayout->addWidget(eptRunGroupBox);

    //路径记录视图
    recordGroupBox = new QGroupBox("路径记录",this);
    recordGroupBox->setAlignment(Qt::AlignHCenter);
    QVBoxLayout *innerrecordLayout = new QVBoxLayout(recordGroupBox);
    recordListWidget = new QListWidget(this);

    innerrecordLayout->addWidget(recordListWidget);
    recordLayout->addWidget(recordGroupBox);
    //! 初始化坐标系的基本运动参数**************************************************************************************
    //! 运动坐标在setcrd（）函数中再次初始化，因此不在此处初始化

    rtn = GTN_SetCrdMapBase(1, 1, 0);
    rtn = GTN_GetCrdPrm(1, 1, &crdPrm);
    crdPrm.dimension = 3; // 坐标系为三维坐标系
    crdPrm.synVelMax = 1500; // 最大合成速度：1500pulse/ms
    crdPrm.synAccMax = 50; // 最大加速度：1pulse/ms^2
    crdPrm.evenTime = 50; // 最小匀速时间：50ms
    crdPrm.profile[0] = 3; // 规划器0对应到Z轴
    crdPrm.profile[1] = 2; // 规划器1对应到Y轴
    crdPrm.profile[2] = 0;
    crdPrm.profile[3] = 1; // 规划器3对应到X轴
    crdPrm.profile[4] = 0;

    //!**************************************************************************************
    // 连接槽函数
    MyFunction *m_myfunction = new MyFunction(this);

    connect(clearPosButton, &QPushButton::clicked, this, &MainWindow::onClearPositionClicked);
    connect(jogMotionButton, &QPushButton::clicked, m_myfunction, &MyFunction::onJogMotionClicked);
    connect(jogStopButton, &QPushButton::clicked, m_myfunction, &MyFunction::onJogStopClicked);
    connect(stopButton, &QPushButton::clicked, m_myfunction, &MyFunction::onStopClicked);
    connect(axisOffButton, &QPushButton::clicked, m_myfunction, &MyFunction::onAxisOffClicked);
    connect(testingButton, &QPushButton::clicked, m_myfunction, &MyFunction::onTestingClicked);

    // connect(eptRunButton, &QPushButton::clicked,this , &MainWindow::onEptRunClicked);

    //槽函数模式转换
    // Connect to ComboBox change event  很重要
    //! 使用了 QOverload<int>::of()
    //! 来明确表示我们要连接的是带有一个整数参数的版本（即 currentIndexChanged(int)）
    connect(modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onModeChanged);

    emit modeComboBox->currentIndexChanged(modeComboBox->currentIndex());

    connect(printButton, &QPushButton::clicked, m_myfunction, &MyFunction::onPrintClicked);

    connect(m_myfunction,&MyFunction::printCompleted, this, [this]{
        recordListWidget->addItem("打印完成");
    });

//线程池逻辑！**************************************************************************************
//!创建任务类对象

    AxisPosUpdate* axisposupdate = new AxisPosUpdate;
    QThread*       axisThread    = new QThread;

    axisposupdate->moveToThread(axisThread);
    connect(axisThread, &QThread::started, axisposupdate, &AxisPosUpdate::startUpdates);
    connect(axisThread, &QThread::finished, axisposupdate, &AxisPosUpdate::deleteLater);
    connect(axisThread, &QThread::finished, axisThread, &QThread::deleteLater);

    connect(qApp, &QCoreApplication::aboutToQuit, this,[=] {
        axisThread->quit();
        axisThread->wait();
    });

    connect(axisposupdate, &AxisPosUpdate::PosClear, this,[this]{
        stsEncPosView->clear();
        stsPrfPosView->clear();
    },Qt::QueuedConnection);

    connect(axisposupdate, &AxisPosUpdate::PosUpdated, this, [this](QVariantList prfData, QVariantList encData){
    //在更新 UI 时，使用 QMetaObject::invokeMethod() 来确保 UI 更新操作在主线程中执行。这样可以避免跨线程访问 UI 引发的问题。
        QMetaObject::invokeMethod(this, [this, prfData, encData] {
            for(int i : {3,1,0,2,4}){
                stsEncPosView->addItem(encData[i].toString());
                stsPrfPosView->addItem(prfData[i].toString());
                }
        },Qt::QueuedConnection);
    },Qt::QueuedConnection);

    connect(eptRunButton, &QPushButton::clicked, this, [=]{
        // 获取 QLineEdit 中的文本并转换为相应的数值
        AxisMoveThread* axismovethread = new AxisMoveThread;
        bool xOk = 0;
        bool yOk = 0;
        bool zOk = 0;
        bool sOk = 0;
        long x   = static_cast<long>(inputEptRun_X->text().toDouble(&xOk) * 10000);
        long y   = static_cast<long>(inputEptRun_Y->text().toDouble(&yOk) * 10000);
        long z   = static_cast<long>(inputEptRun_Z->text().toDouble(&zOk) * 10000);
        double s = inputEptRun_S->text().toDouble(&sOk) * 10;
        axismovethread->setEptRunPrams(x, y, z, s);

        connect(axismovethread ,&AxisMoveThread::MoveCompleted, this,[this/*,axismovethread*/](long x, long y, long z, double s ){
                // commandhandler("空载运动：" , axismovethread->EptRunrtn , motionListWidget);
                recordListWidget->addItem(QString("x运动：%1 mm").arg(x / 10000.0));
                recordListWidget->addItem(QString("y运动：%1 mm").arg(y / 10000.0));
                recordListWidget->addItem(QString("z运动：%1 mm").arg(z / 10000.0));
                recordListWidget->addItem(QString("当前运动次数：%1").arg(this->EptRunCount.load()));
                recordListWidget->scrollToBottom();
                this->EptRunCount.fetch_add(1, std::memory_order_relaxed);
        },Qt::QueuedConnection);

        if(xOk &&yOk && zOk&& sOk){
            setCrd();
            QThreadPool::globalInstance() -> start(axismovethread);
            QMessageBox::information(this, "开始空载", "空载动作完成");
        } else {
            QMessageBox::information(this, "空载失败", "请检查输入参数");
        }
    });

    connect(initializeButton, &QPushButton::clicked, this, [=]{
        Initialize*    initialize    = new Initialize;
        connect(initialize ,&Initialize::commandSend, this,[this](const char* command, short Initialrtn)
        {
        commandhandler(command , Initialrtn , motionListWidget);},Qt::QueuedConnection);
        if(!axisThread->isRunning()){
            // axisThread->quit();
            // axisThread->wait();
            axisThread->start();
        };

        QThreadPool::globalInstance()->start(initialize);
        QMessageBox::information(this, "Initialization", "初始化完成");
    });

//线程池逻辑！**************************************************************************************

}

MainWindow::~MainWindow()
{
    // delete ui;
    rtn = GTN_Close();
    // commandhandler("GTN_Close", rtn, motionListWidget);
}

void MainWindow::commandhandler(const char *command, short error , QListWidget *ListWidget)
{
    // 如果指令执行返回值为非0，说明指令执行错误，向屏幕输出错误结果
    if(error)
    {
        printf("%s = %d\n", command, error);
        QListWidgetItem *item1 = new QListWidgetItem(command);
        item1->setText(QString("当前出错的指令为%1:%2").arg(command).arg(error));
        ListWidget->addItem(item1);
        ListWidget->scrollToBottom();
    }
}

void MainWindow::onModeChanged(int index) {
    // Handle mode changes and update the view
    // You can switch between different inputs for each mode here

    currentIndex = index;
    switch (index) {
    case 0: // "S-L-S-R"
        // speedInput 和 heightInput 可编辑，其他不可编辑
        speedInput->setEnabled(true);
        height_0Input->setEnabled(true);
        lengthInput->setEnabled(true);
        powerInput->setEnabled(true);
        number_way_Input->setEnabled(false);
        number_layerInput->setEnabled(false);
        height_perInput->setEnabled(false);
        offsetInput->setEnabled(false);
        break;
    case 1: // "S-L-M-R"
        speedInput->setEnabled(true);
        height_0Input->setEnabled(true);
        lengthInput->setEnabled(true);
        powerInput->setEnabled(true);
        number_way_Input->setEnabled(true);
        number_layerInput->setEnabled(false);
        height_perInput->setEnabled(false);
        offsetInput->setEnabled(true);
        break;
    case 2: // "M-L-S-R"
        speedInput->setEnabled(true);
        height_0Input->setEnabled(true);
        lengthInput->setEnabled(true);
        powerInput->setEnabled(true);
        number_way_Input->setEnabled(true);
        number_layerInput->setEnabled(true);
        height_perInput->setEnabled(true);
        offsetInput->setEnabled(false);
        break;
    case 3: // "M-L-M-R"
        speedInput->setEnabled(true);
        height_0Input->setEnabled(true);
        lengthInput->setEnabled(true);
        powerInput->setEnabled(true);
        number_way_Input->setEnabled(true);
        number_layerInput->setEnabled(true);
        height_perInput->setEnabled(true);
        offsetInput->setEnabled(true);
        break;
    case 4: // "S-L-S-R 来回走"
        // speedInput 和 heightInput 可编辑，其他不可编辑
        speedInput->setEnabled(true);
        height_0Input->setEnabled(true);
        lengthInput->setEnabled(true);
        powerInput->setEnabled(true);
        number_way_Input->setEnabled(false);
        number_layerInput->setEnabled(false);
        height_perInput->setEnabled(false);
        offsetInput->setEnabled(false);
    }
}

void MainWindow::onClearPositionClicked() {
    // Handle clear position logic
    rtn = GTN_ZeroPos(1, 1, 8);
    commandhandler("GTN_ZeroPos", rtn, motionListWidget);
    QMessageBox::information(this, "Clear Position", "清空位置完成");
}

void MainWindow::setCrd()
{
    //0 ：不需要指定原点坐标值，则坐标系的原点在当前规划位置上。 1 ：需要指定原点坐标值，坐标系的原点在 originPos 指定的规划位置上。
    crdPrm.setOriginFlag = 0; // 表示需要指定坐标系的原点坐标的规划位置       指定原点坐标的规划位置,    是不是刚开机时候的原点，还是当前位置为原点(可能已经运动过一段距离，不在开机的原点)

    crdPrm.originPos[0] = 0; // 坐标系的原点坐标的规划位置(0,0,0)             原点坐标的规划位置为(0, 0, 0)
    crdPrm.originPos[1] = 0;
    crdPrm.originPos[2] = 0;
    crdPrm.originPos[3] = 0;
    crdPrm.originPos[4] = 0;
    // 建立1号坐标系，设置坐标系参数
    rtn = GTN_SetCrdPrm(1, 1, &crdPrm);
    commandhandler("GTN_SetCrdPrm", rtn, motionListWidget);
}

