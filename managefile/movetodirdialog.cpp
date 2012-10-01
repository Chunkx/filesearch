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

#include "movetodirdialog.h"
#include "createsubdirdialog.h"
#include "preferences.h"
#include "utils.h"
#include "fileutils.h"
#include "mytreeview.h"
#include "db/dirdao.h"

// �ƶ������ļ��жԻ���
MoveToDirDialog::MoveToDirDialog(QWidget * parent, const QString & uuId,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_curUuid(uuId),
          m_dir(dir),update(false)
{
        setupUi(this);

        // ����Ŀ��Ŀ¼
        sourceDir->setText(m_dir);

        q_myTreeList = new MyTreeView("doc", this);
        // չ�����ڵ�
        q_myTreeList->enableMouse(true);

        q_myTreeList->expandAll();

        treeLayout->addWidget(q_myTreeList);

        m_newUuid = "";

        this->setWindowIcon(Utils::getIcon("folder.ico"));
        this->setWindowTitle(tr("Move the Sub Directory"));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
        connect(newDirBtn,SIGNAL(clicked()),this,SLOT(newDirBtn_clicked()));
}

// ��ʼ�ƶ��ļ���
void MoveToDirDialog::confirmBtn_clicked(){
    // �ļ������Ƽ��
    QString dirname = sourceDir->text();
    if(dirname.isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select Need Moved Directory"), QMessageBox::Yes);
        return;
    }
    // û�иı�Path
    QString curUuid = q_myTreeList->getCurUuid();
    if(curUuid == m_curUuid){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the other Directory"), QMessageBox::Yes);
        return;
    }

    // ɾ��ԭ��Ŀ¼���ڵ�
    Dir dir = DirDao::selectDir(m_curUuid);
    dir.DIR_PARENT_UUID = curUuid;
    dir.MF_VERSION = dir.MF_VERSION + 1;
    dir.DELETE_FLAG = "0";
    m_toUuid = curUuid;
    DirDao::updateDir(dir);
    update = true;
    // ֪ͨ��������ˢ�� TODO
    this->close();

}

// ȡ����ť
void MoveToDirDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}

// �½��ļ��а�ť
void MoveToDirDialog::newDirBtn_clicked(){

    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curType.isEmpty() && curType != "alltags" && curType != "basket") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, q_myTreeList->getCurUuid(), q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            // ˢ��ѡ�е���
            m_newUuid = dlg.m_newUuid;

            m_parent_newUuid = q_myTreeList->getCurUuid();
            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->addItemByParentItem(curItem, dlg.dirName->text(), m_newUuid, "doc", "folder.ico");
            q_myTreeList->expand(q_myTreeList->getCurIndex());

        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }

}




