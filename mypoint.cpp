#include "mypoint.h"

MyPoint::MyPoint()
{
    mpi_init(&x);
    mpi_init(&y);

    x = stringToMPI("0");
    y = stringToMPI("0");
}
MyPoint::MyPoint(mpi x, mpi y)
{
    this->x = x;
    this->y = y;
}

void MyPoint::setX(mpi x)
{
    this->x = x;
}

void MyPoint::setY(mpi y)
{
    this->y = y;
}

mpi MyPoint::X()
{
    return x;
}

mpi MyPoint::Y()
{
    return y;
}

mpi MyPoint::stringToMPI(QString text)
{
    mpi number; mpi_init(&number);
    int k = 224;
    char buff[k];

    for (int i = 0; i < k; i++){
        buff[i] = '\n';
    }

    for (int i = 0; i < text.size(); i++){
        buff[i] = text.at(i).toLatin1();
    }
    mpi_read_string( &number, 10, buff);

    return number;
}
