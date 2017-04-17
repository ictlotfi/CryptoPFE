#ifndef ECC_H
#define ECC_H

#include <QPoint>
#include <QDebug>


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

    void generatePublicKey();
    void addDouble();
    void addPoints ();
    int InvMod(int x, int n);
    int EGCD(int a, int b, int& u, int &v);
    int NegMod(int a, int p);

private:
    int a;
    int b;
    int p;
    int private_key;
    QPoint *base_point;
    QPoint *public_key;
};

#endif // ECC_H
