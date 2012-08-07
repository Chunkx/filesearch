#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTextCodec>
#include <QSslError>
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "requtil.h"
#include "preferences.h"
#include "utils.h"
#include "db/docdao.h"
#include "db/dirdao.h"

// ��¼����
ReqUtil::ReqUtil(QWidget * parent)
{
    manager=new QNetworkAccessManager(this);
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
    file = new QFile("temp.json");
    if(file->open(QIODevice::WriteOnly)){
        if (file){
            QByteArray bytes = reply->readAll();
            file->write(bytes);
            QString string;
            string = QString(bytes);
            // �ж��Ƿ�404�쳣
            // http://go.microsoft.com/fwlink/?linkid=8180">Microsoft ???</a>&ldquo;HTTP&rdquo;&ldquo;404
            if(string.indexOf("404") != -1 && string.indexOf("HTML") != -1 ){
                QMessageBox::warning(this, tr("Warning"), tr("The Server has Errors, Please Contact the Administrator!"), QMessageBox::Yes);
                QCoreApplication::exit(773);
            }
        }
    }
}
// �������
void ReqUtil::httpFinished()
 {
     file->flush();
     file->close();
     reply->deleteLater();
     reply = 0;
     emit reqfinished();
}

QVariantMap ReqUtil::getReply()
{

  QJson::Parser parser;
  bool ok;
  QVariantMap result = parser.parse(file,&ok).toMap();
  delete file;
  file = 0;
  return result;

}
