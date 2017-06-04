#ifndef BIGWINDOW_H
#define BIGWINDOW_H

#include <QMainWindow>
#include "ecc_big.h"
#include "bignum.h"
#include "mypoint.h"
#include <qglobal.h>
#include <QTime>

namespace Ui {
class BigWindow;
}

class BigWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BigWindow(QWidget *parent = 0);
    ~BigWindow();
    void generate_equation();
    static int generateRNG(void *, unsigned char * buffer, size_t numBytes);
    QString generatePrime();
    mpi generatePrimeMPI();
    static mpi stringToMPI(QString text);
    QString mpiToString(mpi number);

private:
    Ui::BigWindow *ui;
    QString private_key_a, private_key_b;
    ECC_BIG *ecc_big;
};

#endif // BIGWINDOW_H
