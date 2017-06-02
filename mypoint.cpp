#include "mypoint.h"

MyPoint::MyPoint()
{

}
MyPoint::MyPoint(mpi x, mpi y)
{
    this->x = x;
    this->y = y;
}

void MyPoint::setX(mpi x)
{
    this->x = x;
}

void MyPoint::setY(mpi y)
{
    this->y = y;
}

mpi MyPoint::X()
{
    return x;
}

mpi MyPoint::Y()
{
    return y;
}
