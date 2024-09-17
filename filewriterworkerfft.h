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
    void FFT(double fs, double t, double a, double f);

    QMap<double, double> *pValues() const;

    const QVector<double> &getSignal() const;

private:
    QMap<double, double> *m_pValues;
    int m_step;
    QVector<double> signal;

    QVector<double> linspace(double start, double end, int n);
};

#endif // FILEWRITERWORKERFFT_H
