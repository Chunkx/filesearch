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


#include "doctodirdialog.h"
#include "createsubdirdialog.h"
#include "preferences.h"
#include "utils.h"
#include "fileutils.h"
#include "mytreeview.h"
#include "db/dirdao.h"
#include "db/docdao.h"

// �ƶ������ļ��жԻ���
DocToDirDialog::DocToDirDialog(QWidget * parent, const QString & docName, const QString & docUuid,  const bool isCopy)
	: QDialog(parent),
	  m_parent(parent),
          m_docName(docName),
          m_selDocUuid(docUuid),
          m_isCopy(isCopy), update(false)
{
	setupUi(this);

        // �����ļ���
        QFileInfo file(m_docName);
        if(file.exists()){
            sourceDoc->setText(file.fileName());
            sourceDoc->setToolTip(docName);
        }

        q_myTreeList = new MyTreeView("doc", this);
        // չ�����ڵ�
        q_myTreeList->enableMouse(true);

        q_myTreeList->expandAll();

        verticalLayout_2->addWidget(q_myTreeList);

        this->setWindowIcon(Utils::getIcon("folder.ico"));
        if(m_isCopy){
            this->setWindowTitle(tr("Copy the Document to Directory"));
        }else{
            this->setWindowTitle(tr("Move the Document to Directory"));
        }


        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
        connect(newDirBtn,SIGNAL(clicked()),this,SLOT(newDirBtn_clicked()));
}

// ��ʼ�����ļ�
void DocToDirDialog::confirmBtn_clicked(){

    // û�иı�Path
    QString curUuid = q_myTreeList->getCurUuid();
    Doc doc = DocDao::selectDoc(m_selDocUuid);
    if(curUuid == doc.DIR_GUID){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the other Directory"), QMessageBox::Yes);
        return;
    }

    // ����
    if(m_isCopy){
        doc.DOCUMENT_GUID = QUuid::createUuid().toString();
        doc.DELETE_FLAG = "0";
        doc.DIR_GUID = curUuid;
        DocDao::insertDoc(doc);
    }else{
        // �ƶ�
        doc.DIR_GUID = curUuid;
        doc.DELETE_FLAG = "0";
        doc.MF_VERSION = doc.MF_VERSION + 1;
        DocDao::updateDoc(doc);
    }

    update = true;
    // ֪ͨ��������ˢ�� TODO
    this->close();

}

// ȡ����ť
void DocToDirDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}

// �½��ļ��а�ť
void DocToDirDialog::newDirBtn_clicked(){

    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curType.isEmpty() && curType != "alltags" && curType != "basket") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, q_myTreeList->getCurUuid(), q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){

            // �����ļ���
            Dir dir;
            dir.DIR_GUID = QUuid::createUuid().toString();
            dir.DIR_PARENT_UUID = q_myTreeList->getCurUuid();
            dir.DIR_NAME = dlg.dirName->text();
            dir.DIR_DESCRIPTION = "";
            dir.DIR_ICON = "folder.ico";
            dir.DIR_ORDER = 0;
            dir.MF_VERSION = 0;
            DirDao::insertDir(dir);

            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->addItemByParentItem(curItem, dlg.dirName->text(), dir.DIR_GUID, "doc", "folder.ico");

            q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }

}




