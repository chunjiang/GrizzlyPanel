#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grizzlypp.h"
#include "libgrizzly.h"
#include <cstdio>
#include <cstdlib>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    unsigned char all_addr[10];
    int num_grizzly = Grizzly::get_all_addr(all_addr);
    char name[5];
    name[0] = '0';
    name[1] = 'x';
    name[4] = 0;
    for(int i = 0; i < num_grizzly; i++) {
        int_to_hex(all_addr[i], name + 2);
        ui->address_dropdown->addItem(name);
        all_grizzly[i] = new Grizzly(all_addr[i]);
    }
    if (num_grizzly > 0) {
        current = all_grizzly[0];
    } else {
        current = NULL;
    }
    timer = new QTimer(parent);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(show_feedback()));
    timer->start(250);
    unitconv = 1;
    ui->encratio->setText(QString::number(unitconv));

    if (current != NULL) {
        last_ticks = current->read_encoder();
    } else {
        last_ticks = 0;
    }

    pidmode = CMODE_NO_PID;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_address_dropdown_currentIndexChanged(int index)
{
    current = all_grizzly[index];
    //updateForm();
}

void MainWindow::show_feedback() {
    if (current == NULL) {
        return;
    }

    if (!ui->currentlimit->hasFocus()) {
        float amplimit = grizzly_adc_to_amps(current->read_as_int(ADDR_CURRENTLIMIT, 2) + 511);
        ui->currentlimit->setText(QString::number(amplimit));
    }

    if (!ui->accellimit->hasFocus()) {
        int acclimit = current->read_as_int(ADDR_ACCELLIMIT, 1);
        ui->accellimit->setText(QString::number(acclimit));
    }

    if (!ui->throttletarget->hasFocus()) {
        ui->throttletarget->setValue(current->read_target());
    }

    char mode = current->read_single_register(ADDR_MODE_RO);
    if (!ui->isenabled->hasFocus()) {
        ui->isenabled->setChecked((mode & 0x01) == 1);
    }

    if (!ui->pidcontrol->hasFocus()) {
        ui->pidcontrol->setChecked((mode & 0x06) == CMODE_POSITION_PID);
    }

    if (!(ui->pslider->hasFocus() || ui->islider->hasFocus() || ui->dslider->hasFocus() || ui->pbox->hasFocus() || ui->ibox->hasFocus() || ui->dbox->hasFocus())) {
        float constants[3];
        current->read_pid_constants(constants);

        ui->pbox->setValue(constants[0]);
        ui->ibox->setValue(constants[1]);
        ui->dbox->setValue(constants[2]);
    }

    float amps = current->read_current();
    ui->currentreading->setText(QString::number(amps));

    int count = current->read_encoder();
    ui->displacement->setText(QString::number(count * unitconv));

    ui->encspeed->setText(QString::number((count - last_ticks) * unitconv * 4));
    last_ticks = count;
}

void MainWindow::on_currentlimit_returnPressed()
{
    if (current == NULL) {
        return;
    }

    current->limit_current(ui->currentlimit->text().toFloat());
    // need to convert current limit reading from adc values to amps
    ui->currentlimit->setText(QString::number(grizzly_adc_to_amps(current->read_as_int(ADDR_CURRENTLIMIT, 2) + 511)));
}

void MainWindow::on_accellimit_returnPressed()
{
    if (current == NULL) {
        return;
    }

    current->limit_acceleration(ui->accellimit->text().toInt());
    ui->accellimit->setText(QString::number(current->read_as_int(ADDR_ACCELLIMIT, 1)));
}

void MainWindow::on_throttleslider_valueChanged(int value)
{
    ui->throttletarget->setValue(value / 100.0);
}

void MainWindow::on_throttletarget_valueChanged(double throttle)
{
    if (current == NULL) {
        return;
    }

    if (pidmode == CMODE_NO_PID) {
        current->set_target(throttle);
    }

    ui->throttleslider->setValue(throttle * 100);
}

void MainWindow::on_throttlemax_valueChanged(int maxval)
{
    ui->throttletarget->setMaximum(maxval);
}

void MainWindow::on_throttlemin_valueChanged(int minval)
{
    ui->throttletarget->setMinimum(minval);
}

void MainWindow::on_isenabled_clicked(bool checked)
{
    if (current == NULL) {
        ui->isenabled->setChecked(false);
        return;
    }
    if (checked) {
        current->enable();
    } else {
        current->disable();
    }
    ui->isenabled->setChecked(checked == 1);
}

void MainWindow::on_encratio_returnPressed()
{
    unitconv = ui->encratio->text().toDouble();
}

void MainWindow::on_encreset_clicked()
{
    current->write_encoder(0);
}

void MainWindow::on_pmax_valueChanged(double val)
{
    ui->pslider->setMaximum(val * 100);
}

void MainWindow::on_imax_valueChanged(double val)
{
    ui->islider->setMaximum(val * 100);
}

void MainWindow::on_dmax_valueChanged(double val)
{
    ui->dslider->setMaximum(val * 100);
}

void MainWindow::on_pmin_valueChanged(double val)
{
    ui->pslider->setMinimum(val * 100);
}

void MainWindow::on_imin_valueChanged(double val)
{
    ui->islider->setMinimum(val * 100);
}

void MainWindow::on_dmin_valueChanged(double val)
{
    ui->dslider->setMinimum(val * 100);
}

void MainWindow::on_pslider_valueChanged(int value)
{
    ui->pbox->setValue(value / 100.0);
}

void MainWindow::on_islider_valueChanged(int value)
{
    ui->ibox->setValue(value / 100.0);
}

void MainWindow::on_dslider_valueChanged(int value)
{
    ui->dbox->setValue(value / 100.0);
}

void MainWindow::on_pbox_valueChanged(double val)
{
    current->write_as_int(ADDR_PCONSTANT, float_to_fixed(val), 4);
    ui->pslider->setValue(val * 100);
}

void MainWindow::on_ibox_valueChanged(double val)
{
    current->write_as_int(ADDR_ICONSTANT, float_to_fixed(val), 4);
    ui->islider->setValue(val * 100);
}

void MainWindow::on_dbox_valueChanged(double val)
{
    current->write_as_int(ADDR_DCONSTANT, float_to_fixed(val), 4);
    ui->dslider->setValue(val * 100);
}

void MainWindow::on_targetslider_valueChanged(int value)
{
    ui->targetbox->setValue(value / 100.0);
}

void MainWindow::on_targetbox_valueChanged(double val)
{
    if (pidmode != CMODE_NO_PID) {
        current->set_target(val);
    }
    ui->targetslider->setValue(val * 100);
}

void MainWindow::on_targetmax_valueChanged(double val)
{
    ui->targetslider->setMaximum(val * 100);
}

void MainWindow::on_targetmin_valueChanged(double val)
{
    ui->targetslider->setMinimum(val * 100);
}

void MainWindow::on_pidcontrol_clicked(bool checked)
{
    if (checked) {
        pidmode = CMODE_POSITION_PID;
    } else {
        pidmode = CMODE_NO_PID;
    }
    current->set_mode(pidmode, DMODE_DRIVE_COAST);
}
