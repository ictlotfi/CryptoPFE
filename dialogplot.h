#ifndef DIALOGPLOT_H
#define DIALOGPLOT_H

#include <QDialog>

namespace Ui {
class DialogPlot;
}

class DialogPlot : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlot(QWidget *parent = 0);
    ~DialogPlot();

private:
    Ui::DialogPlot *ui;
};

#endif // DIALOGPLOT_H
