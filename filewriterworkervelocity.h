#ifndef FILEWRITERWORKERVELOCITY_H
#define FILEWRITERWORKERVELOCITY_H

#include "filewriterworkerfft.h"

class FileWriterWorkerVelocity : public FileWriterWorkerFFT
{
public:
    explicit FileWriterWorkerVelocity(const SignalFunction *signalFunction, const QString &dataFileName, QObject *parent = nullptr);
    virtual double funcPlot(double time);
};

#endif // FILEWRITERWORKERVELOCITY_H
