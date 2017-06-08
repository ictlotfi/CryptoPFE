#include "dialogplot.h"
#include "ui_dialogplot.h"

DialogPlot::DialogPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlot)
{
    ui->setupUi(this);
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
                ui->textEdit_points->append("("+QString::number(x)+", "+QString::number(y)+")");
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
    drawCurveR();
    return true;
}

bool DialogPlot::drawCurveR()
{
    int nb_points = 20000;
    int x_limit = 20;
    int y_limit = 40;

    QVector<QCPCurveData> data1(nb_points);
    QVector<QCPCurveData> data2(nb_points);

    for (int i = 0; i < nb_points; i++){
        double x = (i * x_limit ) / (double)nb_points -x_limit/2;
        double y = x*x*x +a*x +b;
        y = qSqrt(y);

        data1[i] = QCPCurveData(i, x, y);
        data2[i] = QCPCurveData(i, x, -y);
    }

    QCPCurve *c1 = new QCPCurve(ui->customPlot_R->xAxis, ui->customPlot_R->yAxis);
    QCPCurve *c2 = new QCPCurve(ui->customPlot_R->xAxis, ui->customPlot_R->yAxis);
    c1->data()->set(data1, true);
    c2->data()->set(data2, true);

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->rescaleAxes();

    ui->customPlot_R->xAxis->setLabel("x");
    ui->customPlot_R->yAxis->setLabel("y");

    ui->customPlot_R->xAxis->setRange(-x_limit/2, x_limit/2);
    ui->customPlot_R->yAxis->setRange(-y_limit/2, y_limit/2);
}
