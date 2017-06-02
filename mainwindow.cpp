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
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    this->showMaximized();
    ecc_main = new ECC();

     model = new QStringListModel(this);

     //private_key_a = 17;
     //private_key_b = 19;

     //k_a = 8;//generateRandomNumber();
     //k_b = 12;//generateRandomNumber();

     k_a = 11;
     k_b = 15;
     //private_key_a = 17;
     //private_key_b = 19;

     generate_equation();
     on_button_generate_ec_points_clicked();


     on_button_generate_public_key_alice_clicked();
     on_button_generate_public_key_bob_clicked();

     //generatePublicKeys();

     QRegExp validatorAlphaNumeric("[A-Za-z0-9]*");
     ui->lineEdit_message->setValidator(new QRegExpValidator(validatorAlphaNumeric, ui->lineEdit_message));

     QRegExp validatorNumeric("[0-9]*");
     ui->lineEdit_private_key_alice->setValidator(new QRegExpValidator(validatorNumeric, ui->lineEdit_private_key_alice));
     ui->lineEdit_private_key_bob->setValidator(new QRegExpValidator(validatorNumeric, ui->lineEdit_private_key_bob));
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

    QString equation = "Y^2 = X^3 ";
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
    /*MillerRabin *m = new MillerRabin();
    if(!m->isPrime(p)){
        QMessageBox msgBox;
        msgBox.setText("P is not prime");
        msgBox.exec();

        qDebug() << p << " is NOT prime";
        return;
    }*/

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



void MainWindow::generatePublicKeys()
{
    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());

    private_key_a = ui->lineEdit_private_key_alice->text().toInt();
    private_key_b = ui->lineEdit_private_key_bob->text().toInt();

    peer_public_key_a = ecc_main->encryptPoint(base_point, private_key_a);
    peer_public_key_b = ecc_main->encryptPoint(base_point, private_key_b);

    qDebug() << "wwwprivate_key_b " << private_key_b;
    qDebug() << "wwwpeer_public_key_b->x() " << peer_public_key_b->x();
    qDebug() << "wwwpeer_public_key_b->y() " << peer_public_key_b->y();
}

void MainWindow::generate_public_key_alice(int private_key)
{
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();

    if (private_key>= p || private_key < 1){
        QMessageBox msgBox;
        msgBox.setText("The private key is not between 1 and p-1");
        msgBox.exec();

        return;
    }

    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);

    QPoint *point = ecc_main->encryptPoint(base_point, private_key);

    ui->lineEdit_public_key_alice->setText(QString::number(point->x()) + ", " +
                                                     QString::number(point->y()));

    private_key_a = ui->lineEdit_private_key_alice->text().toInt();
    peer_public_key_a = ecc_main->encryptPoint(base_point, private_key_a);


}

void MainWindow::generate_public_key_bob(int private_key)
{
    int p = ui->lineEdit_p->text().toInt();
    int a = ui->lineEdit_a->text().toInt();
    int b = ui->lineEdit_b->text().toInt();

    if (private_key>= p || private_key < 1){
        QMessageBox msgBox;
        msgBox.setText("The private key is not between 1 and p-1");
        msgBox.exec();

        return;
    }
    QString x, y;
    x = ui->lineEdit_base_x->text();
    y = ui->lineEdit_base_y->text();

    QPoint *base_point = new QPoint(x.toInt(), y.toInt());
    ecc_main->setA(a);
    ecc_main->setB(b);
    ecc_main->setP(p);

     QPoint *point = ecc_main->encryptPoint(base_point, private_key);

    ui->lineEdit_public_key_bob->setText(QString::number(point->x()) + ", " +
                                         QString::number(point->y()));

    private_key_b = ui->lineEdit_private_key_bob->text().toInt();
    peer_public_key_b = ecc_main->encryptPoint(base_point, private_key_b);
    qDebug() << "private_key_b " << private_key_b;
    qDebug() << "peer_public_key_b->x() " << peer_public_key_b->x();
    qDebug() << "peer_public_key_b->y() " << peer_public_key_b->y();
}

int MainWindow::generateRandomNumber()
{
    int p = ui->lineEdit_p->text().toInt();
    return qrand() % ((p + 1) - 1) + 1;
}

void MainWindow::on_button_select_message_clicked()
{
    if(ui->listView_ec_points->currentIndex().row() < 0) return;
    ui->lineEdit_message->setText(ui->listView_ec_points->currentIndex().data().toString());
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

void MainWindow::on_button_generate_public_key_alice_clicked()
{
    int private_key = generateRandomNumber();
    ui->lineEdit_private_key_alice->setText(QString::number(private_key));
}
void MainWindow::on_button_generate_public_key_bob_clicked()
{
    int private_key = generateRandomNumber();
    ui->lineEdit_private_key_bob->setText(QString::number(private_key));
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
       /* QPoint *p_encrypted = ecc_main->encryptPoint(p, k_b);
        QString encryptedPoint = "("+QString::number(p_encrypted->x())+", "+QString::number(p_encrypted->y())+")";
        ui->textEdit_encrypted_message->append(encryptedPoint);
*/
        // generate CM
       // qDebug() << "peer_public_key_b->x() " << peer_public_key_b->x();
        //qDebug() << "peer_public_key_b->y() " << peer_public_key_b->y();

        CM *cm = ecc_main->generateCm(k_a, p, peer_public_key_b);
        ui->textEdit_cm_message->append(cm->toString());

        //decrypt
        QPoint *p1 = cm->getP1();
        QPoint *p2 = cm->getP2();

        qDebug() << "private_key_b " <<private_key_b;

        QPoint *pp1 = ecc_main->encryptPoint(p1, private_key_b); // n_b * (k_a*G)
        //int xx = p2->x() - p1->x();

        qDebug() << "pp1->x() " << pp1->x();
        qDebug() << "pp1->y() " << pp1->y();

        pp1->setY(-pp1->y());

        QPoint *pp2 = ecc_main->addPoints(p2, pp1);

        qDebug() << "pp2->x() " << pp2->x();
        qDebug() << "pp2->y() " << pp2->y();
    }

    QPoint *p0 = new QPoint(15, 12);
    QPoint *p00 = new QPoint(15, 1051);

    p0 = ecc_main->addPoints(p0, p00);
    qDebug() << "qqqq->x() " << p0->x();
    qDebug() << "qqqq->y() " << p0->y();
}

void MainWindow::on_lineEdit_private_key_alice_textChanged(const QString &arg1)
{
    int private_key = arg1.toInt();
    generate_public_key_alice(private_key);
}

void MainWindow::on_lineEdit_private_key_bob_textChanged(const QString &arg1)
{
    int private_key = arg1.toInt();
    generate_public_key_bob(private_key);
}

void MainWindow::on_lineEdit_message_textChanged(const QString &arg1)
{
    ui->lineEdit_message->setText(arg1.toUpper());
}
