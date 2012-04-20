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

#include "exportconvertdialog.h"
#include "preferences.h"
#include "utils.h"
#include "fileutils.h"

ExportConvertDialog::ExportConvertDialog(QWidget * parent, const QString & dir)
	: QDialog(parent),
          m_parent(parent),
          m_dir(dir),update(false)
{
	setupUi(this);

        // Set UI
        // ����Ŀ��Ŀ¼
        QString tdestDir = m_dir;

        // convertTypeCmb
        convertTypeCmb->clear();
        convertTypeCmb->insertItems(0, QStringList()
         << ""
         << tr("Convert Office to Pdf")
         << tr("Convert Pdf to Office")
         << tr("Convert Picture to Pdf")
         << tr("Convert Ppt to Picture")
        );
        QFileInfo file(m_dir);
        this->setWindowIcon(Utils::getIcon("document-import.png"));
        this->setWindowTitle(tr("Doc Export -") + file.fileName() );

        progressBar->hide();
        pgfilename->hide();
        cancelled = false;

        connect(fileSelBtn, SIGNAL(clicked()), this, SLOT(fileSelBtn_clicked()));
        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// ѡ�񵼳�·��
void ExportConvertDialog::fileSelBtn_clicked()
{
  QString path = QFileDialog::getExistingDirectory(this, tr("Select existing Export directory"),
                                                   tr("Choose a directory"), QFileDialog::ShowDirsOnly);
   if (path.isEmpty()){
        return;
   }
   path = QDir::toNativeSeparators(path);
   // �趨����·��
   fileSelDir->setText(path);
   m_exportDir = path;
}

//// ȷ����ť �����ļ���ʡ��д�뵽����Ŀ¼��ת���ļ��ܴ���Ҫ�н�������
void ExportConvertDialog::confirmBtn_clicked(){
     // ���û��ѡ����Ŀ¼
    if(m_exportDir.isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the Path to Export Into.."), QMessageBox::Yes);
        return;
    }


    // ���ý���Ԫ�ز�����
    this->setEnabled(false);

    pgfilename->show();
    progressBar->show();

    // �����ļ�
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setRange(0, 100);
     // ȡ��ת��
    int index = convertTypeCmb->currentIndex();
    QFileInfo file(m_dir);
     if(index == 0){
        QString filepath = file.absoluteFilePath();
        QString displayfilepath = file.fileName();
        pgfilename->setText(displayfilepath);
        FileUtils::copyFileToDir(filepath, m_exportDir, true);
        setProgress(100);
    }else{
         // ת��
         // Convert Office to Pdf
         if(index == 1){

         }
         // Convert Pdf to Office
         if(index == 2){

         }
         // Convert Picture to Pdf
         if(index == 3){

         }
         // Convert Ppt to Picture
         if(index == 4){

         }
    }

     update = true;
     // ��ʾ�������
     int com = QMessageBox::information(this, tr("Message"), tr("Documents Exported Success!"), QMessageBox::Yes);
     if (com == QMessageBox::Yes)
     {
            this->close();
            QDesktopServices::openUrl(QUrl("file:///" + m_exportDir));
            return;
     }
}

// ���ý�������
bool ExportConvertDialog::setProgress(int p)
{
        if (cancelled){
            return false;
        }
        progressBar->setValue(p);
        return true;
}

// ȡ����ť
void ExportConvertDialog::cancelBtn_clicked(){
     cancelled = true;
     update = false;
     this->close();
}



