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

// �ƶ������ļ��жԻ���
MoveToDirDialog::MoveToDirDialog(QWidget * parent, const QString & basedir,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_baseDir(basedir),
          m_dir(dir),update(false)
{
	setupUi(this);

        // ����Ŀ��Ŀ¼
        QString tdestDir = m_dir;
        tdestDir = tdestDir.remove(0, m_baseDir.length());
        sourceDir->setText(tdestDir);

        q_myTreeList = new myTreeList("", this);
        q_myTreeList->setStyleSheet(
                    "QTreeView::branch {image:none;}"
                    "QTreeView::item{height: 25px;}"
                    "QTreeView::item:hover{background-color:rgb(100,100,100)}"
                    "QTreeView::item:selected{background-color:rgb(128,128,128)}"
        );

        // չ�����ڵ�
        q_myTreeList->loadDirByLay(Utils::getLocatePath(), 0, 0);
        q_myTreeList->enableMouse(true);

        q_myTreeList->expandAll();

        verticalLayout_2->addWidget(q_myTreeList);

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
    QString curPath = q_myTreeList->getCurPath();
    m_curPath = curPath;
    if(m_dir == curPath || curPath == "alldocs" || curPath == "alltags"){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the other Directory"), QMessageBox::Yes);
        return;
    }

    // �����ļ�
    QString temp = dirname;
    temp = temp.right(temp.length() - temp.lastIndexOf(QDir::separator()) - 1);
    QString curSelPath = curPath;
    curSelPath.append(QDir::separator()).append(temp);
    m_seldir = curSelPath;
    if(!FileUtils::copyDirectoryFiles(m_dir, curSelPath, false)){
        QMessageBox::warning(this, tr("Error"), tr("Copy Files failed. "), QMessageBox::Yes);
        return;
    }else{
        // ɾ��ԭ��Ŀ¼���ڵ�
        QFileInfo fileinfo(m_dir);
        FileUtils::deleteDirectory(fileinfo);
        update = true;
        // ֪ͨ��������ˢ�� TODO

        this->close();
    }

}

// ȡ����ť
void MoveToDirDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}

// �½��ļ��а�ť
void MoveToDirDialog::newDirBtn_clicked(){

    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alltags") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            // ˢ��ѡ�е���
            QString tempPath = curPath;
            tempPath.append(QDir::separator()).append(dlg.dirName->text());

            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->addItemByParentItem(curItem, dlg.dirName->text(), tempPath, "expander_normal.png");
            curItem->setIcon(Utils::getIcon("expander_open.png"));
            q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }

}




