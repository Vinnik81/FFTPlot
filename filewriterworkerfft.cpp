#include "filewriterworkerfft.h"
#include <QtMath>
#include <vector>
#include "thirdparty/FFT/FftRealPair.hpp"
#include <QTime>
#include <QDataStream>
#include <QDebug>
#include <QPointF>
#include "signalfunction.h"

using std::vector;

FileWriterWorkerFFT::FileWriterWorkerFFT(const SignalFunction *signalFunction, const QString &dataFileName, QObject *parent) : FileWriterWorker(dataFileName, parent)
{
    m_pSignalFunction = signalFunction;
    m_pVibroAcceleration = new QVector<QPointF>;
    m_pVibroVelocity = new QVector<QPointF>;
    m_pDisplacement = new QVector<QPointF>;
}

FileWriterWorkerFFT::~FileWriterWorkerFFT()
{
    delete m_pVibroAcceleration;
    delete m_pVibroVelocity;
    delete  m_pDisplacement;
}

double FileWriterWorkerFFT::funcPlot(double time)
{
    if (m_pVibroAcceleration->isEmpty()) {
        return 0.0; // или какое-то другое значение по умолчанию
    }

    return accelerationMax;
}

void FileWriterWorkerFFT::printPlot()
{
    for (int i = 0; i < m_pVibroAcceleration->size(); ++i) {
        double value = (*m_pVibroAcceleration)[i].y();
        if (value > 100) {
            qDebug() << i << value;
        }
    }
}

void FileWriterWorkerFFT::FFT(double fs, double t)
{
//    const double FS = 48000.0; //32768.0;
//    const double T = 1.0; //1.0;
//    double A = 10.0;
//    double F = 80.0;
    freqMax = 0.0;
    accelerationMax = 0.0;
    velocityMax = 0.0;
    displacementMax = 0.0;
    signal.clear();
    m_pVibroAcceleration->clear();
    m_pVibroVelocity->clear();
    m_pDisplacement->clear();
    int n = ceil(fs * t);
    m_pVibroAcceleration->resize(n);
    m_pVibroVelocity->resize(n);
    m_pDisplacement->resize(n);
    signal.resize(n);
    vector<double> inputreal(n);
    vector<double> inputimag(n);
//    double t = m_pTime->elapsed() / 1000.0; // время в секундах
    for (int i = 0; i < n; ++i) {
        double ts = (i/fs);
        QPointF func = m_pSignalFunction->function(ts);
        inputreal[i] = func.x();
        inputimag[i] = func.y();
        signal[i] = inputreal[i];
    }
    Fft::transform(inputreal, inputimag);
    //int freq
    QVector<double> freqx = linspace(0, fs - 1, n);
    for (int i = 0; i < n; ++i) {
        double value = inputreal[i] / n;
        double vibroVelocity = freqx[i] > 0.0 ? value / (2 * M_PI * freqx[i]) : 0;
        double vibroDisplacement = freqx[i] > 0.0 ? vibroVelocity / (2 * M_PI * freqx[i]) : 0;
        if (value > VALUE_THRESHOLD) {
            freqMax = freqx[i];
            if (value > accelerationMax) {
                accelerationFreq = freqx[i];
                accelerationMax = value;
            }
            if (vibroVelocity > velocityMax) {
                velocityFreq = freqx[i];
                velocityMax = vibroVelocity;
            }
            if (vibroDisplacement > displacementMax) {
                displacementFreq = freqx[i];
                displacementMax = vibroDisplacement;
            }
        }
        (*m_pVibroAcceleration)[i] = QPointF(freqx[i], value);
        (*m_pVibroVelocity)[i] = QPointF(freqx[i], vibroVelocity);
        (*m_pDisplacement)[i] = QPointF(freqx[i], vibroDisplacement);
    }
}

const QVector<QPointF> &FileWriterWorkerFFT::vibroAcceleration() const
{
    return (*m_pVibroAcceleration);
}

const QVector<double> &FileWriterWorkerFFT::getSignal() const
{
    return signal;
}

double FileWriterWorkerFFT::getFreqMax() const
{
    return freqMax;
}

const QVector<QPointF> &FileWriterWorkerFFT::vibroVelocity() const
{
    return (*m_pVibroVelocity);
}

const QVector<QPointF> &FileWriterWorkerFFT::vibroDisplacement() const
{
    return (*m_pDisplacement);
}

QVector<double> FileWriterWorkerFFT::linspace(double start, double end, int n) {
    QVector<double> result;

    if (n <= 0) return result; // если количество точек <= 0, возвращаем пустой вектор
    if (n == 1) {
        result.append(start); // если одна точка, то возвращаем только начальное значение
        return result;
    }

    double step = (end - start) / (n - 1); // шаг между точками

    for (int i = 0; i < n; ++i) {
        result.append(start + i * step);
    }

    return result;
}


