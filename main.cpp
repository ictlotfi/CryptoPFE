#include "mainwindow.h"
#include "bigwindow.h"
#include <QApplication>
#include "dialogplot.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BigWindow w;
    //DialogPlot d;
    //d.show();
    w.show();


    return a.exec();
}
