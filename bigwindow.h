#ifndef BIGWINDOW_H
#define BIGWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ecc_big.h"
#include "bignum.h"
#include "mypoint.h"
#include <qglobal.h>
#include <QTime>
#include "dialogplot.h"
#include "millerrabin.h"
#include "qmath.h"

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
    void generatePrivateKeys();
    void generatePublicKeys();
    void generateRandomK();
    QList<MyCM *> textToMyCMList(QString text);
    static MyCM *stringToMyCM(QString arg0);
    QString myCMListToString(QList<MyCM *> list);
    bool isOnCurve(int x, int y, int a, int b, int p);
    QPoint *generatePoint(int a, int b, int p);

    qreal log2(qreal x);



private slots:
    void button_decrypt_message_alice();

    void on_button_encode_message_bob_clicked();

    void on_button_encode_message_alice_clicked();

    void on_button_decrypt_message_bob_clicked();

    void on_lineEdit_message_bob_textChanged(const QString &arg1);

    void on_button_decrypt_message_alice_clicked();

    void on_lineEdit_message_alice_textChanged(const QString &arg1);

    void on_actionPlot_triggered();

    void on_button_generate_clicked();

private:
    Ui::BigWindow *ui;
    mpi private_key_a, private_key_b;
    MyPoint *public_key_a, *public_key_b;
    ECC_BIG *ecc_big;
    mpi k_a, k_b;
};

#endif // BIGWINDOW_H
