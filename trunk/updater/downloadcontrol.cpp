#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include "downloadcontrol.h"
#include "download.h"

//�������ķ�ʽ��ȡ�����ļ��ĳ���
qint64 DownloadControl::GetFileSize(QUrl url)
{
    QNetworkAccessManager manager;
    qDebug() << "Getting the file size...";
    QEventLoop loop;
    //�������󣬻�ȡĿ���ַ��ͷ����Ϣ
    QNetworkReply *reply = manager.head(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
    delete reply;
    qint64 size = var.toLongLong();
    qDebug() << "The file size is: " << size;
    return size;
}

DownloadControl::DownloadControl(QObject *parent)
: QObject(parent)
{
    m_DownloadCount = 0;
    m_FinishedNum = 0;
    m_FileSize = 0;
    m_File = new QFile;
}

void DownloadControl::StartFileDownload(const QString &url, int count)
{
    m_DownloadCount = count;
    m_FinishedNum = 0;
    m_Url = QUrl(url);
    m_FileSize = GetFileSize(m_Url);
    //�Ȼ���ļ�������
    QFileInfo fileInfo(m_Url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    m_File->setFileName(fileName);
    //���ļ�
    m_File->open(QIODevice::WriteOnly);
    Download *tempDownload;

    //���ļ��ֳ�PointCount�Σ����첽�ķ�ʽ����
    qDebug() << "Start download file from " << url;
    for(int i=0; i < m_DownloadCount; i++)
    {
        //�����ÿ�εĿ�ͷ�ͽ�β��HTTPЭ������Ҫ����Ϣ��
        int start = m_FileSize * i / m_DownloadCount;
        int end = m_FileSize * (i+1) / m_DownloadCount;
        if( i != 0 )
            start--;

        //�ֶ����ظ��ļ�
        tempDownload = new Download(i+1, this);
        connect(tempDownload, SIGNAL(DownloadFinished()),
            this, SLOT(SubPartFinished()));
        connect(tempDownload, SIGNAL(DownloadFinished()),
            tempDownload, SLOT(deleteLater()));
        tempDownload->StartDownload(m_Url, m_File, start, end);
    }
}

void DownloadControl::SubPartFinished()
{
    m_FinishedNum++;
    //�������������ļ���������˵���ļ�������ϣ��ر��ļ��������ź�
    if( m_FinishedNum == m_DownloadCount )
    {
        m_File->close();
        emit FileDownloadFinished();
        qDebug() << "Download finished";
    }
}
