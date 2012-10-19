#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTextCodec>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <QSslError>
#include <QCoreApplication>

#include "requtil.h"
#include "preferences.h"
#include "utils.h"

// ��¼����
ReqUtil::ReqUtil(QWidget * parent)
{
    manager=new QNetworkAccessManager(this);
    loop = new QEventLoop;
}

//��������
void ReqUtil::startRequest(QUrl url)
{

    reply = manager->get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));  //������ɺ�
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead())); //�п�������ʱ
    // �ж��쳣
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

    connect(reply,SIGNAL(finished()),loop,SLOT(quit()));  // �˳�ѭ��
    loop->exec();
}

// ����
void ReqUtil::slotError(QNetworkReply::NetworkError error)
{
     hasError = true;
}
// ����
void ReqUtil::slotSslErrors(QList<QSslError> temp)
{
     hasError = true;
}
//�п�������
bool ReqUtil::getError()
{
     return hasError;
}
//�п�������
void ReqUtil::httpReadyRead()
{

    rtnStr = reply->readAll();
    QString rtnString = QString(rtnStr);
    // �ж��Ƿ�404�쳣
    qDebug() << "rtnString::" << rtnString;

}
// �������
void ReqUtil::httpFinished()
 {
     if(reply->error()){
        hasError = true;
     }
     reply->deleteLater();
     reply = 0;
     emit reqfinished();
}

QVariantMap ReqUtil::getReply()
{

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(rtnStr,&ok).toMap();
    QString rtnString = QString(rtnStr);
    // �ж��Ƿ�404�쳣
    qDebug() << "rtnString::" << rtnString;
    return result;
}
