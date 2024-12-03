#include "mainwindow.h"
// #include "gts.h"
// #include "windows.h"
// #include "stdio.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


}

