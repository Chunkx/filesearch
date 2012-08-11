#include "refereedialog.h"

#include <QDir>
#include <QMessageBox>
#include <QThread>
#include "db/docdao.h"
#include "db/dirdao.h"
#include "db/relatedocdao.h"
#include "utils.h"
#include "preferences.h"

RefereeDialog::RefereeDialog (QWidget *parent, const QString & curUuid)
    : QDialog(parent),update(false)
{
    setupUi (this);

    // ����
    Preferences* p = Preferences::instance();
    QString displayName = p->getDisplayName();

    if(displayName.isEmpty()){
        realname->setText(tr("Please Input your real name"));
    }
    QString userEmail = p->getUserEmail();

    emailtitle->setText(tr("Your Friend %1 recommend slfile to Manage document").arg(displayName));
    content->setHtml(tr("hi! , your friend %1 is using slfile to manage document, <br>do you want to try? Use this Address <a href='http://www.slfile.net/wp-register.php?referee=%2' target='_blank'>http://www.slfile.net/wp-register.php?referee=%3</a>) to Sign up you will help you friend get 300 point score.").arg(displayName).arg(userEmail).arg(userEmail));

    connect(realname, SIGNAL(textChanged(QString)), this, SLOT(changeEmail(QString)));

    connect(closeBtn,SIGNAL(clicked()),this, SLOT(closeBtn_clicked()));
    connect(applyBtn,SIGNAL(clicked()),this, SLOT(applyBtn_clicked()));
}

// �ı��ʼ�����
void RefereeDialog::changeEmail(QString userName)
{
    // ����
    Preferences* p = Preferences::instance();
    QString userEmail = p->getUserEmail();

    emailtitle->setText(tr("Your Friend %1 recommend slfile to Manage document").arg(userName));
    content->setHtml(tr("hi! , your friend %1 is using slfile to manage document, <br>do you want to try? Use this Address <a href='http://www.slfile.net/wp-register.php?referee=%2' target='_blank'>http://www.slfile.net/wp-register.php?referee=%3</a>) to Sign up you will help you friend get 300 point score.").arg(userName).arg(userEmail).arg(userEmail));
}

// �����ʼ�
void RefereeDialog::applyBtn_clicked()
{

    QString addrs = emailaddr->toPlainText();
    if(addrs.trimmed().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("The address is empty, Please Input"), QMessageBox::Yes);
        return;
    }

    QString scontent = content->toHtml();
    if(scontent.trimmed().isEmpty()){
        QMessageBox::warning(this,  tr("Warning") ,tr("The email content is empty, Please Input"), QMessageBox::Yes);
        return;
    }

    QString title = emailtitle->text();
    if(title.trimmed().isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("The title is empty, Please Input"), QMessageBox::Yes);
        return;
    }

    QString name = realname->text();
    if(title.trimmed().isEmpty() || name == tr("Please Input your real name")){
        QMessageBox::warning(this, tr("Warning"), tr("Please Input your real name"), QMessageBox::Yes);
        return;
    }

    sendMailObj.m_successList.clear();
    sendMailObj.m_failList.clear();

    QStringList emailaddrs = addrs.split(",");

    sendMailObj.toaddrs = emailaddrs;
    sendMailObj.title = title;
    sendMailObj.content = scontent;

    SendMailSign dummy;
    QObject::connect(&dummy, SIGNAL(sig()), &sendMailObj, SLOT(sendMail()));
    QObject::connect(&sendMailObj, SIGNAL(finished()), this, SLOT(checkSuccess()));

    dummy.emitsig();

    // ���ʼ���ַ�ύ������

    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    // ����Setting �����û���Ϣ
    QString email = p->getUserEmail();

    QString surl;
    surl.append("http://www.slfile.net/mf-friendemails.php?email=");
    surl.append(email.trimmed());
    surl.append("&friendemails=");
    surl.append(addrs);
    surl.append("&username=");
    surl.append(name);

    requtil = new ReqUtil(this);

    connect(requtil,SIGNAL(reqfinished()),this,SLOT(doConfirmReply()));
    QUrl url= QUrl::fromEncoded(surl.toUtf8());
    requtil->startRequest(url);
}

// ȷ�������ش�
void RefereeDialog::doConfirmReply(){

     QVariantMap varMap = requtil->getReply();
     QVariant verror = varMap["error"];
     QString error = qvariant_cast<QString>(verror);

     if(error == "Email Is Not Existed"){
         QMessageBox::warning(this, tr("Information"), tr("User Is Not Existed."), QMessageBox::Yes);
         return;
     }
     if(error.isEmpty()){
        // ���û����͹����ʼ������ڱ���,�û����ڷ����ĵ�


     }
}

// �ɹ�
void RefereeDialog::checkSuccess()
{

    // ��ʾ���ͳɹ����ʼ�
    if(sendMailObj.m_successList.size() > 0){
        QString email;
        QString msg;
        msg.append(tr("Send Mail Success !"));
        msg.append("\n");
        foreach(email, sendMailObj.m_successList){
            msg.append(email);
            msg.append("\n");
        }
         QMessageBox::warning(this, tr("Information"), msg, QMessageBox::Yes);
    }

     // δ���ͳɹ����ʼ�
    if(sendMailObj.m_failList.size() > 0){
        QString email;
        QString msg;
        msg.append(tr("Send Mail Failed, Please check the email Address !"));
        msg.append("\n");
        foreach(email, sendMailObj.m_successList){
            msg.append(email);
            msg.append("\n");
        }
         QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Yes);
    }


}

// ȡ��
void RefereeDialog::closeBtn_clicked()
{
    update = true;
    this->close();
}

