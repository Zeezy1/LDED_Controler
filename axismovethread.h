#ifndef AXISMOVETHREAD_H
#define AXISMOVETHREAD_H

#include <QObject>
#include <QThread>
#include <QThreadPool>
#include <sstream>
#include <QTimer>
#include <QVariant>
// #include <iomanip>  // 包含 setprecision 的头文件


class AxisMoveThread : public QObject , public QRunnable
{
        Q_OBJECT
    public:
        // AxisMoveThread();
        explicit AxisMoveThread(QObject *parent = nullptr);
        void run() override;
        short EptRunrtn;
        void setEptRunPrams(long x, long y, long z, double s);

    private:
        long m_x;
        long m_y;
        long m_z;
        double m_s;

    signals:
        void MoveCompleted(long ,long ,long ,double);
};

//****************************************************************
class AxisPosUpdate  :public QObject
{
        Q_OBJECT
    public:
        explicit AxisPosUpdate(QObject *parent = nullptr);
        void startUpdates(); // 启动定时器的接口
        void stopUpdates();  // 停止定时器的接口
        void updateAxisPosition();

        // short rtn;
        long *stsModeValue;
        long* stsPrfStsValue;
        double* stsPrfPosValue;
        double* stsEncPosValue;

        QVariantList PrfdataList;
        QVariantList EncdataList;
        double double_prfValue[5];
        std::stringstream prf_stream[5];
        std::string prf_result[5];

        double double_encValue[5];
        std::stringstream enc_stream[5];
        std::string enc_result[5];

        // QVariantList PrfdataList;
        // QVariantList EncdataList;

        short rtn;
        long allmode[5];
        long allsts[5];
        double allprfpos[5];
        double allencpos[5];

        long*   getPrfSts();
        long*   GetPrfMode();
        double* getPrfPos();
        double* getEncPos();
        QTimer* timer;

    signals:
        void PosClear();
        void PosUpdated(QVariantList prfData, QVariantList encData);
    private:

};

//****************************************************************
class Initialize : public QObject , public QRunnable
{
        Q_OBJECT
    public:
        explicit Initialize (QObject *parent = nullptr);
        void run() override;
        short Initalrtn;
    signals:
        void commandSend(const char* command,short Initalrtn);
    private:
        bool isRunning;

};



#endif // AXISMOVETHREAD_H
