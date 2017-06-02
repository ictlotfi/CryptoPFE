#include "ecc_big.h"

ECC_BIG::ECC_BIG()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

void ECC_BIG::setA(int a)
{
    this->a = a;
}

void ECC_BIG::setB(mpi b)
{
    this->b = b;
}

void ECC_BIG::setP(mpi p)
{
    this->p = p;
}

void ECC_BIG::setPrivateKey(mpi private_key)
{
    this->private_key = private_key;
}

void ECC_BIG::setBasePoint(MyPoint *p)
{
    this->base_point = p;
}

int ECC_BIG::getA()
{
    return a;
}

mpi ECC_BIG::getB()
{
    return b;
}

mpi ECC_BIG::getP()
{
    return p;
}

mpi ECC_BIG::getPrivateKey()
{
    return private_key;
}

MyPoint *ECC_BIG::getBasePoint()
{
    return base_point;
}

MyPoint *ECC_BIG::encryptPoint(MyPoint *p_new, mpi k)
{

    return NULL;
}

MyPoint *ECC_BIG::decryptPoint(MyPoint *p)
{
    return NULL;
}
/*
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
 */
MyPoint *ECC_BIG::addDouble(MyPoint *point)
{
    mpi s, n, p_x, p_y, d;
    mpi temp_x_public, temp_y_public;

    mpi_init(&s);
    mpi_init(&n);
    mpi_init(&p_x);
    mpi_init(&p_y);
    mpi_init(&d);
    mpi_init(&temp_x_public);
    mpi_init(&temp_y_public);

    p_x = point->X();
    p_y = point->Y();

    //mpi_fill_random(&n, 256, generateRNG, NULL);
  //  qDebug() << "p_x" << mpiToString(p_x);
    //qDebug() << "n0" << mpiToString(n);

    //int n = 3 * point->x() * point->x() + a ;
    mpi_mul_mpi(&n, &p_x, &p_x);
    //qDebug() << "n1" << mpiToString(n);

    mpi_mul_int(&n, &n, 3);
   // qDebug() << "n2" << mpiToString(n);
    mpi_add_int(&n, &n, a);
   // qDebug() << "n3" << mpiToString(n);

    /*int d = 2 * point->y();
    if (d < 0) {
        n *= -1;
        d *= -1;
    }
    */
    mpi_mul_int(&d, &p_y, 2);
   // qDebug() << "p_y" << mpiToString(p_y);
   // qDebug() << "d0" << mpiToString(d);
    if (mpi_cmp_int(&d, 0) == -1){
        mpi_mul_int(&n, &n, -1);
        mpi_mul_int(&d, &d, -1);
        qDebug() << "d1" << mpiToString(d);
        qDebug() << "n4" << mpiToString(n);
    }

    /*
    int x = InvMod(d, p);
    if (n * x > 0) {
        s = (n * x) % p;
    }
    else {
        s = NegMod(n * x, p);
    }
    */
    mpi x, temp0;
    mpi_init(&x);
    mpi_init(&temp0);

    mpi_inv_mod(&x, &d, &p);
    mpi_mul_mpi(&temp0, &x, &n);
    if (mpi_cmp_int(&temp0, 0) == -1){
        mpi_mul_mpi(&s, &x, &n);
        mpi_mod_mpi(&s, &s, &p);
    }
    else {
        mpi_mul_mpi(&s, &n, &x);
        s = NegModMPI(&s, &p);
    }
//qDebug() << "s" << mpiToString(s);
    /*
     * int xr_ = (s * s - 2 * point->x());
    if (xr_ < 0)
    temp_x_public = NegMod (xr_, p);
    else
    temp_x_public = xr_ % p;
    */

    mpi xr_, temp1, temp2;
    mpi_init(&xr_);
    mpi_init(&temp1);
    mpi_init(&temp2);

    mpi_mul_mpi(&temp1, &s, &s);
    mpi_mul_int(&temp2, &p_x, 2);
    mpi_sub_mpi(&xr_, &temp1, &temp2);

    if (mpi_cmp_int(&xr_, 0) == -1){ //xr_ < 0
        temp_x_public = NegModMPI(&xr_, &p);
    }
    else {
        mpi_mod_mpi(&temp_x_public, &xr_, &p);
    }

    qDebug() << "temp_x_public" << mpiToString(temp_x_public);
    /*
    int yr_ = (-point->y() + s * (point->x() - temp_x_public));
    if (yr_ < 0)
    temp_y_public = NegMod(yr_, p);
    else
    temp_y_public = yr_ % p;
    */
    mpi yr_, temp3;
    mpi_init(&yr_);
    mpi_init(&temp3);

    mpi_sub_mpi(&temp3, &p_x, &temp_x_public);
    mpi_mul_mpi(&temp3, &s, &temp3);
    mpi_sub_mpi(&yr_, &temp3, &p_y);

    if(mpi_cmp_int(&yr_, 0) == -1){ //yr_ < 0
        temp_y_public = NegModMPI(&yr_, &p);
    }
    else {
        mpi_mod_mpi(&temp_y_public, &yr_, &p);
    }
qDebug() << "temp_y_public" << mpiToString(temp_y_public);
    // return new QPoint(temp_x_public, temp_y_public);
    return new MyPoint(temp_x_public, temp_y_public);
}

MyPoint *ECC_BIG::addPoints(MyPoint *point1, MyPoint *point2)
{
    return NULL;
}

MyPoint *ECC_BIG::multPoint(MyPoint *p, int k)
{
    return NULL;
}

MyPoint *ECC_BIG::generatePoint(int m)
{
    return NULL;
}

QList<MyPoint *> *ECC_BIG::textToPoints(QString text)
{
    return NULL;
}

int ECC_BIG::charToCode(QChar ch)
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

mpi ECC_BIG::NegModMPI(mpi *a, mpi *p)
{
    /*
int b = a * -1;
int n = ceil((float)b / p);
return (n * p) - b;
*/
    mpi b, n, q, r;
    mpi_init(&b);
    mpi_init(&n);
    mpi_init(&q);
    mpi_init(&r);

    mpi_mul_int(&b, a, -1);
    /* Division by mpi: A = Q * B + R
     \param Q        Destination MPI for the quotient
     * \param R        Destination MPI for the rest value
     * \param A        Left-hand MPI
     * \param B        Right-hand MPI

    */
    mpi_div_mpi(&q, &r, &b, p);
    int res = mpi_cmp_int(&r, 0);
    if (res == 0){ // r ==0
       n = q;
    }
    else if(res > 0){
        mpi_add_int(&n, &q, 1);
    }
    else{
        mpi_sub_int(&n, &q, 1);
    }

    mpi_mul_mpi(&n, &n, p);
    mpi_sub_mpi(&n, &n, &b);

    return n;
}

QString ECC_BIG::mpiToString(mpi number)
{
    int k = 2048;
    char buff[k];
    size_t nlen = k;
    QString text = "";

    int val = mpi_write_string( &number, 10, buff, &nlen);

    //qDebug() << "val " << val;
    for( int i = 0; i < 256; i++ ){
        text += buff[i];
    }

    return text;
}

int ECC_BIG::generateRNG(void *, unsigned char *buffer, size_t numBytes)
{
    for (size_t i = 0; i < numBytes; i++) {
        buffer[i] = qrand() % 256;
    }
    return 0;
}
