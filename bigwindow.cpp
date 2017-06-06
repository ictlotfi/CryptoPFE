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


    QRegExp validatorAlphaNumeric("[A-Za-z0-9 ]*");
    ui->lineEdit_message_alice->setValidator(new QRegExpValidator(validatorAlphaNumeric, ui->lineEdit_message_alice));
    ui->lineEdit_message_bob->setValidator(new QRegExpValidator(validatorAlphaNumeric, ui->lineEdit_message_bob));

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
    ecc_big->setR(stringToMPI(ui->lineEdit_r->text()));
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

bool BigWindow::isOnCurve(int x, int y, int a, int b, int p)
{
    int k = y * y;
    int m = (x * x * x) + a * x + b;
    if (k % p == m % p) {
        return true;
    }
    return false;
}

QPoint *BigWindow::generatePoint(int a, int b, int p)
{
    for (int x = 0; x < p; x++) {
        for (int y = 0; y < p; y++) {
            int k = y * y;
            int m = (x * x * x) + a * x + b;
            if (k % p == m % p) {
                return new QPoint(x, y);
            }
        }
    }
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


       // qDebug() << "origin: "<< p->toString();
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

void BigWindow::on_actionPlot_triggered()
{
    DialogPlot d;
    d.setA(ecc_big->getA());
    d.setB(mpiToString(ecc_big->getB()).toInt());
    d.setP(mpiToString(ecc_big->getP()).toInt());
    d.setR(mpiToString(ecc_big->getR()).toInt());
    if (d.drawCurve()) d.exec();
    else {
        QMessageBox msgBox;
        msgBox.setText("La courbe n'est pas supporté");
        msgBox.exec();
    }
}

void BigWindow::on_button_generate_clicked()
{
    int temp_p = ui->lineEdit_p->text().toInt();
    int temp_a = ui->lineEdit_a->text().toInt();
    int temp_b = ui->lineEdit_b->text().toInt();
    MillerRabin *m = new MillerRabin();
    if(!m->isPrime(temp_p) && temp_p>0){
        QMessageBox msgBox;
        msgBox.setText("P n'est pas premier");
        msgBox.exec();

        return;
    }

    if (temp_p > 0){ // p is not a big integer, so generate list of points
        // get x and y and verify if they belong to the curve
        int x = ui->lineEdit_base_x->text().toInt();
        int y = ui->lineEdit_base_y->text().toInt();

        if (x ==0 && y == 0){
            QPoint *p = generatePoint(temp_a, temp_b, temp_p);
            ui->lineEdit_base_x->setText(QString::number(p->x()));
            ui->lineEdit_base_y->setText(QString::number(p->y()));

            // getOrder
            ECC *ecc = new ECC();
            ecc->setA(temp_a);
            ecc->setB(temp_b);
            ecc->setP(temp_p);

            int order = ecc->getPointOrder(p);
            ui->lineEdit_r->setText(QString::number(order));

        }
        else if (!isOnCurve(x, y, temp_a, temp_b, temp_p)){
            QMessageBox msgBox;
            msgBox.setText("Le point de base n'est pas sur la courbe");
            msgBox.exec();

            return;
        }
    }

    ecc_big = new ECC_BIG();
    generate_equation();
    generatePrivateKeys();
    generatePublicKeys();
    generateRandomK();
    ui->lineEdit_message_alice->clear();
    ui->lineEdit_message_bob->clear();
    ui->textEdit_encrypted_message_alice->clear();
    ui->textEdit_encrypted_message_bob->clear();
    ui->textEdit_encrypted_message_received_alice->clear();
    ui->textEdit_encrypted_message_received_bob->clear();
}
