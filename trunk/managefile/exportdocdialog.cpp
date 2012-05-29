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

#include "exportdocdialog.h"
#include "preferences.h"
#include "utils.h"
#include "fileutils.h"

ExportDocDialog::ExportDocDialog(QWidget * parent, const QString & basedir,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_baseDir(basedir),
          m_dir(dir),update(false)
{
	setupUi(this);
        inclueSubDirChk->setChecked(true);
        createSubDirChk->setChecked(true);

        // Set UI
        // ����Ŀ��Ŀ¼
        QString tdestDir = m_dir;
        tdestDir = tdestDir.remove(0, m_baseDir.length());
        destDir->setText(dir);

        // �б�
        model = new QStandardItemModel();
        // �½�Model
        model->setColumnCount(1);

        this->setWindowIcon(Utils::getIcon("document-import.png"));
        this->setWindowTitle(tr("Doc Export"));

        progressBar->hide();
        pgfilename->hide();
        cancelled = false;

        connect(fileSelBtn, SIGNAL(clicked()), this, SLOT(fileSelBtn_clicked()));
        connect(previewBtn, SIGNAL(clicked()), this, SLOT(previewBtn_clicked()));
        connect(delBtn, SIGNAL(clicked()), this, SLOT(delBtn_clicked()));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// ѡ�񵼳�·��
void ExportDocDialog::fileSelBtn_clicked()
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

// Ԥ���ļ��б�
void ExportDocDialog::previewBtn_clicked()
{

   // �Զ����ų��ļ�����
   QString sexcludefileType = excludefileType->text();
   QStringList customexList;
   if(!sexcludefileType.isEmpty()){
       customexList = sexcludefileType.split(",");
   }

   // model
   model->clear();

   // �Ƿ������Ŀ¼
   if(inclueSubDirChk->isChecked()){
       loadFiles(m_dir, customexList, model);
   } else {
       // ��files.dat�ļ�
       QString path = m_dir;
       path.append(QDir::separator()).append("files.dat");
       QFile datfile(path);
       if (datfile.exists()){
           path = QDir::toNativeSeparators(path);
           // ��ȡdat�ļ�
           QStringList files = FileUtils::readFile(path);
           for (int var = 0; var < files.length(); ++var) {
               QString file = files.at(var);
               if(!file.isEmpty()){
                   QStringList arr = file.split(",");
                   QString filepath = arr[1];
                   int dotpos = filepath.lastIndexOf(".");
                   QString suffix = filepath.right(filepath.length() - dotpos);
                   suffix = "*" + suffix;
                   // �ų��ֶ���ȥ�ļ�����
                   if(!customexList.contains(suffix)){
                      // ȡ���ļ���
                       QString filename = arr[1].right(arr[1].length() - arr[1].lastIndexOf(QDir::separator()));

                       // ȡ��BaseDir
                       QString tdestDir = m_dir;
                       tdestDir = tdestDir.remove(0, m_baseDir.length());

                       QStandardItem* item = new QStandardItem(tdestDir + filename);
                       item->setData(arr[1], Qt::ToolTip);
                       QList<QStandardItem*> items;
                       items << item;
                       model->appendRow(items);

                   }
               }
           }
       }
   }

   previewView->setModel(model);
   // ���õ�һ�еĿ��
   int tablewidth = this->previewView->width();
   this->previewView->setColumnWidth(0, tablewidth * 1);
}

//// �����ļ���Ŀ¼ȡ�õݹ�õ���Ŀ¼���ṹ
int ExportDocDialog::loadFiles(QString parentPath, QStringList excludeTypeList, QStandardItemModel *model){
    //Ŀ¼
    QDir dir(parentPath);
    if (!dir.exists()) {
        return -1;
    }
    // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    //ת����һ��list
    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i< list.size(); i++){
        QFileInfo fileInfo = list.at(i);
        QString datfilepath = fileInfo.filePath();
        //������ļ���
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            loadFiles(datfilepath, excludeTypeList, model);
        }else{
            // ��files.dat�ļ�
            if (datfilepath.indexOf("files.dat") != -1 ){
                datfilepath = QDir::toNativeSeparators(datfilepath);
                // ��ȡdat�ļ�
                QStringList files = FileUtils::readFile(datfilepath);
                for (int var = 0; var < files.length(); ++var) {
                    QString file = files.at(var);
                    if(!file.isEmpty()){
                        QStringList arr = file.split(",");
                        QString filepath = arr[1];
                        int dotpos = filepath.lastIndexOf(".");
                        QString suffix = filepath.right(filepath.length() - dotpos);
                        suffix = "*" + suffix;
                        // �ų��ֶ���ȥ�ļ�����
                        if(!excludeTypeList.contains(suffix)){
                           // ȡ���ļ���
                            QString filename = arr[1].right(arr[1].length() - arr[1].lastIndexOf(QDir::separator()) - 1);

                            // ȡ��BaseDir
                            QString tdestDir = datfilepath;
                            tdestDir = tdestDir.remove("files.dat");
                            tdestDir = tdestDir.mid(m_baseDir.length() + 1, tdestDir.length());

                            QStandardItem* item = new QStandardItem(tdestDir + filename);
                            item->setData(arr[1], Qt::ToolTip);
                            QList<QStandardItem*> items;
                            items << item;
                            model->appendRow(items);

                        }
                    }
                }
            }
        }
    }
}

// ɾ���ļ���ť
void ExportDocDialog::delBtn_clicked(){

    QItemSelectionModel *selections = previewView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QMap<int, int> rowMap;
    foreach (QModelIndex index, selected)
    {
        rowMap.insert(index.row(), 0);
    }

    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious())
    {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        model->removeRow(rowToDel);
    }
}

//// ȷ����ť �����ļ���ʡ��д�뵽����Ŀ¼�������ļ��ܴ���Ҫ�н�������
void ExportDocDialog::confirmBtn_clicked(){
    // ���û��ѡ����Ŀ¼
    if(m_exportDir.isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the Path to Export Into.."), QMessageBox::Yes);
        return;
    }

    // �Ƿ񴴽����ļ���
    bool isCreateSubDir = createSubDirChk->isChecked();

    // ���ý���Ԫ�ز�����
    this->setEnabled(false);

    pgfilename->show();
    progressBar->show();

    // Ŀ¼����
    model->sort(0, Qt::DescendingOrder);
    // �����ļ�
    int row= model->rowCount();
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setRange(0, row);
     // ȡ�õ������ļ��б�
    for (int var = 0; var < row; ++var) {
         QStandardItem* temp = model->item(var);
         QString filepath = qvariant_cast<QString>(temp->data(Qt::ToolTip));
         QString displayfilepath = qvariant_cast<QString>(temp->data(Qt::DisplayRole));
         pgfilename->setText(displayfilepath);
         // �Ƿ񴴽����ļ���
         if(isCreateSubDir){
            QString destDir = m_exportDir + displayfilepath.left(displayfilepath.lastIndexOf(QDir::separator()));
            // ������Ŀ¼
            FileUtils::copyFileToDir(filepath, destDir, true);
         }else{
            FileUtils::copyFileToDir(filepath, m_exportDir, true);
         }
         setProgress(var);
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
bool ExportDocDialog::setProgress(int p)
{
        if (cancelled){
            return false;
        }
        progressBar->setValue(p);
        return true;
}

// ȡ����ť
void ExportDocDialog::cancelBtn_clicked(){
     cancelled = true;
     update = false;
     this->close();
}



