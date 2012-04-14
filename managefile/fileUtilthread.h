#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QThread>
#include <QDesktopServices>
#include <QUrl>
//������س����ָ���ļ�

#include <QTextCodec>
#include <QDebug>
class FileThread : public QThread
{
public:
    FileThread(QString fileName);
    FileThread();
    ~FileThread();
    void setFileName(QString fileName);
signals:
    void finished();
protected:
    void run();
private:
    QString fileName;
};

#endif // FILETHREAD_H
