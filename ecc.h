#ifndef ECC_H
#define ECC_H

#include <QPoint>
#include <QDebug>
#include "cm.h"


class ECC
{
public:
    ECC();
    void setA(int a);
    void setB(int b);
    void setP(int p);
    void setPrivateKey(int private_key);
    void setBasePoint(QPoint *p);
    void setPublicKey(QPoint *p);


    int getA();
    int getB();
    int getP();
    int getPrivateKey();
    QPoint *getBasePoint();
    QPoint *getPublicKey();

    QPoint * encryptPoint(QPoint *p_new, int k);
    QPoint *decryptPoint(QPoint *p);
    QPoint *addDouble(QPoint *point);
    QPoint *addPoints(QPoint *point1, QPoint *point2);
    int InvMod(int x, int n);
    int EGCD(int a, int b, int& u, int &v);
    int NegMod(int a, int p);

    int add_x(int x1, int y1, int x2, int y2);
    int add_y(int x1, int y1, int x2, int y2, int x3);
    int mult_x (int x, int y, int k);
    int mult_y (int x, int y, int k);
    //QPoint *addPoints(QPoint *p1, QPoint *p2);
    QPoint *multPoint(QPoint *p, int k);
    QPoint *generatePoint(int m);
    QList<QPoint *> *textToPoints(QString text);
    int charToCode(QChar ch);
    CM *generateCm(int k, QPoint *point, QPoint *peer_public_key);

private:
    int a;
    int b;
    int p;
    int private_key;
    int koblitz;
    QPoint *base_point;
    QPoint *result_point;
};

#endif // ECC_H
