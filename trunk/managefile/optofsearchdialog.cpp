#include "optOfsearchdialog.h"

#include <QDir>
#include <QMessageBox>
#include <QThread>
#include "db/docdao.h"
#include "db/dirdao.h"
#include "db/relatedocdao.h"
#include "utils.h"
#include "indexfile.h"
#include "fileutils.h"

OptOfSearchDialog::OptOfSearchDialog (QWidget *parent)
    : QDialog(parent), update(false)
{
    setupUi (this);

    // ȡ�õ�ǰ�ĵ�
    setWindowTitle (tr("Option of Search"));

    int indexdoc = DocDao::selectCountByIndexflg("1");
    int waitindexdoc = DocDao::selectCountByIndexflg("0");

    indexedfileCount->setText(QString::number(indexdoc));
    waitIndexfileCount->setText(QString::number(waitindexdoc));
    connect(closeBtn,SIGNAL(clicked()),this, SLOT(closeBtn_clicked()));

    progressBar->hide();
    // Ĭ��ѡ�е�һ��tab
    tabWidget->setCurrentIndex(0);

    connect(updateBtn,SIGNAL(clicked()),this, SLOT(updateBtn_clicked()));
    connect(reCreateBtn,SIGNAL(clicked()),this, SLOT(reCreateBtn_clicked()));
}

void OptOfSearchDialog::closeBtn_clicked()
{
    update = true;
    this->close();
}

void OptOfSearchDialog::updateBtn_clicked()
{
   int waitindexdoc = DocDao::selectCountByIndexflg("0");
   if(waitindexdoc == 0){
       QMessageBox::warning(this, tr("Warning"), tr("There is no document waiting for indexed"), QMessageBox::Yes);
       return;
   }

   QMessageBox closeConfirmation(this);
   closeConfirmation.setWindowFlags(Qt::Sheet);
   closeConfirmation.setWindowTitle(tr("Do you really want to update index?"));
   closeConfirmation.setInformativeText(tr("Update Index file will cost time.\n"
                                            "Do you really want to update the index file?\n"));
   closeConfirmation.setIcon(QMessageBox::Question);
   closeConfirmation.addButton(QMessageBox::Yes);
   closeConfirmation.addButton(QMessageBox::No);
   closeConfirmation.setEscapeButton(QMessageBox::No);
   if (closeConfirmation.exec() == QMessageBox::No){
      return;
   }
   updateBtn->setDisabled(true);
   reCreateBtn->setDisabled(true);
   closeBtn->setDisabled(true);

   progressBar->show();
   progressBar->setWindowModality(Qt::WindowModal);
   progressBar->setRange(0, waitindexdoc);

   QList<Doc> waitindexdocList = DocDao::selectDocsByIndexFlag("0");
   // �������ڽ����������
   Preferences* p = Preferences::instance();
   p->setIsIndexing(true);

   IndexFilesObj indexFilesObj;
   for (int var = 0; var < waitindexdocList.size(); ++var) {
       Doc doc = waitindexdocList.at(var);
       indexFilesObj.indexfile(doc);
       progressBar->setValue(var);
   }
   progressBar->setValue(waitindexdoc);
   p->setIsIndexing(false);

   QMessageBox::information(this, tr("Information"), tr("Update Index file Success."), QMessageBox::Yes, QMessageBox::Yes);
   // ���½���
   int uindexdoc = DocDao::selectCountByIndexflg("1");
   int uwaitindexdoc = DocDao::selectCountByIndexflg("0");

   indexedfileCount->setText(QString::number(uindexdoc));
   waitIndexfileCount->setText(QString::number(uwaitindexdoc));

   updateBtn->setDisabled(false);
   reCreateBtn->setDisabled(false);
   closeBtn->setDisabled(false);
}

void OptOfSearchDialog::reCreateBtn_clicked()
{
    QMessageBox closeConfirmation(this);
    closeConfirmation.setWindowFlags(Qt::Sheet);
    closeConfirmation.setWindowTitle(tr("Do you really want to recreate index file?"));
    closeConfirmation.setInformativeText(tr("ReCreate Index file will cost time.\n"
                                             "Do you really want to recreate the index file?\n"));
    closeConfirmation.setIcon(QMessageBox::Question);
    closeConfirmation.addButton(QMessageBox::Yes);
    closeConfirmation.addButton(QMessageBox::No);
    closeConfirmation.setEscapeButton(QMessageBox::No);
    if (closeConfirmation.exec() == QMessageBox::No){
       return;
    }
    // ���������ļ���δ��������״̬
    DocDao::updateDocIndexflg("0");

    updateBtn->setDisabled(true);
    reCreateBtn->setDisabled(true);
    closeBtn->setDisabled(true);

    // ɾ������Ŀ¼
    QString indexpath = Utils::getLocateIndexPath();
    FileUtils::removeDirectory(indexpath);

    // �������ڽ����������
    Preferences* p = Preferences::instance();
    p->setIsIndexing(true);

    QList<Doc> waitindexdocList = DocDao::selectDocsByIndexFlag("0");
    progressBar->show();
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setRange(0, waitindexdocList.size());
    int sumofindexfile = waitindexdocList.size();

    IndexFilesObj indexFilesObj;
    for (int var = 0; var < waitindexdocList.size(); ++var) {
        QThread::currentThread()->wait(100);
        Doc doc = waitindexdocList.at(var);
        indexFilesObj.indexfile(doc);
        progressBar->setValue(var);
    }

    progressBar->setValue(sumofindexfile);
    p->setIsIndexing(false);

    QMessageBox::information(this, tr("Information"), tr("ReCreate Index file Success."), QMessageBox::Yes, QMessageBox::Yes);
    // ���½���
    int uindexdoc = DocDao::selectCountByIndexflg("1");
    int uwaitindexdoc = DocDao::selectCountByIndexflg("0");

    indexedfileCount->setText(QString::number(uindexdoc));
    waitIndexfileCount->setText(QString::number(uwaitindexdoc));

    updateBtn->setDisabled(false);
    reCreateBtn->setDisabled(false);
    closeBtn->setDisabled(false);

}


