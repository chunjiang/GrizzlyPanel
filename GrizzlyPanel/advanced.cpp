#include "advanced.h"
#include "ui_advanced.h"
#include "libgrizzly.h"
#include "grizzlypp.h"
#include <QTimer>
#include <QDebug>
#include "mainwindow.h"

unsigned char Advanced::drive_mode[3] = {DMODE_DRIVE_COAST, DMODE_DRIVE_BRAKE, DMODE_BRAKE_COAST};
unsigned char Advanced::pid_mode[2] = {CMODE_SPEED_PID, CMODE_POSITION_PID};

Advanced::Advanced(QWidget *parent, Grizzly** curr) :
    QDialog(parent),
    ui(new Ui::Advanced)
{
    current = curr;
    ui->setupUi(this);
    ui->dmodeselect->addItem("Drive/Coast");
    ui->dmodeselect->addItem("Drive/Brake");
    ui->dmodeselect->addItem("Brake/Coast");

    ui->pidmodeselect->addItem("Speed Control");
    ui->pidmodeselect->addItem("Position Control");

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(show_feedback()));
    timer->start(500);
    cmode = CMODE_SPEED_PID;
    dmode = DMODE_DRIVE_COAST;
}

Advanced::~Advanced()
{
    delete timer;
    delete ui;
}

void Advanced::show_feedback() {
    if (!this->isVisible()) {
        return;
    }
    char mode = (*current)->read_single_register(ADDR_MODE_RO);
    qDebug() << QString::number(mode )<< "\n";

    int uptime = (*current)->read_as_int(ADDR_UPTIME, 4);
    ui->uptime->setText(QString::number(uptime));

    int errorcnt = (*current)->read_single_register(0x18);
    ui->errors->setText(QString::number(errorcnt));
}

void Advanced::on_dmodeselect_activated(int index)
{
    dmode = drive_mode[index];
    char mode = (*current)->read_single_register(ADDR_MODE_RO);
    mode &= ~0x30;
    mode |= dmode;
    (*current)->write_single_register(ADDR_MODE, mode);
    (*current)->write_single_register(ADDR_UPDATE, 0);
}

void Advanced::on_pidmodeselect_activated(int index)
{
    /*
    char mode = (*current)->read_single_register(ADDR_MODE_RO);
    mode &= ~0x06;
    mode |= pid_mode[index];
    (*current)->write_single_register(ADDR_MODE, mode);
    (*current)->write_single_register(ADDR_UPDATE, 0);
    */
    cmode = pid_mode[index];
    if (((MainWindow*)parentWidget())->pidmode != CMODE_NO_PID) {
        (*current)->set_mode(cmode, dmode);
    }
}

void Advanced::on_writesend_clicked()
{
    bool ok;
    char addr = ui->writeaddr->text().toInt(&ok, 0) & 0xff;
    int val = ui->writevalue->text().toInt();
    int num = ui->writesize->text().toInt();
    (*current)->write_as_int(addr, val, num);
}

void Advanced::on_readsend_clicked()
{
    bool ok;
    char addr = ui->readaddr->text().toInt(&ok, 0) & 0xff;
    int num = ui->readsize->text().toInt();
    unsigned int rtn = (*current)->read_as_int(addr, num);
    ui->readrtn->setText(QString::number(rtn));
}

void Advanced::on_closebtn_clicked()
{
    this->done(0);
}
