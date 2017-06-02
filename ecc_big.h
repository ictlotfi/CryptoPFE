#ifndef ECC_BIG_H
#define ECC_BIG_H

#include <QDebug>
#include "cm.h"
#include "bignum.h"
#include "mypoint.h"


class ecc_big
{
public:
    ecc_big();
    void setA(int a);
    void setB(mpi b);
    void setP(mpi p);
    void setPrivateKey(mpi private_key);
    void setBasePoint(MyPoint *p);


    int getA();
    mpi getB();
    mpi getP();
    mpi getPrivateKey();
    MyPoint *getBasePoint();

    MyPoint *encryptPoint(MyPoint *p_new, int k);
    MyPoint *decryptPoint(MyPoint *p);
    MyPoint *addDouble(MyPoint *point);
    MyPoint *addPoints(MyPoint *point1, MyPoint *point2);


    MyPoint *multPoint(MyPoint *p, int k);
    MyPoint *generatePoint(int m);
    QList<MyPoint *> *textToPoints(QString text);
    int charToCode(QChar ch);


private:
    int a;
    mpi b;
    mpi p;
    mpi private_key;
    int koblitz;
    MyPoint *base_point;
    MyPoint *result_point;
};

#endif // ECC_BIG_H
