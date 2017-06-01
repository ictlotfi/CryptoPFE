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
     k_a = 11;
     k_b = 15;
     private_key_a = 17;
     private_key_b = 19;


     generate_equation();
     on_button_generate_ec_points_clicked();

     generatePublicKeys();
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

    int private_key = ui->lineEdit_private_key_alice->text().toInt();
    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();
    QPoint *base_point = new QPoint(x.toInt(), y.toInt());

    ecc_main = new ECC();
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);
    ecc_main->setBasePoint(base_point);
    ecc_main->setPrivateKey(private_key);
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

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main->setBasePoint(base_point);

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

    QStringList telmpList = ui->listView_ec_points->currentIndex().data().toString().split(",");
    //x = telmpList.at(0);
    //y = telmpList.at(1);
    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main = new ECC();
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);
    ecc_main->setBasePoint(base_point);
    ecc_main->setPrivateKey(private_key);

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


    ecc_main->setBasePoint(p);
    //ecc_main->addPoints();

    QPoint *cipher = ecc_main->getPublicKey();

    ui->lineEdit_cipher->setText(QString::number(cipher->x()) + ", " +
                                 QString::number(cipher->y()));
}

void MainWindow::generatePublicKeys()
{
    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());

    peer_public_key_a = ecc_main->encryptPoint(base_point, private_key_a);
    peer_public_key_b = ecc_main->encryptPoint(base_point, private_key_b);
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

    //qDebug() << ecc->getPublicKey()->x();
    ui->lineEdit_public_key_bob->setText(QString::number(ecc_main->getPublicKey()->x()) + ", " +
                                                     QString::number(ecc_main->getPublicKey()->y()));
}

void MainWindow::on_button_encode_message_clicked()
{
    ui->textEdit_encoded_message->clear();
    ui->textEdit_encrypted_message->clear();
    ui->textEdit_cm_message->clear();
    ui->textEdit_decrypted_message->clear();

    QString message = ui->lineEdit_message->text();

    QList<QPoint*> *list = ecc_main->textToPoints(message.toLower());
    for (int i = 0; i < list->size(); i++){
        QPoint *p = list->at(i);
        QString encodedPoint = "("+QString::number(p->x())+", "+QString::number(p->y())+")";
        ui->textEdit_encoded_message->append(encodedPoint);

        // encrypt
        QPoint *p_encrypted = ecc_main->encryptPoint(p, k_b);
        QString encryptedPoint = "("+QString::number(p_encrypted->x())+", "+QString::number(p_encrypted->y())+")";
        ui->textEdit_encrypted_message->append(encryptedPoint);

        // generate CM

        CM *cm = ecc_main->generateCm(k_a, p, peer_public_key_b);
        ui->textEdit_cm_message->append(cm->toString());

        //decrypt
        QPoint *p1 = cm->getP1();
        QPoint *p2 = cm->getP2();

        QPoint *pp1 = ecc_main->encryptPoint(p1, private_key_b); // n_a * (k_a*G)
        int xx = p2->x() - p1->x();
        qDebug() << "pp1->x() " << pp1->x();
        qDebug() << "pp1->y() " << pp1->y();

        pp1->setY(-pp1->y());

        QPoint *pp2 = ecc_main->addPoints(p2, pp1);
        qDebug() << "pp2->x() " << pp2->x();
        qDebug() << "pp2->y() " << pp2->y();
    }
}
