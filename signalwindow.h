#ifndef SIGNALWINDOW_H
#define SIGNALWINDOW_H

#include <QWindow>
#include "thirdparty/qcustomplot/qcustomplot.h"

class FileWriterWorkerFFT;

class SignalFunction;

class SignalWindow : public QWidget
{
public:
    explicit SignalWindow(QWidget* parent = Q_NULLPTR);

private:
    QCustomPlot *customPlotSignal;
    QCustomPlot *customPlotSpectrum;
    QCustomPlot *vibroVelocity;
    QCustomPlot *vibroDisplacement;
    QLabel *labelFs;
    QLabel *labelT;
    QLabel *labelA;
    QLabel *labelF;
    QLabel *labelA2;
    QLabel *labelF2;
    QLabel *labelScale;
    QDoubleSpinBox *lineEditFs;
    QDoubleSpinBox *lineEditT;
    QDoubleSpinBox *lineEditA;
    QDoubleSpinBox *lineEditF;
    QDoubleSpinBox *lineEditA2;
    QDoubleSpinBox *lineEditF2;
    QDoubleSpinBox *lineEditScale;
    QPushButton *generateButton;
    FileWriterWorkerFFT * worker;
    SignalFunction *signalFunction;

    QCustomPlot *createPlot();
    void generate();
    QDoubleSpinBox *createEdit();
};

#endif // SIGNALWINDOW_H
