#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grizzlypp.h"
#include "advanced.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Grizzly* current;
    Grizzly* all_grizzly[10];
    int num_grizzly;
    QTimer* timer;
    double unitconv;
    int last_ticks;
    Advanced* adv;

public:
    explicit MainWindow(QWidget *parent = 0);
    char pidmode;
    ~MainWindow();

private slots:
    void on_address_dropdown_currentIndexChanged(int index);

    void on_currentlimit_returnPressed();

    void on_accellimit_returnPressed();

    void on_throttletarget_valueChanged(double value);

    void on_throttlemax_valueChanged(int value);

    void on_throttlemin_valueChanged(int value);

    void on_isenabled_clicked(bool checked);

    void show_feedback();

    void on_encratio_returnPressed();

    void on_throttleslider_valueChanged(int value);

    void on_pmax_valueChanged(double value);

    void on_imax_valueChanged(double value);

    void on_dmax_valueChanged(double value);

    void on_pmin_valueChanged(double value);

    void on_imin_valueChanged(double value);

    void on_dmin_valueChanged(double value);

    void on_targetmax_valueChanged(double value);

    void on_targetmin_valueChanged(double value);

    void on_targetbox_valueChanged(double value);

    void on_pslider_valueChanged(int value);

    void on_islider_valueChanged(int value);

    void on_dslider_valueChanged(int value);

    void on_targetslider_valueChanged(int value);

    void on_pbox_valueChanged(double arg1);

    void on_ibox_valueChanged(double arg1);

    void on_dbox_valueChanged(double arg1);

    void on_encreset_clicked();

    void on_pidcontrol_clicked(bool checked);

    void on_pushButton_2_clicked();


private:
    Ui::MainWindow *ui;
    void updateForm();
};

#endif // MAINWINDOW_H
