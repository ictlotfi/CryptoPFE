#include "mainwindow.h"
#include "bigwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BigWindow w;
    w.show();


    return a.exec();
}
