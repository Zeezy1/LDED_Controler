#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QListView>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QLineEdit>
#include <QStandardItemModel>
#include "gts.h"


// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

#define		AXIS1	1
#define		AXIS2	2
#define		AXIS3	3
#define		AXIS4	4
#define		AXIS5	5


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QLineEdit *speedInput;              // 速度输入框
    QLineEdit *height_0Input;           // 初高输入框
    QLineEdit *lengthInput;             // 道长输入框
    QLineEdit *powerInput;              // 功率输入框
    QLineEdit *number_way_Input;
    QLineEdit *number_layerInput;
    QLineEdit *height_perInput;
    QLineEdit *offsetInput;

    QGroupBox *motionGroupBox;          // 运动控制组框
    QGroupBox *recordGroupBox;          // 轨迹记录组框
    QListWidget *motionListWidget;
    QListWidget *recordListWidget;

    QListWidget *stsPrfPosView;
    QListWidget *stsEncPosView;

    QLineEdit *inputEptRun_X;
    QLineEdit *inputEptRun_Y;
    QLineEdit *inputEptRun_Z;
    QLineEdit *inputEptRun_S;
    short rtn;
    // Function *m_function;

    // private:
    //     Ui::MainWindow *ui;

public:
    void commandhandler(const char *command, short error , QListWidget *ListWidget);
    void setCrd();
    QStandardItemModel *rcsModel;

    QPushButton *printButton;

    int currentIndex;
    TTrapPrm trap;   //定义了点位运动参数
    TCrdPrm crdPrm;  // TCrdPrm结构体变量，该结构体定义了坐标系

private slots:
    void onClearPositionClicked();
    void onModeChanged(int index);

private:
    QPushButton *initializeButton;
    QPushButton *clearPosButton;
    QPushButton *jogMotionButton;
    QPushButton *jogStopButton;
    QPushButton *stopButton;
    QPushButton *axisOffButton;
    QPushButton *testingButton;
    QPushButton *eptRunButton;
    QComboBox *modeComboBox;

    QPushButton *ConfirmButton;
    QPushButton *RunButton;
    std::atomic<int> EptRunCount{1};

};


#endif // MAINWINDOW_H
