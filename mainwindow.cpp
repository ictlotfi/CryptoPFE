#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "millerrabin.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

     model = new QStringListModel(this);

     generate_equation();
     on_button_generate_ec_points_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate_equation()
{
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();

    QString equation = "Y² = X3 ";
    if( a>0)  equation += " +" +QString::number(a) +"X ";
    else equation += " " +QString::number(a) +"X ";

    if( b>0)  equation += " +" +QString::number(b);
    else equation += " " +QString::number(b) ;

    equation += "       Z/"+QString::number(p)+"Z";

    ui->label_ec_equation->setText(equation);
}

void MainWindow::on_lineEdit_p_textChanged(const QString &)
{
    generate_equation();
}

void MainWindow::on_lineEdit_a_textChanged(const QString &)
{
    generate_equation();
}

void MainWindow::on_lineEdit_b_textChanged(const QString &)
{
    generate_equation();
}

void MainWindow::on_button_generate_ec_points_clicked()
{
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();
    // verify if p is a prime number
    MillerRabin *m = new MillerRabin();
    if(!m->isPrime(p)){
        QMessageBox msgBox;
        msgBox.setText("P is not prime");
        msgBox.exec();

        qDebug() << p << " is NOT prime";
        return;
    }

    // verify is 4A^3+27b^2 <> 0


    QStringList list;

    for (int x = 0; x < p; x++) {
        for (int y = 0; y < p; y++) {
            int k = y * y;
            int m = (x * x * x) + a * x + b;
            if (k % p == m % p) {
                QString temp = QString::number(x)+", "+QString::number(y);
                list.append(temp);
            }
        }
    }



    model->setStringList(list);

    ui->listView_ec_points->setModel(model);

}

void MainWindow::on_button_select_base_clicked()
{
    if(ui->listView_ec_points->currentIndex().row() < 0) return;
    QString x, y;
    QStringList telmpList = ui->listView_ec_points->currentIndex().data().toString().split(",");
    x = telmpList.at(0);
    y = telmpList.at(1);

    ui->lineEdit_base_x->setText(x);
    ui->lineEdit_base_y->setText(y);

    ui->lineEdit_private_key_alice->setFocus();
}

void MainWindow::on_button_generate_public_key_alice_clicked()
{
    //if(ui->listView_ec_points->currentIndex().row() < 0) return;
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();
    int private_key = ui->lineEdit_private_key_alice->text().toInt();

    if (private_key>= p || private_key < 1){
        QMessageBox msgBox;
        msgBox.setText("The private key is not between 1 and p-1");
        msgBox.exec();

        return;
    }
    QString x, y;
    QStringList telmpList = ui->listView_ec_points->currentIndex().data().toString().split(",");
    //x = telmpList.at(0);
    //y = telmpList.at(1);
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main = new ECC();
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);
    ecc_main->setBasePoint(base_point);
    ecc_main->setPrivateKey(private_key);
    ecc_main->generatePublicKey();

    //qDebug() << ecc->getPublicKey()->x();
    ui->lineEdit_public_key_alice->setText(QString::number(ecc_main->getPublicKey()->x()) + ", " +
                                                     QString::number(ecc_main->getPublicKey()->y()));

}


QPoint *MainWindow::encrypt(QPoint *p)
{
    /*
     *
    int xr, yr;
    scalar_multiplicaiton(PUx, PUy, k, a, p, xr, yr);
    add_points(Mx, My, xr, yr, Cx, Cy, p);
     *
    */
    ecc_main->setBasePoint(ecc_main->getPublicKey());
    ecc_main->generatePublicKey();


    ecc_main->setBasePoint(p);
    ecc_main->addPoints();

    QPoint *cipher = ecc_main->getPublicKey();

    ui->lineEdit_cipher->setText(QString::number(cipher->x()) + ", " +
                                                     QString::number(cipher->y()));
}

void MainWindow::on_button_select_message_clicked()
{
    if(ui->listView_ec_points->currentIndex().row() < 0) return;
    ui->lineEdit_message->setText(ui->listView_ec_points->currentIndex().data().toString());
}

void MainWindow::on_button_generate_cipher_clicked()
{
    QString x, y;
    QStringList telmpList = ui->lineEdit_message->text().split(",");
    x = telmpList.at(0);
    y = telmpList.at(1);

    QPoint *p = new QPoint(x.toInt(), y.toInt());

    QPoint *p_encrypted = encrypt(p);
}

int premier (long n) {
  long d;

  if (n % 2 == 0)
    return (n == 2);
  for (d = 3; d * d <= n; d = d + 2)
    if (n % d == 0)
      return 0;
  return 1;
}

void MainWindow::on_button_generate_public_key_bob_clicked()
{
    if(ui->listView_ec_points->currentIndex().row() < 0) return;
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();
    int private_key = ui->lineEdit_private_key_bob->text().toInt();

    if (private_key>= p || private_key < 1){
        QMessageBox msgBox;
        msgBox.setText("The private key is not between 1 and p-1");
        msgBox.exec();

        return;
    }
    QString x, y;
    QStringList telmpList = ui->listView_ec_points->currentIndex().data().toString().split(",");
    x = telmpList.at(0);
    y = telmpList.at(1);

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main = new ECC();
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);
    ecc_main->setBasePoint(base_point);
    ecc_main->setPrivateKey(private_key);
    ecc_main->generatePublicKey();

    //qDebug() << ecc->getPublicKey()->x();
    ui->lineEdit_public_key_bob->setText(QString::number(ecc_main->getPublicKey()->x()) + ", " +
                                                     QString::number(ecc_main->getPublicKey()->y()));
}
