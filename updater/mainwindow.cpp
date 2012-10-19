#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QMessageBox>

#include "mainwindow.h"
#include "preferences.h"
#include "updatedialog.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    // ��÷�����Version
    QString surl;
    surl.append("http://www.slfile.net/mf-getnewversion.php");

    requtil = new ReqUtil(this);
    connect(requtil,SIGNAL(reqfinished()),this,SLOT(doConfirmReply()));

    QUrl url= QUrl::fromEncoded(surl.toUtf8());
    requtil->startRequest(url);
}


MainWindow::~MainWindow()
{
    Preferences::deleteInstance();
}

void MainWindow::closeEvent(QCloseEvent * e)
{
     Preferences::deleteInstance();
}

// ȷ�������ش�
void MainWindow::doConfirmReply(){
    bool hasError = requtil->getError();
    if(!hasError){
        QVariantMap varMap = requtil->getReply();
        QVariant new_version = varMap["new_version"];

        QString newversion = qvariant_cast<QString>(new_version);
        qDebug() << "new_version::" << newversion;

        Preferences* p = Preferences::instance();
        QString version = p->getVersion();
        qDebug() << "local_version::" << version;

        if((newversion != version) || version.isEmpty()){
             UpdateDialog dlg;
             dlg.exec();
        }else{
            QMessageBox::information(this, tr("Information"), tr("Solo's Version is already the latest."), QMessageBox::Yes);
            QCoreApplication::exit(773);
        }
    }else{
        // �����쳣,����������
        QCoreApplication::exit(773);
    }

}
