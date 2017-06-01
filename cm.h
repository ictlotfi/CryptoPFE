#ifndef CM_H
#define CM_H

#include <QPoint>
#include <QDebug>

class CM
{
public:
    CM();
    CM(QPoint *p1, QPoint *p2);
    void setP1(QPoint *p);
    void setP2(QPoint *p);
    QPoint *getP1();
    QPoint *getP2();
    QString toString();

private:
    QPoint *p1, *p2;
};

#endif // CM_H
