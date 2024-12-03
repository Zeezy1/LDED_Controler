#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <QObject>

#include "mainwindow.h"

// #include "gts.h"

class MyFunction : public QObject
{
    Q_OBJECT
public:
    explicit MyFunction(MainWindow *mainwindow);
    ~MyFunction();

    //!在运行时通过信号与槽机制连接信号和槽，槽函数可能在不同情况下有所不同
    void onPrintClicked();
    void printSLSW(double speed, long length, long h0, short p);
    void printSLMW(double speed, long length, long h0, short p, int way_number, long offset);
    void printMLSW(double speed, long length, long h0, short p, int layer_number, long h);
    void printMLMW(double speed, long length, long h0, short p, int layer_number, int way_number, long offset, long h);

    MainWindow *m_mainWindow;

    void recordUpdate();

    short rtn;

    void onJogMotionClicked();
    void onJogStopClicked();
    void onStopClicked();
    void onAxisOffClicked();
    void onTestingClicked();
signals:
    void EncPosUpdated();
    void printCompleted();
};

#endif // MYFUNCTION_H
