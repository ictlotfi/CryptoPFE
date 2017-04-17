#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QPoint>
#include <QDebug>
#include "ecc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void generate_equation();
    void add_double (int xp, int yp, int &xr, int &yr, int a, int p);
    int NegMod (int a, int p);
    void add_points (int xp, int yp, int xq, int yq, int &xr, int &yr, int p);
    static int InvMod(int x, int n) ;
    void scalar_multiplicaiton (int xp, int yp, int k, int a, int p, int &PUx, int &PUy);
    static int EGCD(int a, int b, int& u, int &v);
    QPoint *encrypt(QPoint *p);

private slots:
    void on_lineEdit_p_textChanged(const QString &);

    void on_lineEdit_a_textChanged(const QString &);

    void on_lineEdit_b_textChanged(const QString &);

    void on_button_generate_ec_points_clicked();

    void on_button_select_base_clicked();

    void on_button_generate_public_key_alice_clicked();

    void on_button_select_message_clicked();

    void on_button_generate_cipher_clicked();

    void on_button_generate_public_key_bob_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    ECC *ecc_main;
};

#endif // MAINWINDOW_H
