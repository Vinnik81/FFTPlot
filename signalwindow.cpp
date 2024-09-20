#include "signalwindow.h"
#include "filewriterworkerfft.h"
#include "signalfunction.h"
#include "signalfunctionsum.h"

#include <QLabel>

SignalWindow::SignalWindow(QWidget *parent) : QWidget(parent)
{
    signalFunction = new SignalFunctionSum();
    worker = new FileWriterWorkerFFT(signalFunction, "testFFT.dat", this);
    //worker->printPlot();
    customPlotSignal = createPlot();

    QVBoxLayout *l = new QVBoxLayout();
    //ui->centralwidget->setLayout(l);
    setLayout(l);
    setMinimumSize(200, 500);

    QHBoxLayout *lh = new QHBoxLayout();

    labelFs = new QLabel(this);
    labelFs->setText("fs: ");
    lineEditFs = createEdit();
    lineEditFs->setValue(48000.0);
    labelT = new QLabel(this);
    labelT->setText("t: ");
    lineEditT = createEdit();
    lineEditT->setValue(1.0);
    labelA = new QLabel(this);
    labelA->setText("a: ");
    lineEditA = createEdit();
    lineEditA->setValue(10.0);
    labelF = new QLabel(this);
    labelF->setText("f: ");
    lineEditF = createEdit();
    lineEditF->setValue(80.0);
    labelA2 = new QLabel(this);
    labelA2->setText("a2: ");
    lineEditA2 = createEdit();
    lineEditA2->setValue(30.0);
    labelF2 = new QLabel(this);
    labelF2->setText("f2: ");
    lineEditF2 = createEdit();
    lineEditF2->setValue(180.0);
    labelScale = new QLabel(this);
    labelScale->setText("Scale: ");
    lineEditScale = createEdit();
    lineEditScale->setValue(100);

    generateButton = new QPushButton(this);
    generateButton->setText("Generate");

    connect(generateButton, &QPushButton::clicked, this, &SignalWindow::generate);

    customPlotSpectrum = createPlot();
    vibroVelocity = createPlot();
    vibroDisplacement = createPlot();
    l->addItem(lh);
    l->addWidget(generateButton);
    lh->addWidget(labelFs);
    lh->addWidget(lineEditFs);
    lh->addWidget(labelT);
    lh->addWidget(lineEditT);
    lh->addWidget(labelA);
    lh->addWidget(lineEditA);
    lh->addWidget(labelA2);
    lh->addWidget(lineEditA2);
    lh->addWidget(labelF);
    lh->addWidget(lineEditF);
    lh->addWidget(labelF2);
    lh->addWidget(lineEditF2);
    lh->addWidget(labelScale);
    lh->addWidget(lineEditScale);
    l->addWidget(customPlotSignal);
    l->addWidget(customPlotSpectrum);
    l->addWidget(vibroVelocity);
    l->addWidget(vibroDisplacement);
}

QCustomPlot *SignalWindow::createPlot()
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

void SignalWindow::generate()
{
    double fs = lineEditFs->value();
    double f = lineEditF->value();
    double f2 = lineEditF2->value();
    double t =  lineEditT->value();
    double scale = lineEditScale->value() / 100.0;
    signalFunction->setA(lineEditA->value());
    signalFunction->setF(f);
    static_cast<SignalFunctionSum*>(signalFunction)->setA2(lineEditA2->value());
    static_cast<SignalFunctionSum*>(signalFunction)->setF2(f2);
    worker->FFT(fs, t);
    double freqMax = worker->getFreqMax();

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
    double freqMax2 = ceil(freqMax) * 2;
    int ss = worker->vibroAcceleration().size();
    QVector<double> x1(ss), y1(ss), y2(ss), y3(ss);
    for (int i = 0; i < ss; ++i) {
        x1[i] = worker->vibroAcceleration()[i].x();
        if (x1[i] > freqMax2) {
            break;
        }
        double value = worker->vibroAcceleration()[i].y();

        y1[i] = value;
        y2[i] = worker->vibroVelocity()[i].y();
        y3[i] = worker->vibroDisplacement()[i].y();
//        qDebug() << i << value;

    }

    customPlotSpectrum->graph(0)->setData(x1, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSpectrum->graph(0)->rescaleAxes();
   vibroVelocity->graph(0)->setData(x1, y2);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    vibroVelocity->graph(0)->rescaleAxes();
    vibroDisplacement->graph(0)->setData(x1, y3);
    vibroDisplacement->graph(0)->rescaleAxes();
    customPlotSpectrum->replot();
    vibroVelocity->replot();
    vibroDisplacement->replot();
}

QDoubleSpinBox *SignalWindow::createEdit()
{
    QDoubleSpinBox *lineEdit = new QDoubleSpinBox(this);

    lineEdit->setMaximum(100000.0);
    lineEdit->setMinimum(0.0);
    lineEdit->setSingleStep(0.1);

    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        return lineEdit;
}

