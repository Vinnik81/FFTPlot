#include "filewriterworkerfft.h"
#include <QtMath>
#include <vector>
#include "thirdparty/FFT/FftRealPair.hpp"
#include <QTime>
#include <QDataStream>
#include <QDebug>
#include <QPointF>

using std::vector;

FileWriterWorkerFFT::FileWriterWorkerFFT(const QString &dataFileName, QObject *parent) : QObject(parent)
{
    m_pValues = new QVector<QPointF>;
}

FileWriterWorkerFFT::~FileWriterWorkerFFT()
{
    delete m_pValues;
}

double FileWriterWorkerFFT::funcPlot(double time)
{
    if (m_pValues->isEmpty()) {
        return 0.0; // или какое-то другое значение по умолчанию
    }

    double value = (*m_pValues)[m_step].y();
    m_step = (m_step + 1) % m_pValues->size();
    //qDebug() << qMax(abs(value), 0.1);
    return qMax(abs(log10(abs(value))), 100.0);
}

void FileWriterWorkerFFT::printPlot()
{
    for (int i = 0; i < m_pValues->size(); ++i) {
        double value = (*m_pValues)[i].y();
        if (value > 100) {
            qDebug() << i << value;
        }
    }
}

void FileWriterWorkerFFT::FFT(double fs, double t, double a, double f)
{
//    const double FS = 48000.0; //32768.0;
//    const double T = 1.0; //1.0;
//    double A = 10.0;
//    double F = 80.0;
    signal.clear();
    m_pValues->clear();
    int n = ceil(fs * t);
    m_pValues->resize(n);
    signal.resize(n);
    vector<double> inputreal(n);
    vector<double> inputimag(n);
//    double t = m_pTime->elapsed() / 1000.0; // время в секундах
    for (int i = 0; i < n; ++i) {
        double ts = (i/fs);
        inputreal[i] = a * qCos(2 * M_PI * f * ts);
        inputimag[i] = a * qSin(2 * M_PI * f * ts);
        signal[i] = inputreal[i];
    }
    Fft::transform(inputreal, inputimag);
    //int freq
    QVector<double> freqx = linspace(0, fs - 1, n);
    for (int i = 0; i < n; ++i) {

        (*m_pValues)[i] = QPointF(freqx[i], inputreal[i] / n);
    }
}

QVector<QPointF> *FileWriterWorkerFFT::pValues() const
{
    return m_pValues;
}

const QVector<double> &FileWriterWorkerFFT::getSignal() const
{
    return signal;
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
