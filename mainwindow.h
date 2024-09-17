#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "thirdparty/qcustomplot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class FileWriterWorkerFFT;
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
    QLabel *labelFs;
    QLabel *labelT;
    QLabel *labelA;
    QLabel *labelF;
    QLineEdit *lineEditFs;
    QLineEdit *lineEditT;
    QLineEdit *lineEditA;
    QLineEdit *lineEditF;
    QPushButton *generateButton;
    FileWriterWorkerFFT * worker;

    QCustomPlot *createPlot();
    void generate();
};
#endif // MAINWINDOW_H
