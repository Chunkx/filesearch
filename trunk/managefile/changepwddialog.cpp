/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/
#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>
#include <QUuid>
#include "changepwddialog.h"
#include "utils.h"

#include "fileutils.h"
#include "preferences.h"
#include "db/tagdao.h"

ChangePwdDialog::ChangePwdDialog(QWidget * parent)
        : QDialog(parent),
          update(false)
{
        setupUi(this);

        // Set UI
        this->setWindowTitle(tr("Change PassWord"));

        // ȷ����ȡ��
        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));


        Preferences* p = Preferences::instance();
        QString address = p->getAddress();
        quint16 port = p->getPort();
        c = new Client(address, port, this);
        connect(c, SIGNAL(changePwdDataReady()), this, SLOT(changePwdReply()) );
}

// ȷ���ı�����
void ChangePwdDialog::confirmBtn_clicked(){

    QString snewpwd = newpwd->text().trimmed();
    QString srenewpwd = renewpwd->text().trimmed();
    if(snewpwd != srenewpwd){
        QMessageBox::warning(this, tr("Warning"), tr("The ReInput Password is not same with the New PassWord."), QMessageBox::Yes);
        return;
    }

//    // ����ѡ���docUuid
//    Preferences* p = Preferences::instance();

//    // ����Setting �����û���Ϣ
//    QString email = p->getUserEmail();

//    QString surl;
//    surl.append("http://www.slfile.net/mf-changepwd.php?email=");
//    surl.append(email.trimmed());
//    surl.append("&oldpwd=");
//    QString soldpwd = Utils::getMD5Str(oldpwd->text().trimmed());
//    surl.append(soldpwd);
//    surl.append("&newpwd=");
//    QString newpwd = Utils::getMD5Str(snewpwd);
//    surl.append(newpwd);

//    requtil = new ReqUtil(this);
//    connect(requtil,SIGNAL(reqfinished()),this,SLOT(doConfirmReply()));

//    QUrl url= QUrl::fromEncoded(surl.toUtf8());
//    requtil->startRequest(url);

      c->changePwd(oldpwd->text().trimmed(), snewpwd);
}

// ȷ�������ش�
void ChangePwdDialog::doConfirmReply(){

     QVariantMap varMap = requtil->getReply();
     QVariant verror = varMap["error"];
     QString error = qvariant_cast<QString>(verror);
     if(error == "Old PassWord Is Incorrect"){
         QMessageBox::warning(this, tr("Information"), tr("Old PassWord Is Incorrect."), QMessageBox::Yes);
         return;
     }
     if(error == "Email Is Not Existed"){
         QMessageBox::warning(this, tr("Information"), tr("User Is Not Existed."), QMessageBox::Yes);
         return;
     }

     if(error.isEmpty()){
         QMessageBox::information(this, tr("Information"), tr("Change PassWord Success, Please Login in"), QMessageBox::Yes);
         // �ر� TODO: ������������
         QCoreApplication::exit(773);
     }
}

// �ı�����󷵻ش���
void ChangePwdDialog::changePwdReply(){
    qDebug() << "ChangePwdDialog::changePwdReply";
    if(c->isSuccess()){
        QMessageBox::information(this, tr("Information"), tr("Change PassWord Success, Please Login in"), QMessageBox::Yes);
        // �ر� TODO: ������������
        QCoreApplication::exit(773);
    }
}

// ȡ����ť
void ChangePwdDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}



