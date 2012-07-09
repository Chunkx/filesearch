#include "PropOfDirDialog.h"

#include <QDir>
#include <QMessageBox>
#include "db/docdao.h"
#include "db/dirdao.h"
#include "db/relatedocdao.h"
#include "utils.h"

PropOfDirDialog::PropOfDirDialog (QWidget *parent, const QString & curUuid)
    : QDialog(parent), m_curUuid(curUuid),update(false)
{
    setupUi (this);

    // ȡ�õ�ǰ�ĵ�
    Dir dir = DirDao::selectDir(m_curUuid);

    setWindowTitle (dir.DIR_NAME +" - " +tr ("Property Editor","Window caption"));


    // ���û�����Ϣ
    name->setText(dir.DIR_NAME);
    uuid->setText(dir.DIR_GUID);
    icon->setIcon(Utils::getIcon(dir.DIR_ICON));

    // ȡ�õ�ǰ·��
    QList<Dir> dirList;
    DirDao::selectAllParentDirbyDir(dirList, m_curUuid);

    QString slocation;
    slocation.append(QDir::separator());
    for (int var = dirList.length() - 1; var >= 0 ; --var) {
        Dir dir = dirList.at(var);
        slocation.append(dir.DIR_NAME);
        slocation.append(QDir::separator());
    }
    location->setText(slocation);
    desp->setText(dir.DIR_DESCRIPTION);

    // ���ñ���
    QList<Dir> selDirList;
    DirDao::selectAllSubDirbyDir(selDirList, m_curUuid, "0");
    DirDao::selectAllSubDirbyDir(selDirList, m_curUuid, "1");
    dirCount->setText(QString::number(selDirList.size()));

    Dir tempDir;
    int ifileCount = 0;
    int ifileSize = 0;
    foreach(tempDir, selDirList){
        QList<Doc> docs = DocDao::selectDocsbyDir(tempDir.DIR_GUID, "0");
        QList<Doc> deldocs = DocDao::selectDocsbyDir(tempDir.DIR_GUID, "1");
        ifileCount = docs.size() + deldocs.size() + 1;
        Doc doc;
        foreach(doc, docs){
            // ����size K
            QFileInfo file(doc.DOCUMENT_LOCATION);
            ifileSize = ifileSize + file.size();
        }
        foreach(doc, deldocs){
            // ����size K
            QFileInfo file(doc.DOCUMENT_LOCATION);
            ifileSize = ifileSize + file.size();
        }
    }
    fileCount->setText(QString::number(ifileCount));
    fileSize->setText(QString::number(ifileSize/1024).append(" K"));

    // ����
    // MD5����
    QString dirprotect = dir.DIR_PROTECT;
    if(dirprotect.isEmpty()){
        // δ���ñ���
        isProtect->setChecked(false);
        orgpwd->setText("");
        pwd->setText("");
        repwd->setText("");

        orgpwd->setDisabled(true);
        pwd->setDisabled(true);
        repwd->setDisabled(true);
    }else{
        // ���ù�����
        isProtect->setChecked(true);
        orgpwd->setText("password");
        orgpwd->setDisabled(false);
        pwd->setDisabled(false);
        repwd->setDisabled(false);
    }

    // ����ͬ��
    localVer->setText(QString::number(dir.MF_VERSION));

    // �ӷ�������ȡ�����Ա�Version���Ƚϰ汾 �����µ���ͬ��
    // remoteVer->setText();
    connect(closeBtn,SIGNAL(clicked()),this, SLOT(closeBtn_clicked()));
    connect(applyBtn,SIGNAL(clicked()),this, SLOT(applyBtn_clicked()));
    connect(isProtect,SIGNAL(clicked()),this, SLOT(setProtect()));


    // Ĭ��ѡ�е�һ��tab
    tabWidget->setCurrentIndex(0);

}

// �����ļ���
void PropOfDirDialog::setProtect()
{
    if(orgpwd->isEnabled()){
       orgpwd->setDisabled(true);
    }else{
       orgpwd->setDisabled(false);
    }

    if(pwd->isEnabled()){
       pwd->setDisabled(true);
    }else{
       pwd->setDisabled(false);
    }

    if(repwd->isEnabled()){
       repwd->setDisabled(true);
    }else{
       repwd->setDisabled(false);
    }

}

// ����������
void PropOfDirDialog::applyBtn_clicked()
{
     // �������
    // ȡ�õ�ǰ�ĵ�
    Dir dir = DirDao::selectDir(m_curUuid);

     QString sorgpwd = orgpwd->text();
     if(!sorgpwd.isEmpty()){
        // �жϸ����ݿ�һ��
         QString dbprotect = dir.DIR_PROTECT;
         QString md5pwd = Utils::getMD5Str(sorgpwd);
         if(dbprotect != md5pwd){
             QMessageBox::warning(this, tr("Warning"), tr("The Org password is wrong, Please Confirm"), QMessageBox::Yes);
             return;
         }
     }


     QString srepwd = repwd->text();
     QString spwd = pwd->text();
     QString protectStr = "";

     if(!srepwd.isEmpty() && !spwd.isEmpty()){
         if(srepwd != spwd){
             QMessageBox::warning(this, tr("Warning"), tr("The password you entered must be the same as the  former"), QMessageBox::Yes);
             return;
         }else{
             protectStr =  Utils::getMD5Str(srepwd);
         }
     }


     dir.DIR_NAME = name->text();
     dir.DIR_DESCRIPTION = desp->toPlainText();
     dir.DIR_ICON = icon->icon().name();
     dir.DIR_PROTECT = protectStr;
     dir.MF_VERSION = dir.MF_VERSION + 1;

     DirDao::updateDir(dir);

     update = true;
     this->close();
}


void PropOfDirDialog::closeBtn_clicked()
{
    update = true;
    this->close();
}

