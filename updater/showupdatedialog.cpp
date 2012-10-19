#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QDateTime>
#include <QProcess>
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

        // �б�
        model = new QStandardItemModel();
        // �½�Model
        model->setColumnCount(3);
        model->setHeaderData(0,Qt::Horizontal,tr("Action"));
        model->setHeaderData(1,Qt::Horizontal,tr("File"));
        model->setHeaderData(2,Qt::Horizontal,tr("Status"));

        this->setWindowIcon(Utils::getIcon("updater.ico"));
        this->setWindowTitle(tr("update Solo..."));
        updatefiles();

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(closeBtn_clicked()));

        showfiles->setModel(model);
        showfiles->setShowGrid(false);
        showfiles->verticalHeader()->hide();
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

     // ������
     if(error == "Already is newest version"){
         QMessageBox::warning(this, tr("Information"), tr("Solo's Version is already the latest."), QMessageBox::Yes);
         return;
     }

     // û�д������
     if(error.isEmpty()){
        dellist = varMap["dellist"].toList();
        updatelist = varMap["updatelist"].toList();
        reglist =  varMap["reglist"].toList();

        int sumoffile = dellist.size() + updatelist.size() + reglist.size();
        progressBar->setWindowModality(Qt::WindowModal);
        progressBar->setRange(0, sumoffile);

        QVariant vversion = varMap["newversion"];
        QString version = qvariant_cast<QString>(vversion);
        qDebug() << "newest version::" << version;

        QVariant vserver = varMap["server"];
        server = qvariant_cast<QString>(vserver);
        qDebug() << "server::" << server;


        QVariant vvseron= varMap["newversion"];
        newversion = qvariant_cast<QString>(vvseron);
        qDebug() << "newversion::" << newversion;

        QVariant vtestfile = varMap["testfile"];
        QString testfile = qvariant_cast<QString>(vtestfile);
        qDebug() << "testfile::" << testfile;

        // downloadfiles();

        // ���������Ƿ�����
        QString surl(server);
        surl.append(testfile);

        locfileinfo = getLocalFile(testfile);

        DownloadControl *control = new DownloadControl(0, &locfileinfo);
        QObject::connect(control, SIGNAL(FileDownloadFinished()),this, SLOT(checkTestFile()));
        control->StartFileDownload(surl, 1);
     }
}

// ȡ�ñ����ļ���Ϣ
void ShowUpdateDialog::downloadfiles(){
    // ��ʼ����
    int pos = 0;
    // ��ftp�������ļ����Ǳ���
    qDebug() << "updatelist size::" << updatelist.size();

    // ɾ��Reg.exe�ļ�
    QString regpath = QDir::currentPath().append(QDir::separator()).append("reg.exe");
    QFileInfo regFile(regpath);
    // ����Ѿ�������ɾ��
    if (QFile::exists(regpath)) {
        QFile::remove(regFile.fileName());
    }

    foreach(QVariant updatefile, updatelist)
    {

        QString supdatefile = qvariant_cast<QString>(updatefile);
        QFileInfo locfileinfo = getLocalFile(supdatefile);

        QString surl(server);
        surl.append(supdatefile);

        QList<QStandardItem*> items;
        QStandardItem* action = new QStandardItem(tr("downloading"));
        QStandardItem* file = new QStandardItem(supdatefile);
        QStandardItem* isdone = new QStandardItem(tr("busying"));
        items << action;
        items << file;
        items << isdone;
        model->appendRow(items);

        // �������ķ�ʽ�����ļ�����ɺ��˳�
        DownloadControl *control = new DownloadControl(0, &locfileinfo);
        QEventLoop loop;
        QObject::connect(control, SIGNAL(FileDownloadFinished()),&loop, SLOT(quit()));

        url= QUrl::fromEncoded(surl.toUtf8());
        qint64 size = control->GetFileSize(url);
        // 1M Ϊһ�����ݿ�
        int pointCount = 1;
        if( size > 1048576 ){
           pointCount = ( size / 1048576);
        }
        qDebug() << "pointCount::" << pointCount;
        control->StartFileDownload(surl, pointCount);

        loop.exec();

        QList<QStandardItem*> doneitems;
        action = new QStandardItem(tr("download"));
        file = new QStandardItem(supdatefile);
        isdone = new QStandardItem(tr("done"));

        doneitems << action;
        doneitems << file;
        doneitems << isdone;
        model->appendRow(doneitems);

        pos = pos + 1;
        progressBar->setValue(pos);
        showfiles->scrollToBottom();
        showfiles->setColumnWidth(1, showfiles->width() * 0.6);
    }

    // ɾ��
    foreach(QVariant delfile, dellist)
    {
       QString sdelfile = qvariant_cast<QString>(delfile);
       QString delpath = QDir::currentPath().append(QDir::separator()).append(sdelfile);
       QFileInfo tmpfile(delpath);
       qDebug() << " delpath::" << delpath;
       if(tmpfile.exists()){
           QDir dir(tmpfile.absolutePath());
           dir.remove(tmpfile.fileName());
       }

       QList<QStandardItem*> doneitems;
       QStandardItem* action = new QStandardItem(tr("delete"));
       QStandardItem* file = new QStandardItem(sdelfile);
       QStandardItem* isdone = new QStandardItem(tr("done"));

       doneitems << action;
       doneitems << file;
       doneitems << isdone;
       model->appendRow(doneitems);

       showfiles->scrollToBottom();
       pos = pos + 1;
       progressBar->setValue(pos);
    }

    // ע���ļ� ֻ���� ����������Ϊ�߳�ռ��
    foreach(QVariant regfile, reglist)
    {
        QString sregfile = qvariant_cast<QString>(regfile);
        QFileInfo locfileinfo = getLocalFile(sregfile);

        QString surl(server);
        surl.append(sregfile);

        QList<QStandardItem*> items;
        QStandardItem* action = new QStandardItem(tr("downloading"));
        QStandardItem* file = new QStandardItem(sregfile);
        QStandardItem* isdone = new QStandardItem(tr("busying"));
        items << action;
        items << file;
        items << isdone;
        model->appendRow(items);

        // �������ķ�ʽ�����ļ�����ɺ��˳�
        DownloadControl *control = new DownloadControl(0, &locfileinfo);
        QEventLoop loop;
        QObject::connect(control, SIGNAL(FileDownloadFinished()),&loop, SLOT(quit()));

        url= QUrl::fromEncoded(surl.toUtf8());
        qint64 size = control->GetFileSize(url);
        // 1M Ϊһ�����ݿ�
        int pointCount = 1;
        if( size > 1048576 ){
           pointCount = ( size / 1048576);
        }
        qDebug() << "pointCount::" << pointCount;
        control->StartFileDownload(surl, pointCount);

        loop.exec();

        QList<QStandardItem*> doneitems;
        action = new QStandardItem(tr("download"));
        file = new QStandardItem(sregfile);
        isdone = new QStandardItem(tr("done"));

        doneitems << action;
        doneitems << file;
        doneitems << isdone;
        model->appendRow(doneitems);

        pos = pos + 1;
        progressBar->setValue(pos);
        showfiles->scrollToBottom();
        showfiles->setColumnWidth(1, showfiles->width() * 0.6);
    }

    // ������ɺ� д�뱾�����°汾
    int sumoffile = dellist.size() + updatelist.size() + reglist.size();
    if(pos == sumoffile){
        Preferences* p = Preferences::instance();
        p->setVersion(newversion);
    }
}

// �û��ж����������Ƿ�������
void ShowUpdateDialog::checkTestFile()
{
    // <h1>Bad Request (Invalid Hostname)</h1>
    QStringList values = FileUtils::readFile(locfileinfo.filePath());
    if(values.size() > 0){
       QString temp =  values.at(0);
       if(temp.contains("Bad Request (Invalid Hostname)")){
           servererror();
       }else{
           downloadfiles();
       }
    }else{
         // Ϊ��ʱ
         downloadfiles();
    }
}

// ȡ�ñ����ļ���Ϣ
void ShowUpdateDialog::servererror(){
    QMessageBox::warning(this, tr("Information"), tr("Server errors, can not update"), QMessageBox::Yes);
    QCoreApplication::exit(773);
}

// ȡ�ñ����ļ���Ϣ
QFileInfo ShowUpdateDialog::getLocalFile(QString filepath){
    // �����ļ�
    QString localfilepath = QDir::currentPath().append(QDir::separator()).append(filepath.right(filepath.length() -7));
    QFileInfo localfileInfo(localfilepath);

    qDebug() << "localfilepath::" << localfilepath;

    QString localfileName = localfileInfo.fileName();
    QString localpath = localfileInfo.path();

    // ����Ѿ�������ɾ��
    if (QFile::exists(localfilepath)) {
        QFile::remove(localfileName);
    }

    // ������Ŀ¼ʱ ����Ŀ¼
    QDir dir(localpath);
    if(!dir.exists()){
       dir.mkpath(localpath);
    }

    file = new QFile(localfileInfo.filePath());
    if (!file->open(QIODevice::WriteOnly)) {
        delete file;
        file = 0;
    }

    return localfileInfo;
}

// ȷ����ť
void ShowUpdateDialog::confirmBtn_clicked(){
    update = false;
    this->close();
    // ����������
    QCoreApplication::exit(773);
}

// �رհ�ť
void ShowUpdateDialog::closeBtn_clicked(){
     update = false;
     this->close();
     // ����������
     QCoreApplication::exit(773);
}





