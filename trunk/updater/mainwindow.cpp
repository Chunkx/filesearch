#include <QDebug>

#include "mainwindow.h"
#include "preferences.h"
#include "showupdatedialog.h"


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

// ȷ�������ش�
void MainWindow::doConfirmReply(){

     QVariantMap varMap = requtil->getReply();
     QVariant new_version = varMap["new_version"];

     QString newversion = qvariant_cast<QString>(new_version);
     qDebug() << "new_version::" << newversion;

     Preferences* p = Preferences::instance();
     QString version = p->getVersion();
     qDebug() << "version::" << version;
     if(newversion != version || version.isEmpty()){
         ShowUpdateDialog dlg;
         dlg.exec();
     }

}


MainWindow::~MainWindow()
{

}
