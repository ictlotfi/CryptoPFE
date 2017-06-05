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
    generateRandomK();


    QRegExp validatorAlphaNumeric("[A-Za-z0-9]*");
    ui->lineEdit_message_alice->setValidator(new QRegExpValidator(validatorAlphaNumeric, ui->lineEdit_message_alice));
    ui->lineEdit_message_bob->setValidator(new QRegExpValidator(validatorAlphaNumeric, ui->lineEdit_message_bob));


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
                || buff[i] == '6' || buff[i] == '7' || buff[i] == '8' || buff[i] == '9' || buff[i] == '-')  text += buff[i];
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

void BigWindow::generateRandomK()
{
    mpi_init(&k_a);mpi_init(&k_b);
    k_a = ecc_big->generatePrivateKey();
    k_b = ecc_big->generatePrivateKey();
}

QList<MyCM *> BigWindow::textToMyCMList(QString text)
{
    QList<MyCM *> myCMList = QList<MyCM *>();
    QStringList list = text.split("#");
    for (int i = 0; i < list.size(); i++){
        //qDebug() << list.at(i);
        MyCM *myCM = stringToMyCM(list.at(i));
        //qDebug() << myCM->toString();
        myCMList.append(myCM);;
    }
    return myCMList;
}

MyCM *BigWindow::stringToMyCM(QString arg0)
{
    MyPoint *p1 = new MyPoint();
    MyPoint *p2 = new MyPoint();
    int counter = 0;
    QString temp = "";
    QString arg = arg0;
    arg.replace(" ", "");
    arg.replace("\n", "");
    arg.replace("#", "");


    if (arg.startsWith("{") && arg.endsWith("}")){
        arg.replace("{", "");
        arg.replace("}", "");
        arg.replace("(", "");
        arg.replace(")", "");


        for (int i = 0; i < arg.size(); i++){
            if (arg.at(i) != ',') temp+= arg.at(i);
            else {
                if (counter == 0){
                    p1->setX(stringToMPI(temp));
                    temp = "";
                }
                else if (counter == 1){
                    p1->setY(stringToMPI(temp));
                    temp = "";
                }
                else if (counter == 2){
                    p2->setX(stringToMPI(temp));
                    temp = "";
                }

                counter++;
            }
        }
        p2->setY(stringToMPI(temp));

    }
    return new MyCM(p1, p2);
}

QString BigWindow::myCMListToString(QList<MyCM *> list)
{
    QString temp = "";
    for (int i=0; i < list.size(); i++){
        temp += list.at(i)->toString();
        if (i != list.size()-1) temp+= "#";

    }
    return temp;
}

void BigWindow::on_button_encode_message_alice_clicked()
{
    ui->textEdit_encrypted_message_alice->clear();
    ui->textEdit_encrypted_message_received_bob->clear();

    QString message = ui->lineEdit_message_alice->text();

    QList<MyPoint*> *list = ecc_big->textToPoints(message.toLower());
    QString temp = "";
    for (int i = 0; i < list->size(); i++){
        MyPoint *p = list->at(i);

        // generate CM
        MyCM *cm = ecc_big->generateCm(k_a, p, public_key_b);
        temp = cm->toString();
        if (i != list->size()-1) temp +="#";


        qDebug() << "origin: "<< p->toString();
        ui->textEdit_encrypted_message_alice->append(temp);
        ui->textEdit_encrypted_message_received_bob->append(temp);
    }
}

void BigWindow::button_decrypt_message_alice()
{

}

void BigWindow::on_button_encode_message_bob_clicked()
{
    ui->textEdit_encrypted_message_bob->clear();
    ui->textEdit_encrypted_message_received_alice->clear();

    QString message = ui->lineEdit_message_bob->text();

    QList<MyPoint*> *list = ecc_big->textToPoints(message.toLower());
    QString temp = "";
    for (int i = 0; i < list->size(); i++){
        MyPoint *p = list->at(i);

        // generate CM
        MyCM *cm = ecc_big->generateCm(k_b, p, public_key_a);
        temp = cm->toString();
        if (i != list->size()-1) temp +="#";


        qDebug() << "origin: "<< p->toString();
        ui->textEdit_encrypted_message_bob->append(temp);
        ui->textEdit_encrypted_message_received_alice->append(temp);
    }
}

void BigWindow::on_button_decrypt_message_bob_clicked()
{
    QString text = ui->textEdit_encrypted_message_received_bob->toPlainText();
    QList<MyCM *> list = textToMyCMList(text);
    QList<MyPoint*> *listDecrypted = new QList<MyPoint*>();
    ui->lineEdit_message_received_bob->clear();

    //decrypt
    for (int i = 0; i <list.size(); i++){
        MyPoint *p1 = list.at(i)->getP1();
        MyPoint *p2 = list.at(i)->getP2();

        MyPoint *pp1 = ecc_big->encryptPointFast(p1, private_key_b);


        mpi temp_y; mpi_init(&temp_y);
        temp_y = pp1->Y();
        mpi_mul_negative(&temp_y, &temp_y, -1);

        pp1->setY(temp_y);

        MyPoint *pp2 = ecc_big->addPoints(p2, pp1);
        listDecrypted->append(pp2);
    }

    // point to alphabet
    ui->lineEdit_message_received_bob->setText(ecc_big->pointsToText(listDecrypted));
}

void BigWindow::on_lineEdit_message_bob_textChanged(const QString &arg1)
{
    ui->lineEdit_message_bob->setText(arg1.toUpper());
}

void BigWindow::on_button_decrypt_message_alice_clicked()
{
    QString text = ui->textEdit_encrypted_message_received_alice->toPlainText();
    QList<MyCM *> list = textToMyCMList(text);
    QList<MyPoint*> *listDecrypted = new QList<MyPoint*>();
    ui->lineEdit_message_received_alice->clear();

    //decrypt
    for (int i = 0; i <list.size(); i++){
        MyPoint *p1 = list.at(i)->getP1();
        MyPoint *p2 = list.at(i)->getP2();

        MyPoint *pp1 = ecc_big->encryptPointFast(p1, private_key_a);


        mpi temp_y; mpi_init(&temp_y);
        temp_y = pp1->Y();
        mpi_mul_negative(&temp_y, &temp_y, -1);

        pp1->setY(temp_y);

        MyPoint *pp2 = ecc_big->addPoints(p2, pp1);
        listDecrypted->append(pp2);
    }
    // point to alphabet
    ui->lineEdit_message_received_alice->setText(ecc_big->pointsToText(listDecrypted));
}

void BigWindow::on_lineEdit_message_alice_textChanged(const QString &arg1)
{
    ui->lineEdit_message_alice->setText(arg1.toUpper());
}
