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
#include "scoretovipdialog.h"
#include "utils.h"
#include "fileutils.h"
#include "preferences.h"
#include "db/tagdao.h"

ScoreToVipDialog::ScoreToVipDialog(QWidget * parent)
        : QDialog(parent),
          update(false)
{
	setupUi(this);

        // Set UI
        this->setWindowTitle(tr("Use Score to vip"));

        // �趨ֵ
        // ����ѡ���docUuid
        Preferences* p = Preferences::instance();
        QString score = p->getScore();
        totalscore->setText(score);

        // ȷ����ȡ��
        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));

        // �����
        connect(inputscore, SIGNAL(textChanged(const QString &)), this, SLOT(countMonth()));

        //����Disable��Ŀ
        totalscore->setDisabled(true);
        numofmonth->setDisabled(true);
        remainscore->setDisabled(true);
}

// ȷ��
void ScoreToVipDialog::confirmBtn_clicked(){
    int month = numofmonth->text().toInt();
    if(month == 0){
        QMessageBox::warning(this, tr("Information"), tr("The month is 0 . Please input score again"), QMessageBox::Yes);
        return;
    }

    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    // ����Setting �����û���Ϣ
    QString email = p->getUserEmail();

    QString surl;
    surl.append("http://www.slfile.net/mf-scoretovip.php?email=");
    surl.append(email.trimmed());
    surl.append("&score=");
    QString score = remainscore->text().trimmed();
    surl.append(score);
    surl.append("&month=");
    QString smonth = numofmonth->text().trimmed();
    surl.append(smonth);

    requtil = new ReqUtil(this);
    connect(requtil,SIGNAL(reqfinished()),this,SLOT(doConfirmReply()));

    QUrl url= QUrl::fromEncoded(surl.toUtf8());
    requtil->startRequest(url);
}

// ȷ�������ش�
void ScoreToVipDialog::doConfirmReply(){

     QVariantMap varMap = requtil->getReply();
     QVariant verror = varMap["error"];
     QString error = qvariant_cast<QString>(verror);
     if(error == "The server find errors when compute the score, please contact Admimistrator"){
         QMessageBox::warning(this, tr("Information"), tr("The server find errors when compute the score, please logo off try Again or Contact Admimistrator"), QMessageBox::Yes);
         return;
     }
     if(error == "Email Is Not Existed"){
         QMessageBox::warning(this, tr("Information"), tr("User Is Not Existed."), QMessageBox::Yes);
         return;
     }

     if(error.isEmpty()){
         // д��vip��Ϣ��������Ϣ
         // ����ѡ���docUuid
         Preferences* p = Preferences::instance();
         QVariant vscore = varMap["score"];
         QString score = qvariant_cast<QString>(vscore);
         p->setScore(score);
         // ����Setting �����û���Ϣ
         QVariant vdt_expired = varMap["dt_expired"];
         QString dt_expired = qvariant_cast<QString>(vdt_expired);
         p->setDtExpired(dt_expired);

         QMessageBox::information(this, tr("Information"), tr("ExChange Success."), QMessageBox::Yes);
         update = false;
         this->close();
     }
}

// ������·�
void ScoreToVipDialog::countMonth(){

    QString inscore = inputscore->text().trimmed();
    if(inscore.isEmpty()){
       return;
    }

    int intscore = inscore.toInt();
    if(intscore == 0){
        QMessageBox::warning(this, tr("Warning"), tr("The Input Score is 0."), QMessageBox::Yes);
        return;
    }

    QString stotalscore = totalscore->text().trimmed();
    int totalintscore = stotalscore.toInt();

    if(intscore > totalintscore){
        QMessageBox::warning(this, tr("Warning"), tr("The Input Score is Bigger than your Toatal Score."), QMessageBox::Yes);
        return;
    }

    numofmonth->setText(QString::number(intscore/ 1000));
    int iremainscore = totalintscore - numofmonth->text().toInt()* 1000;
    remainscore->setText(QString::number(iremainscore));
    return;
}

// ȡ����ť
void ScoreToVipDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}



