#ifndef MYCM_H
#define MYCM_H

#include "mypoint.h"
#include <QDebug>

class MyCM
{
public:
    MyCM();
    MyCM(MyPoint *p1, MyPoint *p2);
    void setP1(MyPoint *p);
    void setP2(MyPoint *p);
    MyPoint *getP1();
    MyPoint *getP2();
    static mpi stringToMPI(QString text);
    QString mpiToString(mpi number);
    QString toString();


private:
    MyPoint *p1, *p2;
};

#endif // MYCM_H
