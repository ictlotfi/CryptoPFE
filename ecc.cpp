#include "ecc.h"
#include "alphabet.h"


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
    this->base_point->setX(p->x());
    this->base_point->setY(p->y());
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

QPoint *ECC::encryptPoint(QPoint *p_new, int k)
{
    //QPoint *p = generatePoint(1);
    /*int x3 = mult_x(base_point->x(), base_point->y(), private_key);
    int y3 = mult_y(base_point->x(), base_point->y(), private_key);
    qDebug() << "base_point->x() " << base_point->x();
    qDebug() << "base_point->y() " << base_point->y();*/
    //qDebug() << "x " << p->x();
    //qDebug() << "y " << p->y();

    QPoint *p ;

    if (k == 2) {
        p = addDouble(p_new);
    }
    else if (k > 2) {
        p = addDouble(p_new);
        for (int i = 0; i < k - 2; i++) {
            p = addPoints(p_new, p);
        }
    }

    return p;
}

QPoint *ECC::decryptPoint(QPoint *p)
{

}

QPoint *ECC::addDouble(QPoint *point) // int xp, int yp, int &xr, int &yr, int a, int p
{

    int s;
    int temp_x_public, temp_y_public;
    int n = 3 * point->x() * point->x() + a ;
    int d = 2 * point->y();
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
    int xr_ = (s * s - 2 * point->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    int yr_ = (-point->y() + s * (point->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;

    return new QPoint(temp_x_public, temp_y_public);
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

/*QPoint *ECC::addPoints(QPoint *p1, QPoint *p2)
{
    int x3 = add_x(p1->x(), p1->y(), p2->x(), p2->y());
    int y3 = add_y(p1->x(), p1->y(), p2->x(), p2->y(), x3);

     return new QPoint(x3, y3);
}*/

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
                //qDebug() << "1 " << k % p;
               // qDebug() << "2 " << yy % p;

                return new QPoint(x, y);
            }
        }
    }
    return new QPoint(0, 0);
}

QList<QPoint*> *ECC::textToPoints(QString text)
{
    QList<QPoint*> *list = new QList<QPoint*>();

    for (int i = 0; i < text.size(); i++){
        QChar ch = text[i];
        int m = charToCode(ch);
        QPoint *point = generatePoint(m);
        list->append(point);
    }
    return list;
}

int ECC::charToCode(QChar ch)
{
    if (ch == '0') return 0;
    else if (ch == '1') return 1;
    else if (ch == '2') return 2;
    else if (ch == '3') return 3;
    else if (ch == '4') return 4;
    else if (ch == '5') return 5;
    else if (ch == '6') return 6;
    else if (ch == '7') return 7;
    else if (ch == '8') return 8;
    else if (ch == '9') return 9;
    else if (ch == 'a') return 10;
    else if (ch == 'b') return 11;
    else if (ch == 'c') return 12;
    else if (ch == 'd') return 13;
    else if (ch == 'e') return 14;
    else if (ch == 'f') return 15;
    else if (ch == 'g') return 16;
    else if (ch == 'h') return 17;
    else if (ch == 'i') return 18;
    else if (ch == 'j') return 19;
    else if (ch == 'k') return 20;
    else if (ch == 'l') return 21;
    else if (ch == 'm') return 22;
    else if (ch == 'n') return 23;
    else if (ch == 'o') return 24;
    else if (ch == 'p') return 25;
    else if (ch == 'q') return 26;
    else if (ch == 'r') return 27;
    else if (ch == 's') return 28;
    else if (ch == 't') return 29;
    else if (ch == 'u') return 30;
    else if (ch == 'v') return 31;
    else if (ch == 'w') return 32;
    else if (ch == 'x') return 33;
    else if (ch == 'y') return 34;
    else if (ch == 'z') return 35;
}

CM *ECC::generateCm(int k, QPoint *point, QPoint *peer_public_key)
{
    QPoint *p1 = encryptPoint(base_point, k);
    QPoint *p2 = encryptPoint(peer_public_key, k);
    p2 = addPoints(point, p2);

    return new CM(p1, p2);
}

QPoint *ECC::addPoints (QPoint *point1, QPoint *point2) //int xp, int yp, int xq, int yq, int &xr, int &yr, int p
{
    int s;
    int temp_x_public, temp_y_public;
    int n = point1->y() - point2->y();
    int d = point1->x() - point2->x();

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
    int xr_ = (s * s - point1->x() - point2->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    int yr_ = (-point1->y() + s * (point1->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;

    return new QPoint(temp_x_public, temp_y_public);
}
