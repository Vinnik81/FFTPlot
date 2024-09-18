#ifndef FILEWRITERWORKER_H
#define FILEWRITERWORKER_H

#include <QObject>

class FileWriterWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWriterWorker(const QString &dataFileName, QObject *parent = nullptr);

signals:

};

#endif // FILEWRITERWORKER_H
