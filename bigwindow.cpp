#include "bigwindow.h"
#include "ui_bigwindow.h"
#include "ecc.h"


BigWindow::BigWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BigWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    ecc_big = new ECC_BIG();
    generate_equation();
    generatePrivateKeys();
    generatePublicKeys();


    mpi numberB; mpi_init(&numberB);
    mpi numberP; mpi_init(&numberP);

    numberB = stringToMPI("18958286285566608000408668544493926415504680968679321075787234672564");
    numberP = stringToMPI("26959946667150639794667015087019630673557916260026308143510066298881");
  //  ecc_big->setB(stringToMPI("3"));
  // ecc_big->setP(stringToMPI("1063"));


    /*number = stringToMPI(private_key_a);

    text = mpiToString(number);

    qDebug() << text;*/

    // creating base point
    MyPoint *myPoint = new MyPoint();
    mpi base_x, base_y;
    mpi_init(&base_x);mpi_init(&base_y);

    base_x = stringToMPI(ui->lineEdit_base_x->text());
    base_y = stringToMPI(ui->lineEdit_base_y->text());

   // base_x = stringToMPI("11838696407187388799350957250141035264678915751356546206913969278886");
  //  base_y = stringToMPI("2966624012289393637077209076615926844583158638456025172915528198331");

    // 6455442420784385171892731890321939130302256445452508665571987526
    // 11713622093973467124672938316960118024378453519425273321965577354050
    myPoint->setX(base_x);
    myPoint->setY(base_y);

    MyPoint *myPoint0 = new MyPoint();
    MyPoint *myPoint1 = new MyPoint();
    myPoint0->setX(base_x);
    myPoint0->setY(base_y);
    //ecc_big->setBasePoint(myPoint);

    mpi k; mpi_init(&k);
    k = ecc_big->generatePrivateKey();
    myPoint1 = ecc_big->encryptPointFast(myPoint, k);


   // myPoint0 = ecc_big->addPoints(myPoint1, myPoint);
  //  myPoint0 = ecc_big->addPoints(myPoint0, myPoint);

  //  myPoint0 = ecc_big->addDouble(myPoint1);




   /* qDebug() << "k " << mpiToString(k);


    qDebug() << "myPoint1.X " << mpiToString(myPoint1->X());
    qDebug() << "myPoint1.Y " << mpiToString(myPoint1->Y());*/

   /* qDebug() << "myPoint0.X " << mpiToString(myPoint0->X());
    qDebug() << "myPoint0.Y " << mpiToString(myPoint0->Y());*/

   /* ECC *ecc_stand = new ECC();
    ecc_stand->setA(-3);
    ecc_stand->setB(3);
    ecc_stand->setP(1063);

    QPoint *point = ecc_stand->addDouble(new QPoint(15, 1051));
    qDebug() << "X " << point->x();
    qDebug() << "Y " << point->y();


    point = ecc_stand->addDouble(point);
    qDebug() << "X " << point->x();
    qDebug() << "Y " << point->y();*/


  /*  qDebug() << "numberB " << mpiToString(numberB);
    qDebug() << "numberP " << mpiToString(numberP);
    qDebug() << "base_x " << mpiToString(base_x);
    qDebug() << "base_y " << mpiToString(base_y);
*/

  /*  base_x = myPoint1->X();
    base_y = myPoint1->Y();
    mpi t0, t1, t3;
    mpi_init(&t0);mpi_init(&t1);mpi_init(&t3);


    //y^2 = x^3 -3X + B
    mpi_mul_mpi(&t3, &base_y, &base_y);
    mpi_mod_mpi(&t3, &t3, &numberP);

    mpi_mul_mpi(&t0, &base_x, &base_x);
    mpi_mul_mpi(&t0, &t0, &base_x);

    mpi_mul_negative(&t1, &base_x, -3);//t1<- 3*x
    //qDebug() << "t1 " << mpiToString(t1);

    mpi_add_mpi(&t0, &t0, &numberB);
    mpi_add_mpi(&t0, &t0, &t1);

    mpi_mod_mpi(&t0, &t0, &numberP);

    qDebug() << "t0 " << mpiToString(t0);
    qDebug() << "t3 " << mpiToString(t3);*/



}

BigWindow::~BigWindow()
{
    delete ui;
}

void BigWindow::generate_equation()
{
    int a = ui->lineEdit_a->text().toInt();
    QString equation = "Y^2 = X^3 ";
    if( a>0)  equation += " +" +ui->lineEdit_a->text() +"X ";
    else equation += " " +ui->lineEdit_a->text() +"X ";

    equation += " +" +ui->lineEdit_b->text();

    equation += "\n       Z/"+ui->lineEdit_p->text()+"Z";

    ui->label_ec_equation->setText(equation);

    MyPoint *myPoint = new MyPoint();
    mpi base_x, base_y;
    mpi_init(&base_x);mpi_init(&base_y);

    base_x = stringToMPI(ui->lineEdit_base_x->text());
    base_y = stringToMPI(ui->lineEdit_base_y->text());
    myPoint->setX(base_x);
    myPoint->setY(base_y);



    ecc_big->setA(ui->lineEdit_a->text().toInt());
    ecc_big->setB(stringToMPI(ui->lineEdit_b->text()));
    ecc_big->setP(stringToMPI(ui->lineEdit_p->text()));
    ecc_big->setBasePoint(myPoint);
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

    for (int i = 0; i < k; i++){
        buff[i] = '\n';
    }

    for (int i = 0; i < text.size(); i++){
        buff[i] = text.at(i).toLatin1();
    }
    mpi_read_string( &number, 10, buff);
    return number;
}

QString BigWindow::mpiToString(mpi number)
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

void BigWindow::generatePrivateKeys()
{
    mpi_init(&private_key_a);mpi_init(&private_key_b);
    private_key_a = ecc_big->generatePrivateKey();
    private_key_b = ecc_big->generatePrivateKey();

   // qDebug() << "private_key_a " << mpiToString(private_key_a);
   // qDebug() << "private_key_b " << mpiToString(private_key_b);

    ui->textEdit_private_key_alice->setText(mpiToString(private_key_a));
    ui->textEdit_private_key_bob->setText(mpiToString(private_key_b));
}

void BigWindow::generatePublicKeys()
{
    public_key_a = ecc_big->encryptPointFast(ecc_big->getBasePoint(), private_key_a);
    public_key_b = ecc_big->encryptPointFast(ecc_big->getBasePoint(), private_key_b);

    ui->textEdit_public_key_alice->setText(public_key_a->toString());
    ui->textEdit_public_key_bob->setText(public_key_b->toString());
}

void BigWindow::on_button_encode_message_clicked()
{
    ui->textEdit_encoded_message->clear();
    ui->textEdit_encrypted_message->clear();
    ui->textEdit_cm_message->clear();
    ui->textEdit_decrypted_message->clear();

    QString message = ui->lineEdit_message->text();

    QList<MyPoint*> *list = ecc_big->textToPoints(message.toLower());

    for (int i = 0; i < list->size(); i++){
        MyPoint *p = list->at(i);
        QString encodedPoint = p->toString();
        ui->textEdit_encoded_message->append(encodedPoint);
        break;
    }
}
