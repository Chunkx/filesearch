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

#include "propofdirdialog.h"
#include "utils.h"
#include "fileutils.h"

PropOfDirDialog::PropOfDirDialog(QWidget * parent, const QString & basedir,const QString & dir)
	: QDialog(parent),
          m_parent(parent),
          m_baseDir(basedir),
          m_dir(dir),update(false)
{
        setupUi(this);

        // Set UI
        // ����Ŀ��Ŀ¼����
        dirName->setText(Utils::getDirNameByPath(m_dir));
        location->setText(m_dir);

        this->setWindowIcon(Utils::getIcon("folder.ico"));
        this->setWindowTitle(tr("Properties Of Directory"));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// �������ļ���
void PropOfDirDialog::confirmBtn_clicked(){
    update = false;
    this->close();
}

// ȡ����ť
void PropOfDirDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}



