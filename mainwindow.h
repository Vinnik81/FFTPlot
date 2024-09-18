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
    QLabel *labelFs;
    QLabel *labelT;
    QLabel *labelA;
    QLabel *labelF;
    QLabel *labelA2;
    QLabel *labelF2;
    QLabel *labelScale;
    QLineEdit *lineEditFs;
    QLineEdit *lineEditT;
    QLineEdit *lineEditA;
    QLineEdit *lineEditF;
    QLineEdit *lineEditA2;
    QLineEdit *lineEditF2;
    QLineEdit *lineEditScale;
    QPushButton *generateButton;
    FileWriterWorkerFFT * worker;
    SignalFunction *signalFunction;

    QCustomPlot *createPlot();
    void generate();
};
#endif // MAINWINDOW_H
