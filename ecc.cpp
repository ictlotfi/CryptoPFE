#include "ecc.h"

ECC::ECC()
{
    base_point = new QPoint();
    public_key = new QPoint();
}

void ECC::setA(int a)
{
    this->a = a;
}

void ECC::setB(int b)
{
    this->b = b;
}

void ECC::setP(int p)
{
    this->p = p;
}

void ECC::setPrivateKey(int private_key)
{
    this->private_key = private_key;
}

void ECC::setBasePoint(QPoint *p)
{
    this->base_point = p;
}

void ECC::setPublicKey(QPoint *p)
{
    this->public_key = p;
}

int ECC::getA()
{
    return this->a;
}

int ECC::getB()
{
    return this->b;
}

int ECC::getP()
{
    return this->p;
}

int ECC::getPrivateKey()
{
    return this->private_key;
}

QPoint *ECC::getBasePoint()
{
    return this->base_point;
}

QPoint *ECC::getPublicKey()
{
    return this->public_key;
}

void ECC::generatePublicKey()
{
    if (private_key == 2) {
        addDouble();
    }
    else if (private_key > 2) {
        addDouble();
        for (int i = 0; i < private_key - 2; i++) {
            addPoints();
        }
    }
}

void ECC::addDouble() // int xp, int yp, int &xr, int &yr, int a, int p
{
    int s;
    int temp_x_public, temp_y_public;
    int n = 3 * base_point->x() * base_point->x() + a ;
    int d = 2 * base_point->y();
    if (d < 0) {
        n *= -1;
        d *= -1;
    }
    int x = InvMod(d, p);
    if (n * x > 0) {
        s = (n * x) % p;
    }
    else {
        s = NegMod(n * x, p);
    }
    int xr_ = (s * s - 2 * base_point->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    int yr_ = (-base_point->y() + s * (base_point->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;

    public_key->setX(temp_x_public);
    public_key->setY(temp_y_public);

    /*qDebug() << "base_point->x() " << base_point->x();
    qDebug() << "base_point->y() " << base_point->y();
    qDebug() << "a " << a;
    qDebug() << "b " << b;
    qDebug() << "p " << p;
    qDebug() << "private_key " << private_key;
    qDebug() << "x " << temp_x_public;
    qDebug() << "y " << temp_y_public;*/
}

int ECC::InvMod(int x, int n) // Solve linear congruence equation x * z == 1 (mod n)for z
{
    x = x % n;
    int u,v,g,z;
    g = EGCD(x, n, u,v);
    if (g != 1)
    {
    z = 0;
    }
    else
    {
    z = u % n;
    }
    return z;
}

int ECC::EGCD(int a, int b, int& u, int &v) //Extended GCD gives g = a*u + b*v
{
    u = 1;
    v = 0;
    int g = a;
    int u1 = 0;
    int v1 = 1;
    int g1 = b;

    while (g1 != 0)
    {
        int q = g/g1; // Integer divide
        int t1 = u - q*u1;
        int t2 = v - q*v1;
        int t3 = g - q*g1;
        u = u1;
        v = v1;
        g = g1;
        u1 = t1;
        v1 = t2;
        g1 = t3;
    }
    return g;
}


int ECC::NegMod(int a, int p)
{
int b = a * -1;
int n = ceil((float)b / p);
return (n * p) - b;
}

void ECC::addPoints () //int xp, int yp, int xq, int yq, int &xr, int &yr, int p
{
    int s;
    int temp_x_public, temp_y_public;
    int n = base_point->y() - public_key->y();
    int d = base_point->x() - public_key->x();

    if (d < 0) {
            n *= -1; d *= -1;
    }
    int x = InvMod(d, p);
    if (n * x > 0) {
    s = (n * x) % p;
    }
    else {
    s = NegMod(n * x, p);
    }
    int xr_ = (s * s - base_point->x() - public_key->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    int yr_ = (-base_point->y() + s * (base_point->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;

    public_key->setX(temp_x_public);
    public_key->setY(temp_y_public);
}
