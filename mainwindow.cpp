
#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <signalwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SignalWindow *signalWindow = new SignalWindow();
    signalWindow->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
