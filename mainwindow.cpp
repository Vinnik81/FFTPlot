#include "filewriterworkerfft.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FileWriterWorkerFFT *worker = new FileWriterWorkerFFT("testFFT.dat", this);
    worker->printPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

