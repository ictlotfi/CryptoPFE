#include "bigwindow.h"
#include "ui_bigwindow.h"

BigWindow::BigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BigWindow)
{
    ui->setupUi(this);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    private_key_a = "20723429452102997097693055120908112174847588083791179561894667245437";
    private_key_b = "26783546327533480407843357618229179705380334259814175764895254907511";

    int k = 224;
    char buff[k];
    size_t nlen = k;
    QString text = "";
    mpi number; mpi_init(&number);
/*
    //mpi_fill_random(&message, 20, generateRNG, NULL);
    mpi_gen_prime( &message, k, 0, generateRNG, NULL );
    mpi_write_string( &message, 10, buff, &nlen);

    for( int i = 0; i < 68; i++ ){
        number += buff[i];
    }
    qDebug() << number << " size: " << number.size();*/
    //ui->lineEdit_p->setText(number);

    number = stringToMPI(private_key_a);

    text = mpiToString(number);

    qDebug() << text;
}

BigWindow::~BigWindow()
{
    delete ui;
}

int BigWindow::generateRNG(void *, unsigned char *buffer, size_t numBytes)
{
    for (size_t i = 0; i < numBytes; i++) {
        buffer[i] = qrand() % 256;
    }
    return 0;
}

QString BigWindow::generatePrime()
{
    int k = 224;
    char buff[k];
    size_t nlen = k;
    QString text = "";
    mpi number; mpi_init(&number);

    mpi_gen_prime( &number, k, 0, generateRNG, NULL );
    mpi_write_string( &number, 10, buff, &nlen);

    for( int i = 0; i < 68; i++ ){
        text += buff[i];
    }
    return text;
}

mpi BigWindow::generatePrimeMPI()
{
    int k = 224;
    mpi number; mpi_init(&number);
    mpi_gen_prime( &number, k, 0, generateRNG, NULL );

    return number;
}

mpi BigWindow::stringToMPI(QString text)
{
    mpi number; mpi_init(&number);
    int k = 224;
    char buff[k];

    for (int i = 0; i < text.size(); i++){
        buff[i] = text.at(i).toLatin1();
    }
    mpi_read_string( &number, 10, buff);

    return number;
}

QString BigWindow::mpiToString(mpi number)
{
    int k = 224;
    char buff[k];
    size_t nlen = k;
    QString text = "";

    mpi_write_string( &number, 10, buff, &nlen);

    for( int i = 0; i < 68; i++ ){
        text += buff[i];
    }

    return text;
}
