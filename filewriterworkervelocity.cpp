#include "filewriterworkervelocity.h"

FileWriterWorkerVelocity::FileWriterWorkerVelocity(const SignalFunction *signalFunction, const QString &dataFileName, QObject *parent)
    : FileWriterWorkerFFT(signalFunction, dataFileName, parent)
{

}

double FileWriterWorkerVelocity::funcPlot(double time)
{
    if (m_pVibroVelocity->isEmpty()) {
        return 0.0; // или какое-то другое значение по умолчанию
    }

    return velocityMax;
}
