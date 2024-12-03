#include "axismovethread.h"
#include "gts.h"
#include "mainwindow.h"
#include "myfunction.h"
#include "QString"
#include "QThread"

//*******************************************************************************************************
AxisMoveThread::AxisMoveThread(QObject *parent) :  QObject (parent),  QRunnable()
{
    setAutoDelete(true);
}

void AxisMoveThread::run()
{
    EptRunrtn = GTN_CrdClear(1, 1, 0);

    EptRunrtn = GTN_LnXYZ(1, 1, m_x, m_y, m_z, m_s, 20, 0, 0);

    EptRunrtn = GTN_CrdStart(1, 1, 0);
    emit MoveCompleted(m_x, m_y , m_z , m_s);

}

void AxisMoveThread::setEptRunPrams(long x, long y, long z, double s)
{
    m_x = x;
    m_y = y;
    m_z = z;
    m_s = s;
}

//*******************************************************************************************************
AxisPosUpdate::AxisPosUpdate(QObject *parent) :  QObject (parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AxisPosUpdate::updateAxisPosition);
}

void AxisPosUpdate::startUpdates()
{
    timer->start(1000);
}

void AxisPosUpdate::stopUpdates()
{
    timer->stop();
}

void AxisPosUpdate::updateAxisPosition() {
    emit PosClear();
    PrfdataList.clear();
    EncdataList.clear();
    stsPrfPosValue = getPrfPos();
    stsEncPosValue = getEncPos();

    for (int i = 0; i < 5; i++)
    {
        double_prfValue[i] = (stsPrfPosValue[i]) / 10000;
        PrfdataList.append(QVariant(QString::number(double_prfValue[i],'f',2)));

        double_encValue[i] = (stsEncPosValue[i]) / 10000;
        EncdataList.append(QVariant(QString::number(double_encValue[i], 'f', 2)));
    }

    emit AxisPosUpdate::PosUpdated(PrfdataList, EncdataList);
}

long* AxisPosUpdate::getPrfSts(){
    rtn = GTN_GetPrfSts(1, 1, allsts, 5);
    // m_mainWindow->commandhandler("GTN_GetPrfSts", rtn, m_mainWindow-> motionListWidget);
    return allsts;
};

long* AxisPosUpdate::GetPrfMode(){
    rtn = GTN_GetPrfMode(1, 1, allmode, 5);
    // m_mainWindow->commandhandler("GTN_GetPrfMode", rtn, m_mainWindow-> motionListWidget);
    return allmode;
};

double * AxisPosUpdate::getPrfPos(){
    rtn = GTN_GetPrfPos(1, 1, allprfpos, 5);
    // m_mainWindow->commandhandler("GTN_GetPrfPos", rtn, m_mainWindow-> motionListWidget);
    return allprfpos;
};
double * AxisPosUpdate::getEncPos(){
    rtn = GTN_GetEncPos(1, 1, allencpos, 5);
    // m_mainWindow->commandhandler("GTN_GetEncPos", rtn, m_mainWindow-> motionListWidget);
    return allencpos;
};

//*******************************************************************************************************
Initialize::Initialize(QObject *parent) :  QObject (parent),  QRunnable()
{
    setAutoDelete(true);
}

void Initialize::run()
{
    if (isRunning) return;
    isRunning = true;
    Initalrtn = GTN_Open(5, 2);
    emit commandSend(const_cast<char*>("GTN_Open"), Initalrtn);

    Initalrtn = GTN_Reset(1);
    emit commandSend(const_cast<char*>("GTN_Reset"), Initalrtn);

    Initalrtn = GTN_LoadConfig( 1 ,  const_cast<char*>("core1_1.cfg"));
        emit commandSend(const_cast<char*>("GTN_LoadConfig"), Initalrtn);

    for (int i = 1; i <= 5; i++)
    {
        Initalrtn = GTN_AlarmOff(1, i);
        emit commandSend(const_cast<char*>("GTN_AlarmOff"), Initalrtn);
    }

    Initalrtn = GTN_ClrSts(1, 1, 8);
        emit commandSend(const_cast<char*>("GTN_ClrSts"), Initalrtn);

    for (int i = 1; i <= 5; i++)
    {
        Initalrtn = GTN_ZeroPos(1, i);
        emit commandSend(const_cast<char*>("GTN_ZeroPos"), Initalrtn);
    }

    for (int i = 1; i <= 5; i++)
    {
        Initalrtn = GTN_AxisOn(1, i);
        emit commandSend(const_cast<char*>("GTN_AxisOn"), Initalrtn);
    }

    short core = 1;
    short group = 1;
    short ctlMode = 1;
    short gantryMode;
    long synErrLmt = 500;

    Initalrtn = GTN_CtrlMode(core, AXIS2, ctlMode);
    emit commandSend(const_cast<char*>("GTN_CtrlMode"), Initalrtn);
    Initalrtn = GTN_CtrlMode(core, AXIS3, ctlMode);
    emit commandSend(const_cast<char*>("GTN_CtrlMode"), Initalrtn);

    gantryMode = GANTRY_MODE_OPEN_LOOP_GANTRY;
    Initalrtn = GTN_SetGantryMode(core, group, AXIS2, AXIS3, gantryMode, synErrLmt);
    emit commandSend(const_cast<char*>("GTN_SetGantryMode"), Initalrtn);
    isRunning = false;
}
//*******************************************************************************************************
