#ifndef FILEWRITERWORKERDISPLACEMENT_H
#define FILEWRITERWORKERDISPLACEMENT_H

#include "filewriterworkerfft.h"

class FileWriterWorkerDisplacement : public FileWriterWorkerFFT
{
public:
    explicit FileWriterWorkerDisplacement(SignalFunction *signalFunction, const QString &dataFileName, QObject *parent = nullptr);
    virtual double funcPlot(double time);
};

#endif // FILEWRITERWORKERDISPLACEMENT_H
