#ifndef MYPOINT_H
#define MYPOINT_H

#include "bignum.h"
#include <QDebug>


class MyPoint
{
public:
    MyPoint();
    MyPoint(mpi x, mpi y);
    void setX(mpi x);
    void setY(mpi y);

    mpi X();
    mpi Y();
    static mpi stringToMPI(QString text);

private:
    mpi x, y;
};

#endif // MYPOINT_H
