#include "mycm.h"

MyCM::MyCM()
{

}

MyCM::MyCM(MyPoint *p1, MyPoint *p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

void MyCM::setP1(MyPoint *p)
{
    this->p1 = p;
}

void MyCM::setP2(MyPoint *p)
{
    this->p2 = p;
}

MyPoint *MyCM::getP1()
{
    return p1;
}

MyPoint *MyCM::getP2()
{
    return p2;
}

mpi MyCM::stringToMPI(QString text)
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

QString MyCM::mpiToString(mpi number)
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

QString MyCM::toString()
{
    return "{("+mpiToString(p1->X())+","+mpiToString(p1->Y())+"),("+mpiToString(p2->X())+","+mpiToString(p2->Y())+")}";
}

