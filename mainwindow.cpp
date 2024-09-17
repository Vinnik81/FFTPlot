#include "filewriterworkerfft.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    worker = new FileWriterWorkerFFT("testFFT.dat", this);
    //worker->printPlot();
    customPlotSignal = createPlot();

    QVBoxLayout *l = new QVBoxLayout();
    ui->centralwidget->setLayout(l);
    l->addWidget(customPlotSignal);

    QHBoxLayout *lh = new QHBoxLayout();
    labelFs = new QLabel(this);
    labelFs->setText("fs: ");
    lineEditFs = new QLineEdit(this);
    lineEditFs->setText("48000.0");
    labelT = new QLabel(this);
    labelT->setText("t: ");
    lineEditT = new QLineEdit("1.0", this);
    labelA = new QLabel(this);
    labelA->setText("a: ");
    lineEditA = new QLineEdit("10.0", this);
    labelF = new QLabel(this);
    labelF->setText("f: ");
    lineEditF = new QLineEdit("80.0", this);

    generateButton = new QPushButton(this);
    generateButton->setText("Generate");

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generate);

    customPlotSpectrum = createPlot();
    l->addWidget(customPlotSpectrum);
    lh->addWidget(labelFs);
    lh->addWidget(lineEditFs);
    lh->addWidget(labelT);
    lh->addWidget(lineEditT);
    lh->addWidget(labelA);
    lh->addWidget(lineEditA);
    lh->addWidget(labelF);
    lh->addWidget(lineEditF);
    l->addItem(lh);
    l->addWidget(generateButton);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QCustomPlot *MainWindow::createPlot()
{
    QCustomPlot *customPlot = new QCustomPlot(this);
    // add two new graphs and set their look:
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
//    customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    // generate some points of data (y0 for first, y1 for second graph):

    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    return customPlot;
}

void MainWindow::generate()
{
    double fs = lineEditFs->text().toDouble();
    double f = lineEditF->text().toDouble();
    worker->FFT(fs, lineEditT->text().toDouble(), lineEditA->text().toDouble(), f);

    int s = (ceil(fs/f) + 1) * 2;
    QVector<double> x(s), y0(s);
    int j = 0;
    for (double i : worker->pValues()->keys()) {
        double value = (*worker->pValues())[i];
        x.push_back(i);
        y0.push_back(worker->getSignal()[j++]);
        if (j >= s) {
            break;
//            qDebug() << i << value;
        }
    }
    customPlotSignal->graph(0)->setData(x, y0);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSignal->graph(0)->rescaleAxes();
    customPlotSignal->replot();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    int k = 0;
    int ss = (ceil(f) + 1) * 2;
    QVector<double> x1(ss), y1(ss);
    for (double i : worker->pValues()->keys()) {
        double value = (*worker->pValues())[i];
        x1.push_back(i);
        y1.push_back(value);
        k++;
        if (k >= ss) {
            break;
        }
        qDebug() << i << value;

    }

    customPlotSpectrum->graph(0)->setData(x1, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSpectrum->graph(0)->rescaleAxes();
    customPlotSpectrum->replot();
}

