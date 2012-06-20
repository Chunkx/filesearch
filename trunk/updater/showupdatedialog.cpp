#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QDateTime>
#include <QDir>
#include <QTextCodec>
#include <qjson/parser.h>
#include <qjson/serializer.h>

#include "showupdatedialog.h"
#include "preferences.h"
#include "utils.h"
#include "fileutils.h"
#include "download.h"
#include "downloadcontrol.h"

// ��ʾ�������̴���
ShowUpdateDialog::ShowUpdateDialog(QWidget * parent)
        : QDialog(parent),
          update(false)
{
	setupUi(this);
        this->setWindowIcon(Utils::getIcon("file_manager.png"));
        this->setWindowTitle(tr("update Slfile..."));
        updatefiles();

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(closeBtn_clicked()));
}

// Copy all
void ShowUpdateDialog::updatefiles(){

    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();

    // ����Setting �����û���Ϣ
    QString email = p->getUserEmail();
    QString version = p->getVersion();

    QString surl;
    surl.append("http://www.slfile.net/mf-update.php?email=");
    surl.append(email.trimmed());
    surl.append("&version=");
    surl.append(version);

    qDebug() << "surl::" << surl;
    requtil = new ReqUtil(this);
    connect(requtil,SIGNAL(reqfinished()),this,SLOT(doConfirmReply()));

    QUrl url= QUrl::fromEncoded(surl.toUtf8());
    requtil->startRequest(url);
}

// ȷ�������ش�
void ShowUpdateDialog::doConfirmReply(){

     QVariantMap varMap = requtil->getReply();
     QVariant verror = varMap["error"];
     QString error = qvariant_cast<QString>(verror);
     qDebug() << "error::" << error;

     // ������
     if(error == "Email Is Not Existed"){
         QMessageBox::warning(this, tr("Information"), tr("User Is Not Existed."), QMessageBox::Yes);
         return;
     }

     // û�д������
     if(error.isEmpty()){
        QVariantList dellist = varMap["dellist"].toList();
        QVariantList updatelist = varMap["updatelist"].toList();

        int sumoffile = dellist.size() + updatelist.size();
        progressBar->setWindowModality(Qt::WindowModal);
        progressBar->setRange(0, sumoffile);

        QVariant vversion = varMap["newversion"];
        QString version = qvariant_cast<QString>(vversion);
        qDebug() << "newest version::" << version;

        QVariant vserver = varMap["server"];
        QString server = qvariant_cast<QString>(vserver);
        qDebug() << "server::" << server;

        // ��ʼ����
        int pos = 0;
        foreach(QVariant delfile, dellist)
        {

           QString sdelfile = qvariant_cast<QString>(delfile);
           FileUtils::removeDirectory(sdelfile);
           qDebug() << "del file::" << sdelfile;
           pos = pos + 1;
           progressBar->setValue(pos);
        }

        // TODO ��ftp�������ļ����Ǳ���
        foreach(QVariant updatefile, updatelist)
        {

             QString supdatefile = qvariant_cast<QString>(updatefile);
//           qDebug() << "update file::" << supdatefile;
//           downloadFile(server, supdatefile);
           // FileUtils::copyFileToDir(sdelfile, )

           //�������ķ�ʽ�����ļ�����ɺ��˳�
            DownloadControl *control = new DownloadControl;
            QEventLoop loop;
            QObject::connect(control, SIGNAL(FileDownloadFinished()),&loop, SLOT(quit()));

            QString surl(server);
            surl.append(supdatefile);
            //QUrl url = QUrl::fromEncoded(surl->toUtf8());
            qint64 size = control->GetFileSize(url);
            // 1M Ϊһ�����ݿ�
            int pointCount = size / 1024*1024;
            qDebug() << "pointCount::" << pointCount;

            pointCount = (pointCount == 0? 1 : pointCount);
            control->StartFileDownload(surl, pointCount);
            loop.exec();

           pos = pos + 1;
           progressBar->setValue(pos);
        }
     }
}
// �����ļ�
void ShowUpdateDialog::downloadFile(QString server, QString filepath){

    QString* surl = new QString(server);
    surl->append(filepath);
    url = QUrl::fromEncoded(surl->toUtf8());

    // �����ļ�
    QString localfilepath = QDir::currentPath().append(QDir::separator()).append(filepath.right(filepath.length() -7));
    QFileInfo localfileInfo(localfilepath);

    QString localfileName = localfileInfo.fileName();
    QString localpath = localfileInfo.path();
    qDebug() << "filepath::" << localfilepath;
    qDebug() << "path::" << localpath;
    qDebug() << "fileName::" << localfileName;

    // schedule the request
    qDebug() << "url::" << url;

    if (QFile::exists(localfilepath)) {
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("There already exists a file called %1 in "
                                     "the current directory. Overwrite?").arg(localfileName),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
            == QMessageBox::No)
            return;
        QFile::remove(localfileName);
    }

    // ������Ŀ¼ʱ ����Ŀ¼
    QDir dir(localpath);
    if(!dir.exists()){
       dir.mkpath(localpath);
    }

    file = new QFile(localfileInfo.filePath());
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(localfileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    httpRequestAborted = false;
    startRequest(url);
}


void ShowUpdateDialog::startRequest(QUrl url)
{
    QEventLoop loop;
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

    QObject::connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
    loop.exec();

}

void ShowUpdateDialog::httpFinished()
{
//    if (httpRequestAborted) {
//        if (file) {
//            file->close();
//            file->remove();
//            delete file;
//            file = 0;
//        }
//        reply->deleteLater();
//        return;
//    }

//    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//    if (reply->error()) {
//        file->remove();
//        QMessageBox::information(this, tr("HTTP"),
//                                 tr("Download failed: %1.")
//                                 .arg(reply->errorString()));
//    } else if (!redirectionTarget.isNull()) {
//        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
//        if (QMessageBox::question(this, tr("HTTP"),
//                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
//                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
//            url = newUrl;
//            reply->deleteLater();
//            file->open(QIODevice::WriteOnly);
//            file->resize(0);
//            startRequest(url);
//            return;
//        }
//    } else {
//          QString text = showfiles->toPlainText();
//          text.append(tr("Downloaded %1 to %2.").arg(file->fileName()).arg(QDir::currentPath())).append("\n");
//          showfiles->setPlainText(text);
//    }

    reply->deleteLater();
    reply = 0;
////    delete file;
//    file = 0;
}

void ShowUpdateDialog::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file){
        file->write(reply->readAll());
    }
}


// ȷ����ť
void ShowUpdateDialog::confirmBtn_clicked(){
     update = false;
     this->close();
}

// �رհ�ť
void ShowUpdateDialog::closeBtn_clicked(){
     update = false;
     this->close();
}





