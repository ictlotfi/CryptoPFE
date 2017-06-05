#ifndef ECC_BIG_H
#define ECC_BIG_H

#include <QDebug>
#include "bignum.h"
#include "mypoint.h"
#include <qglobal.h>
#include <QTime>
#include "mycm.h"

class ECC_BIG
{
public:
    ECC_BIG();
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

   // MyPoint *encryptPoint(MyPoint *p_new, int k);
    MyPoint *encryptPointFast(MyPoint *p_new, mpi counter);
    MyPoint *decryptPoint(MyPoint *p);
    MyPoint *addDouble(MyPoint *point);
    MyPoint *addPoints(MyPoint *point1, MyPoint *point2);


    MyPoint *multPoint(MyPoint *p, int k);
    MyPoint *generatePoint(int m);
    QList<MyPoint *> *textToPoints(QString text);
    int charToCode(QChar ch);
    QChar codeToChar(int code);
    mpi NegModMPI(mpi *a, mpi *p);
    static mpi stringToMPI(QString text);
    QString mpiToString(mpi number);
    static int generateRNG(void *, unsigned char * buffer, size_t numBytes);
    MyCM *generateCm(mpi counter, MyPoint *point, MyPoint *peer_public_key);
    mpi generatePrivateKey();
    bool isOnCurve(mpi x, mpi y);
    QString pointsToText(QList<MyPoint*> *list);
    QString pointToString(MyPoint *p);


private:
    int a;
    mpi b;
    mpi p;
    mpi private_key;
    int koblitz;
    MyPoint *base_point;
    MyPoint *result_point;
    mpi r;
};

#endif // ECC_BIG_H
