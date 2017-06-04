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

QString MyPoint::mpiToString(mpi number)
{
    int k = 512;
    char buff[k];
    size_t nlen = k;
    QString text = "";

    mpi_write_string( &number, 10, buff, &nlen);

    for( int i = 0; i < k; i++ ){
        if (buff[i] == '0' || buff[i] == '1' || buff[i] == '2' || buff[i] == '3' || buff[i] == '4' || buff[i] == '5'
                || buff[i] == '6' || buff[i] == '7' || buff[i] == '8' || buff[i] == '9')  text += buff[i];
        else return text;
    }

    return text;
}

QString MyPoint::toString()
{
    return "("+mpiToString(x)+", "+mpiToString(y)+")";
}
