#include "dialogplot.h"
#include "ui_dialogplot.h"

DialogPlot::DialogPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlot)
{
    ui->setupUi(this);


    int a = -1;
    int b = 3;
    int p = 127;


    QList<QPoint *> *list = new QList<QPoint *>();
    for (int x = 0; x < p; x++) {
        long yy = x*x*x +a*x +b;
        for (int y = 0; y < p; y++) {
            int k = y * y;
            if (k % p == yy % p) {
                list->append(new QPoint(x, y));
            }
        }
    }


    QVector<double> x0(list->size()), y0(list->size());

    for (int i = 0; i < list->size(); i++){
        x0[i] = list->at(i)->x();
        y0[i] = list->at(i)->y();
    }

    // generate some data:
   /* for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }*/
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x0, y0);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 130);
    ui->customPlot->yAxis->setRange(0, 130);
    ui->customPlot->replot();
}

DialogPlot::~DialogPlot()
{
    delete ui;
}

void DialogPlot::setA(int a)
{
    this->a =a;
}

void DialogPlot::setB(int b)
{
    this->b =b;
}

void DialogPlot::setP(int p)
{
    this->p = p;
}

void DialogPlot::setR(int r)
{
    this->r = r;
}

bool DialogPlot::drawCurve()
{
    if (p > 1000 || p < 5) return false;

    QList<QPoint *> *list = new QList<QPoint *>();
    for (int x = 0; x < p; x++) {
        long yy = x*x*x +a*x +b;
        for (int y = 0; y < p; y++) {
            int k = y * y;
            if (k % p == yy % p) {
                list->append(new QPoint(x, y));
            }
        }
    }
    ui->label_nb_points->setText("Nombre de points: "+QString::number(list->size()+1));

    QVector<double> x0(list->size()), y0(list->size());

    for (int i = 0; i < list->size(); i++){
        x0[i] = list->at(i)->x();
        y0[i] = list->at(i)->y();
    }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x0, y0);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, p);
    ui->customPlot->yAxis->setRange(0, p);
    ui->customPlot->replot();


    QString equation = "Y^2 = X^3 ";
    if( a>0)  equation += " +" +QString::number(a) +"X ";
    else equation += " " +QString::number(a) +"X ";

    equation += " +" +QString::number(b);

    equation += "\n       Z/"+QString::number(p)+"Z";

    ui->label_ec_equation->setText(equation);
    return true;
}
