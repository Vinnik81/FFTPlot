#include "filewriterworkerdisplacement.h"

FileWriterWorkerDisplacement::FileWriterWorkerDisplacement(SignalFunction *signalFunction, const QString &dataFileName, QObject *parent)
    : FileWriterWorkerFFT(signalFunction, dataFileName, parent)
{

}

double FileWriterWorkerDisplacement::funcPlot(double time)
{
    if (m_pDisplacement->isEmpty()) {
        return 0.0;
    }
    return displacementMax * 1000000;
}
