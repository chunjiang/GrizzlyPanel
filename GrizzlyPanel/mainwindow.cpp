#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grizzlypp/src/grizzlypp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->address_dropdown->addItem("0x0f");
    ui->address_dropdown->addItem("0x0c");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_8_valueChanged(int arg1)
{

}
