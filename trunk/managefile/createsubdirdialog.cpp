/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/
#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QUuid>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QUrl>

#include "createsubdirdialog.h"
#include "utils.h"
#include "fileutils.h"
#include "printerwidget.h"
#include "db/dirdao.h"

CreateSubDirDialog::CreateSubDirDialog(QWidget * parent, const QString & curUuid,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_curUuid(curUuid),
          m_dir(dir),update(false)
{
	setupUi(this);

        // Set UI
        // ����Ŀ��Ŀ¼
        destDir->setText(m_dir);

        this->setWindowIcon(Utils::getIcon("folder.ico"));
        this->setWindowTitle(tr("Create New Sub Directory"));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// �������ļ���
void CreateSubDirDialog::confirmBtn_clicked(){
    // �ļ������Ƽ��
    QString dirname = dirName->text();
    if(dirname.isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Input Directory Name: "), QMessageBox::Yes);
        return;
    }

    // �½��ļ���
    Dir dir;
    dir.DIR_GUID = QUuid::createUuid();
    m_newUuid = dir.DIR_GUID ;
    dir.DIR_ICON = "folder.ico";
    dir.DIR_NAME = dirname;
    dir.DIR_PARENT_UUID = m_curUuid;
    DirDao::insertDir(dir);
    update = true;
    this->close();

}

// ȡ����ť
void CreateSubDirDialog::cancelBtn_clicked(){

    PrinterWidget diew = new PrinterWidget(this);
    diew.printer();

//     update = false;
//     this->close();
}



