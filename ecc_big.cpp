#include "ecc_big.h"

ecc_big::ecc_big()
{

}

void ecc_big::setA(int a)
{
    this->a = a;
}

void ecc_big::setB(mpi b)
{
    this->b = b;
}

void ecc_big::setP(mpi p)
{
    this->p = p;
}

void ecc_big::setPrivateKey(mpi private_key)
{
    this->private_key = private_key;
}

void ecc_big::setBasePoint(MyPoint *p)
{
    this->base_point = p;
}

int ecc_big::getA()
{
    return a;
}

mpi ecc_big::getB()
{
    return b;
}

mpi ecc_big::getP()
{
    return p;
}

mpi ecc_big::getPrivateKey()
{
    return private_key;
}

MyPoint *ecc_big::getBasePoint()
{
    return base_point;
}

MyPoint *ecc_big::encryptPoint(MyPoint *p_new, int k)
{
    return NULL;
}

MyPoint *ecc_big::decryptPoint(MyPoint *p)
{
    return NULL;
}

MyPoint *ecc_big::addDouble(MyPoint *point)
{
    return NULL;
}

MyPoint *ecc_big::addPoints(MyPoint *point1, MyPoint *point2)
{
    return NULL;
}

MyPoint *ecc_big::multPoint(MyPoint *p, int k)
{
    return NULL;
}

MyPoint *ecc_big::generatePoint(int m)
{
    return NULL;
}

QList<MyPoint *> *ecc_big::textToPoints(QString text)
{
    return NULL;
}

int ecc_big::charToCode(QChar ch)
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
