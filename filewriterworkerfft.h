#ifndef FILEWRITERWORKERFFT_H
#define FILEWRITERWORKERFFT_H

#include <QObject>
#include <QVector>



class FileWriterWorkerFFT: public QObject
{
public:
    explicit FileWriterWorkerFFT(const QString &dataFileName, QObject *parent = Q_NULLPTR);
    virtual ~FileWriterWorkerFFT();
    double funcPlot(double time);
    void printPlot();

private:
    QMap<double, double> *m_pValues;
    int m_step;
    QVector<double> linspace(double start, double end, int n);
};

#endif // FILEWRITERWORKERFFT_H
