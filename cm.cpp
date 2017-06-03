#include "cm.h"

CM::CM()
{

}

CM::CM(QPoint *p1, QPoint *p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

void CM::setP1(QPoint *p)
{
    this->p1 = p;
}

void CM::setP2(QPoint *p)
{
    this->p2 = p;
}

QPoint *CM::getP1()
{
    return p1;
}

QPoint *CM::getP2()
{
    return p2;
}

QString CM::toString()
{
    return "(" + QString::number(p1->x()) + ", " + QString::number(p1->y()) + ") ("+
            QString::number(p2->x()) + ", " + QString::number(p2->y()) + ")";
}
