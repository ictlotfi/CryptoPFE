#include "mycm.h"

MyCM::MyCM()
{

}

MyCM::MyCM(MyPoint *p1, MyPoint *p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

void MyCM::setP1(MyPoint *p)
{
    this->p1 = p;
}

void MyCM::setP2(MyPoint *p)
{
    this->p2 = p;
}

MyPoint *MyCM::getP1()
{
    return p1;
}

MyPoint *MyCM::getP2()
{
    return p2;
}

QString MyCM::toString()
{

}
