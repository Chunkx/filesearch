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

#include "movetotagdialog.h"
#include "preferences.h"
#include "utils.h"
#include "mytreeview.h"
#include "fileutils.h"
#include "db/tagdao.h"

// �ƶ������ļ��жԻ���
MoveToTagDialog::MoveToTagDialog(QWidget * parent, const QString & uuId)
	: QDialog(parent),
	  m_parent(parent),
          m_uuId(uuId),
          update(false)
{
        setupUi(this);

        q_myTreeList = new MyTreeView("tag", this);
        // չ�����ڵ�
        q_myTreeList->enableMouse(true);
        q_myTreeList->expandAll();
        movtoTagTree->addWidget(q_myTreeList);

        this->setWindowIcon(Utils::getIcon("tag.ico"));
        this->setWindowTitle(tr("Move the Tag"));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// ��ʼ����Tag
void MoveToTagDialog::confirmBtn_clicked(){
    // û�иı�Path
    QString uuId = q_myTreeList->getCurUuid();
    m_selUuId = uuId;
    if(m_uuId == uuId){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the other Tag"), QMessageBox::Yes);
        return;
    }

    // �������ݿ�
    Tag tag = TagDao::selectTag(m_uuId);
    tag.TAG_GROUP_GUID = m_selUuId;
    tag.MF_VERSION = tag.MF_VERSION + 1;

    bool success  = TagDao::updateTag(tag);
    if(success){
        update = true;
        this->close();
    }
}

// ȡ����ť
void MoveToTagDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}




