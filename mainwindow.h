#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "thirdparty/qcustomplot/qcustomplot.h"


QT_BEGIN_NAMESPACE



class FileWriterWorkerFFT;

class SignalFunction;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlotSignal;
    QCustomPlot *customPlotSpectrum;
    QCustomPlot *vibroVelocity;
    QCustomPlot *vibroDisplacement;
    QCPItemTracer *m_pTracerAcceleration = Q_NULLPTR;
    QCPItemTracer *m_pTracerVelocity = Q_NULLPTR;
    QCPItemTracer *m_pTracerDisplacement = Q_NULLPTR;
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
//    FileWriterWorkerFFT * worker;
    SignalFunction *signalFunction;

    QCustomPlot *createPlot(const QString &xLabel, const QString &yLabel);
    void generate();
    QDoubleSpinBox *createEdit();
    QCPItemTracer *createTracer(QCustomPlot *customPlot, double graphKey, QString keyUnit, double graphValue, QString valueUnit);

};
#endif // MAINWINDOW_H
