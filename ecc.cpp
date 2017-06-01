#include "ecc.h"

ECC::ECC()
{
    base_point = new QPoint();
    result_point = new QPoint();
    koblitz = 30;
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
    this->result_point = p;
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
    return this->result_point;
}

void ECC::generatePublicKey()
{
    QPoint *p = generatePoint(1);
    /*int x3 = mult_x(base_point->x(), base_point->y(), private_key);
    int y3 = mult_y(base_point->x(), base_point->y(), private_key);
    qDebug() << "base_point->x() " << base_point->x();
    qDebug() << "base_point->y() " << base_point->y();*/
    qDebug() << "x " << p->x();
    qDebug() << "y " << p->y();


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

    result_point->setX(temp_x_public);
    result_point->setY(temp_y_public);

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

int ECC::add_x(int x1, int y1, int x2, int y2)
{
    if ( x1>x2 )
    {
    int temp=x2 ;
    x2=x1 ; x1=temp ;
    temp=y2 ;
    y2=y1 ; y1=temp ;
    }
    int x3 , m;
    if ( x1 == 0 ) { x3=x2 ; }
    else if ( x2 ==0) { x3=x1 ; }
    else if ( x1 != x2 )
    {
    m = NegMod ((NegMod((y2-y1), p ) * InvMod ( NegMod ( x2-x1 , p ) , p ) ) , p );
    x3 = NegMod ( ( (m*m) -x1 -x2 ) , p ) ;
    }
    else if ( ( x1==x2 ) && ( y1 != y2 ) ) { x3 =0;}
    else if ( x1==x2 && ( y1==y2 ) && y1 !=0)
    {
    m = NegMod ( ( ( 3 * ( x1 * x1 )+a ) * InvMod ( ( 2 * y1 ) , p ) ) , p );
    x3 = NegMod ( ( (m*m) - x1 - x2 ) , p ) ;
    }
    else if ( x1==x2 && y1==y2 && y1==0) { x3 =0;}
    return NegMod ( x3 , p ) ;
}

int ECC::add_y(int x1, int y1, int x2, int y2, int x3)
{
    if ( x1>x2 )
    {
    int temp=x2 ;
    x2=x1 ; x1=temp ;
    temp=y2 ;
    y2=y1 ; y1=temp ;
    }
    int m, y3 ;
    if ( x1 == 0 ) { y3=y2 ; }
    else if ( x2 ==0) { y3=y1 ; }
    else if ( x1 != x2 )
    {
        m = NegMod ( ( NegMod ( ( y2-y1 ) , p ) * InvMod ( NegMod ( x2-x1 , p ) , p ) ) , p );
        y3 = NegMod ( ( (m * NegMod ( ( x1-x3 ) , p ) ) -y1 ) , p ) ;
    }

    else  if ( ( x1==x2 ) && ( y1 != y2 ) ) { y3 =0;}
    else if ( x1==x2 && ( y1==y2 ) && y1 !=0)
    {
    m = NegMod ( ( ( 3 * ( x1 * x1 )+a ) * InvMod ( ( 2 * y1 ) , p ) ) , p ) ;
    y3 = NegMod ( ( (m * NegMod ( ( x1-x3 ) , p ) ) -y1 ) , p ) ;
    }
    else if ( x1==x2 && y1==y2 && y1==0) { y3 =0;}
    return NegMod ( y3 , p ) ;
}

int ECC::mult_x(int x, int y, int k)
{
    int Rx , Ry ;
    Rx=x ;
    Ry=y ;
    int temp , compteur ;
    for ( compteur =1; compteur < k ; compteur++)
    {
    temp=Rx ;
    Rx=add_x ( x , y , Rx , Ry) ;
    Ry=add_y ( x , y , temp , Ry , Rx) ;
    }
    return Rx ;
}

int ECC::mult_y(int x, int y, int k)
{
    int Rx , Ry ;
    Rx=x ;
    Ry=y ;
    int temp , compteur ;
    for ( compteur =1; compteur < k ; compteur++)
    {
    temp=Rx ;
    Rx=add_x ( x , y , Rx , Ry) ;
    Ry=add_y ( x , y , temp , Ry , Rx) ;
    }
    return Ry ;
}

QPoint *ECC::addPoints(QPoint *p1, QPoint *p2)
{
    int x3 = add_x(p1->x(), p1->y(), p2->x(), p2->y());
    int y3 = add_y(p1->x(), p1->y(), p2->x(), p2->y(), x3);

     return new QPoint(x3, y3);
}

QPoint *ECC::multPoint(QPoint *p, int k)
{
    int x3 = mult_x(p->x(), p->y(), private_key);
    int y3 = mult_y(p->x(), p->y(), private_key);

    return new QPoint(x3, y3);
}

QPoint *ECC::generatePoint(int m)
{
    for (int i=1; i < koblitz; i++){
        long x = m * koblitz + i;
        long yy = x*x*x +a*x +b;

        for (int y = 0; y < p; y++) {
            int k = y * y;
            if (k % p == yy % p) {
                // point exists
                qDebug() << "1 " << k % p;
                qDebug() << "2 " << yy % p;

                return new QPoint(x, y);
            }
        }
    }
    return new QPoint(0, 0);
}

void ECC::addPoints () //int xp, int yp, int xq, int yq, int &xr, int &yr, int p
{
    int s;
    int temp_x_public, temp_y_public;
    int n = base_point->y() - result_point->y();
    int d = base_point->x() - result_point->x();

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
    int xr_ = (s * s - base_point->x() - result_point->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    int yr_ = (-base_point->y() + s * (base_point->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;

    result_point->setX(temp_x_public);
    result_point->setY(temp_y_public);
}
