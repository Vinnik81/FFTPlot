#include "filewriterworkerfft.h"
#include <QtMath>
#include <vector>
#include "thirdparty/FFT/FftRealPair.hpp"
#include <QTime>
#include <QDataStream>
#include <QDebug>

using std::vector;

FileWriterWorkerFFT::FileWriterWorkerFFT(const QString &dataFileName, QObject *parent) : QObject(parent)
{
    m_pValues = new QMap<double, double>;
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

    double value = m_pValues->value(m_step, 0.0);
    m_step = (m_step + 1) % m_pValues->size();
    //qDebug() << qMax(abs(value), 0.1);
    return qMax(abs(log10(abs(value))), 100.0);
}

void FileWriterWorkerFFT::printPlot()
{
    for (double i : m_pValues->keys()) {
        double value = (*m_pValues)[i];
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
    int n = (fs * t);
    vector<double> inputreal;
    vector<double> inputimag;
//    double t = m_pTime->elapsed() / 1000.0; // время в секундах
    for (int i = 0; i < n; ++i) {
        double ts = (i/fs);
        inputreal.push_back(a * qCos(2 * M_PI * f * ts));
        inputimag.push_back(a * qSin(2 * M_PI * f * ts));
        signal.push_back(inputreal[i]);
    }
    Fft::transform(inputreal, inputimag);
    //int freq
    QVector<double> freqx = linspace(0, fs - 1, n);
    for (int i = 0; i < n; ++i) {

        m_pValues->insert(freqx[i], inputreal[i] / n);
    }
}

QMap<double, double> *FileWriterWorkerFFT::pValues() const
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
