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
    labelScale = new QLabel(this);
    labelScale->setText("Scale: ");
    lineEditScale = new QLineEdit(this);
    lineEditScale->setText("100");

    generateButton = new QPushButton(this);
    generateButton->setText("Generate");

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generate);

    customPlotSpectrum = createPlot();
    l->addItem(lh);
    l->addWidget(generateButton);
    lh->addWidget(labelFs);
    lh->addWidget(lineEditFs);
    lh->addWidget(labelT);
    lh->addWidget(lineEditT);
    lh->addWidget(labelA);
    lh->addWidget(lineEditA);
    lh->addWidget(labelF);
    lh->addWidget(lineEditF);
    lh->addWidget(labelScale);
    lh->addWidget(lineEditScale);
    l->addWidget(customPlotSignal);
    l->addWidget(customPlotSpectrum);
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
    double t =  lineEditT->text().toDouble();
    double scale = lineEditScale->text().toDouble() / 100.0;
    worker->FFT(fs, t, lineEditA->text().toDouble(), f);

//    int s = ceil(fs/f) * 2 + 1;
    int s = ceil(fs * scale);
    QVector<double> x(s), y0(s);
    for (int j = 0; j < s; ++j) {
        double ts = j / fs;
        x[j] = ts;
        y0[j] = worker->getSignal()[j];

//            qDebug() << i << value;
    }
    customPlotSignal->graph(0)->setData(x, y0);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSignal->graph(0)->rescaleAxes();
    customPlotSignal->replot();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    int ss = ceil(f) * 2;
    QVector<double> x1(ss), y1(ss);
    for (int i = 0; i < worker->pValues()->size(); ++i) {
        if (i >= ss) {
            break;
        }
        double value = (*worker->pValues())[i].y();
        x1[i] = (*worker->pValues())[i].x();
        y1[i] = value;
//        qDebug() << i << value;

    }

    customPlotSpectrum->graph(0)->setData(x1, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSpectrum->graph(0)->rescaleAxes();
    customPlotSpectrum->replot();
}

