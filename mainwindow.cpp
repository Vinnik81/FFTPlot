
#include "mainwindow.h"

#include "filewriterworkerdisplacement.h"
#include "filewriterworkerfft.h"
#include "filewriterworkervelocity.h"
#include "signalfunctionsum.h"
#include "ui_mainwindow.h"

#include <signalwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    SignalWindow *signalWindow = new SignalWindow();
//    signalWindow->show();
    signalFunction = new SignalFunctionSum();
//    worker = new FileWriterWorkerFFT(signalFunction, "testFFT.dat", this);
    //worker->printPlot();
    customPlotSignal = createPlot(tr("Time, s"), tr("Magnitude"));

    QVBoxLayout *l = new QVBoxLayout();
    ui->centralwidget->setLayout(l);
    //setLayout(l);
    setMinimumSize(828, 743);

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

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generate);

    customPlotSpectrum = createPlot(tr("Frequency, Hz"), tr("Vibro Acceleration, m/s²"));
    vibroVelocity = createPlot(tr("Frequency, Hz"), tr("Vibro Velocity, mm/s"));
    vibroDisplacement = createPlot(tr("Frequency, Hz"), tr("Vibro Displacement, mkm"));

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

MainWindow::~MainWindow()
{
    delete ui;
}

QCustomPlot *MainWindow::createPlot(const QString &xLabel, const QString &yLabel)
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
    customPlot->xAxis->setLabel(xLabel);
    customPlot->yAxis->setLabel(yLabel);
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
    FileWriterWorkerFFT *worker = new FileWriterWorkerFFT(signalFunction, "plotfft.dat", this);
    FileWriterWorkerVelocity *workerVelocity = new FileWriterWorkerVelocity(signalFunction, "plotvelocity.dat", this);
    FileWriterWorkerDisplacement *workerDisplacement = new FileWriterWorkerDisplacement(signalFunction, "plotdisplacement.dat", this);
    double fs = lineEditFs->value();
    double f = lineEditF->value();
    double f2 = lineEditF2->value();
    double t =  lineEditT->value();
    double scale = lineEditScale->value() / 100.0;
    worker->signalFunction()->setA(lineEditA->value());
    worker->signalFunction()->setF(f);
    workerVelocity->signalFunction()->setA(lineEditA->value());
    workerVelocity->signalFunction()->setF(f);
    workerDisplacement->signalFunction()->setA(lineEditA->value());
    workerDisplacement->signalFunction()->setF(f);
    static_cast<SignalFunctionSum*>(signalFunction)->setA2(lineEditA2->value());
    static_cast<SignalFunctionSum*>(signalFunction)->setF2(f2);

    worker->FFT(fs, t);
    workerVelocity->FFT(fs, t);
    workerDisplacement->FFT(fs, t);

    double freqMax = worker->getFreqMax();
    double freqMaxVelocity = workerVelocity->getFreqMax();
    double freqMaxDisplacement = workerDisplacement->getFreqMax();

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
    double freqMaxVelocity2 = ceil(freqMaxVelocity) * 2;
    double freqMaxDisplacement2 = ceil(freqMaxDisplacement) * 2;

    int ss = worker->vibroAcceleration().size();
    int ssVelocity = workerVelocity->vibroVelocity().size();
    int ssDisplacement = workerDisplacement->vibroDisplacement().size();
    QVector<double> x1(ss), y1(ss), x2(ssVelocity), y2(ssVelocity), x3(ssDisplacement), y3(ssDisplacement);
    for (int i = 0; i < ss; ++i) {
        x1[i] = worker->vibroAcceleration()[i].x();
        if (x1[i] > freqMax2) {
            break;
        }
        double value = worker->vibroAcceleration()[i].y();

        y1[i] = value;
//        qDebug() << i << value;

    }

    for (int i = 0; i < ssVelocity; ++i) {
        x2[i] = worker->vibroVelocity()[i].x();
        if (x2[i] > freqMaxVelocity2) {
            break;
        }
        double value = workerVelocity->vibroVelocity()[i].y();

        y2[i] = value * 1000;
//        qDebug() << i << value;

    }

    for (int i = 0; i < ssDisplacement; ++i) {
        x3[i] = workerDisplacement->vibroDisplacement()[i].x();
        if (x3[i] > freqMaxDisplacement2) {
            break;
        }
        double value = workerDisplacement->vibroDisplacement()[i].y();

        y3[i] = value * 1000000;
//        qDebug() << i << value;

    }

    customPlotSpectrum->graph(0)->setData(x1, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlotSpectrum->graph(0)->rescaleAxes();
    customPlotSpectrum->yAxis->setRange(0, 1.05 * (worker->getAccelerationMax()));

   vibroVelocity->graph(0)->setData(x2, y2);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    vibroVelocity->graph(0)->rescaleAxes();
    vibroVelocity->yAxis->setRange(0, 1.05 * (workerVelocity->getVelocityMax() * 1000));

    vibroDisplacement->graph(0)->setData(x3, y3);
    vibroDisplacement->graph(0)->rescaleAxes();
    vibroDisplacement->yAxis->setRange(0, 1.05 * (workerDisplacement->getDisplacementMax() * 1000000));

    if (m_pTracerAcceleration) customPlotSpectrum->removeItem(m_pTracerAcceleration);
    m_pTracerAcceleration = createTracer(customPlotSpectrum, worker->getAccelerationFreq(), tr("Hz"), worker->getAccelerationMax(), tr("m/s²"));

    if (m_pTracerVelocity) vibroVelocity->removeItem(m_pTracerVelocity);
    m_pTracerVelocity = createTracer(vibroVelocity, workerVelocity->getVelocityFreq(), tr("Hz"), workerVelocity->getVelocityMax() * 1000, tr("mm/s"));

    if (m_pTracerDisplacement) vibroDisplacement->removeItem(m_pTracerDisplacement);
    m_pTracerDisplacement = createTracer(vibroDisplacement, workerDisplacement->getDisplacementFreq(), tr("Hz"), workerDisplacement->getDisplacementMax() * 1000000, tr("mkm"));

    customPlotSpectrum->replot();
    vibroVelocity->replot();
    vibroDisplacement->replot();
}

QDoubleSpinBox *MainWindow::createEdit()
{
    QDoubleSpinBox *lineEdit = new QDoubleSpinBox(this);

    lineEdit->setMaximum(100000.0);
    lineEdit->setMinimum(0.0);
    lineEdit->setSingleStep(0.1);

    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        return lineEdit;
}

QCPItemTracer *MainWindow::createTracer(QCustomPlot *customPlot, double graphKey, QString keyUnit, double graphValue, QString valueUnit)
{
    QCPItemTracer *phaseTracer = new QCPItemTracer(customPlot);
    phaseTracer->setGraph(customPlot->graph(0));
    phaseTracer->setGraphKey(graphKey);
    phaseTracer->setInterpolating(true);
    phaseTracer->setStyle(QCPItemTracer::tsCircle);
    phaseTracer->setPen(QPen(Qt::red));
    phaseTracer->setBrush(Qt::red);
    phaseTracer->setSize(7);

    // add label for phase tracer:
    QCPItemText *phaseTracerText = new QCPItemText(customPlot);
    phaseTracerText->position->setParentAnchor(phaseTracer->position);
    phaseTracerText->setPositionAlignment(Qt::AlignLeft|Qt::AlignTop);
    phaseTracerText->position->setCoords(10.0, 0.0); // lower right corner of axis rect
    phaseTracerText->setText(QString("%1 %2, %3 %4").arg(graphKey).arg(keyUnit).arg(graphValue).arg(valueUnit));
    return phaseTracer;
}
