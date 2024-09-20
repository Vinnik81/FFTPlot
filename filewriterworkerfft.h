#ifndef FILEWRITERWORKERFFT_H
#define FILEWRITERWORKERFFT_H

#include "filewriterworker.h"

#include <QObject>
#include <QVector>

class SignalFunction;


class FileWriterWorkerFFT: public FileWriterWorker
{
public:
    static constexpr double VALUE_THRESHOLD = 0.1;

    explicit FileWriterWorkerFFT(const SignalFunction *signalFunction, const QString &dataFileName, QObject *parent = Q_NULLPTR);
    virtual ~FileWriterWorkerFFT();
    virtual double funcPlot(double time);
    void printPlot();
    void FFT(double fs, double t);

    const QVector<QPointF> &vibroAcceleration() const;
    const QVector<QPointF> &vibroVelocity() const;
    const QVector<double> &getSignal() const;
    const QVector<QPointF> &vibroDisplacement() const;
    double getFreqMax() const;

private:
    const SignalFunction *m_pSignalFunction;
    QVector<QPointF> *m_pVibroAcceleration;
    QVector<double> signal;
    double freqMax;
    double accelerationFreq;
    double accelerationMax;

    QVector<double> linspace(double start, double end, int n);

protected:
    double velocityMax;
    QVector<QPointF> *m_pVibroVelocity;
    double velocityFreq;
    double displacementMax;
    QVector<QPointF> *m_pDisplacement;
    double displacementFreq;
};

#endif // FILEWRITERWORKERFFT_H
