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

    int add_x(int x1, int y1, int x2, int y2);
    int add_y(int x1, int y1, int x2, int y2, int x3);
    int mult_x (int x, int y, int k);
    int mult_y (int x, int y, int k);
    QPoint *addPoints(QPoint *p1, QPoint *p2);
    QPoint *multPoint(QPoint *p, int k);
    QPoint *generatePoint(int m);
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
