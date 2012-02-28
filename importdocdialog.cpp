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

#include "importdocdialog.h"
#include "preferences.h"

ImportDocDialog::ImportDocDialog(QWidget * parent, const QString & basedir,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_baseDir(basedir),
          m_dir(dir),update(false)
{
	setupUi(this);
        inclueSubDirCheck->setChecked(true);

        // fileTypeCmb
        fileTypeCmb->clear();
        fileTypeCmb->insertItems(0, QStringList()
         << QApplication::translate("ImportDocDialog", "All files(*.*)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "All supported files", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Office files(*.doc,*.docx,*.xls,*.xlsx,*.ppt,*.pptx)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Html files(*.html,*.htm)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Picture file(*.jpeg,*.jpg,*.png,*.bmp,*.gif)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Flash files(*.flv,*.swf)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Source files(*.cpp,*.h,*.hpp,*.c,*.java,*.js,*.cs,*.vbs,*.sql,*.rb,*.py,*.asp,*.aspx,*.jsp,*.php,*.xml)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Txt files(*.txt,*.ini,*.bar,*.inf)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ImportDocDialog", "Movie fies(*.rm,*.rmvb,*.avi,*.mpg,*.mlv,*.mpe,*.mpeg,*.m2v)", 0, QApplication::UnicodeUTF8)
        );


        Preferences* p = Preferences::instance();
        fileTypeCmb->insertItems(10, p->allsupported());

        // Set UI
        // ����Ŀ��Ŀ¼
        QString tdestDir = m_dir.remove(0, m_baseDir.length());
        destDir->setText(tdestDir);

        // �б�
        model = new QStandardItemModel();
        // �½�Model
        model->setColumnCount(1);

        connect(fileSelBtn, SIGNAL(clicked()), this, SLOT(fileSelBtn_clicked()));
        connect(previewBtn, SIGNAL(clicked()), this, SLOT(previewBtn_clicked()));
        connect(delBtn, SIGNAL(clicked()), this, SLOT(delBtn_clicked()));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));



//	connect(fileButton, SIGNAL(clicked()), this, SLOT(fileButton_clicked()));
//	connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAccepted()));
//	connect(tabWidget, SIGNAL(currentChanged(int)),
//			this, SLOT(createPreview(int)));
//	// csv
//	connect(pipeRadioButton, SIGNAL(clicked(bool)),
//			this, SLOT(createPreview(bool)));
//	connect(commaRadioButton, SIGNAL(clicked(bool)),
//			this, SLOT(createPreview(bool)));
//	connect(semicolonRadioButton, SIGNAL(clicked(bool)),
//			this, SLOT(createPreview(bool)));
//	connect(tabelatorRadioButton, SIGNAL(clicked(bool)),
//			this, SLOT(createPreview(bool)));
//	connect(customRadioButton, SIGNAL(clicked(bool)),
//			this, SLOT(createPreview(bool)));
//	connect(customEdit, SIGNAL(textChanged(QString)),
//			this, SLOT(customEdit_textChanged(QString)));
//	connect(skipHeaderCheck, SIGNAL(toggled(bool)),
//			this, SLOT(skipHeaderCheck_toggled(bool)));

        //skipHeaderCheck_toggled(false);
}

// ѡ��·��
void ImportDocDialog::fileSelBtn_clicked()
{
  QString path = QFileDialog::getExistingDirectory(this,"get existing doc directory",
                                                   "Choose a directory", QFileDialog::ShowDirsOnly);
   if (path.isEmpty()){
        return;
   }
   // �趨·��
   fileSelDir->setText(path);
   m_importDir = path;
}

// ѡ��·��
void ImportDocDialog::previewBtn_clicked()
{
    // ���û��ѡ����Ŀ¼
    if(m_importDir.isEmpty()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select the Path to Import Into.."), QMessageBox::Yes);
        return;
    }

   // ȡ��fliter
   int curIndex = fileTypeCmb->currentIndex();
   QDir *dir=new QDir(m_importDir);
   // dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
   // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
   //dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

   Preferences* p = Preferences::instance();

   QStringList filter;
   // all files
   if(curIndex == 0){
       filter << "*.*";
   }
   // all supported files
   if(curIndex == 1){
       filter = p->allsupported();
   }
   // officedoc
   if(curIndex == 2){
       filter = p->officedoc();
   }
   // officedoc
   if(curIndex == 3){
       filter = p->pdf();
   }
   // htmls files
   if(curIndex == 4){
       filter = p->htmls();
   }
   // pics files
   if(curIndex == 5){
       filter = p->pics();
   }
   // swfs files
   if(curIndex == 6){
       filter = p->swfs();
   }
   // sources files
   if(curIndex == 7){
       filter = p->sources();
   }
   // txts files
   if(curIndex == 8){
       filter = p->txts();
   }
   // movies files
   if(curIndex == 9){
       filter = p->movies();
   }

   // �Զ����ļ�����
   QString scustomFileType = customFileType->text();
   if(!scustomFileType.isEmpty()){
       QStringList customList = scustomFileType.split(",");
       filter.append(customList);
   }

   // �Զ����ų��ļ�����
   QString sexcludefileType = excludefileType->text();
   if(!sexcludefileType.isEmpty()){
       QStringList customexList = sexcludefileType.split(",");
       for (int var = 0; var < customexList.size(); ++var) {
            QString temp = customexList.at(var);
            filter.removeAll(temp);
       }
   }

   // model
   model->clear();
   model->setHeaderData(0, Qt::Horizontal, tr("The Files waiting for Imported"));

   // �Ƿ������Ŀ¼
   if(inclueSubDirCheck->isChecked()){
       loadFiles(m_importDir, filter, model);
   }else{
       // ��������Ŀ¼
       dir->setNameFilters(filter);
       QList<QFileInfo> *fileInfos=new QList<QFileInfo>(dir->entryInfoList(filter));
       QFileInfo fileinfo;
       // �г���Ŀ¼�ļ�
       for (int var = 0; var < fileInfos->size(); ++var) {
                fileinfo = fileInfos->at(var);
                if(fileinfo.isFile()){
                    QString filepath = fileinfo.absoluteFilePath();
                    filepath = QDir::toNativeSeparators(filepath);
                    QList<QStandardItem*> items;
                    QStandardItem* item = new QStandardItem(filepath);
                    items << item;
                    model->appendRow(items);
                }
       }
   }

   previewView->setModel(model);
   previewView->resizeColumnToContents(0);
}

//// �����ļ���Ŀ¼ȡ�õݹ�õ���Ŀ¼���ṹ
int ImportDocDialog::loadFiles(QString parentPath, QStringList filter, QStandardItemModel *model){
    //Ŀ¼
    QDir dir(parentPath);
    // ����
    dir.setNameFilters(filter);
    if (!dir.exists()) {
        return -1;
    }

    // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //ת����һ��list
    QFileInfoList list = dir.entryInfoList(filter);
    if(list.size()< 1 ) {
        return -1;
    }

    int i=0;
    // ����Ŀ¼
    do{
        QFileInfo fileInfo = list.at(i);
        QString filepath = fileInfo.filePath();
        //������ļ���
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            loadFiles(filepath, filter, model);
        }else{
            filepath = QDir::toNativeSeparators(filepath);
            QStandardItem* item = new QStandardItem(filepath);
            QList<QStandardItem*> items;
            items << item;
            model->appendRow(items);
        }
        i++;
    } while(i < list.size());
}

//// ɾ���ļ���ť
void ImportDocDialog::delBtn_clicked(){

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

//// ȷ����ť
void ImportDocDialog::confirmBtn_clicked(){
     update = true;
     this->close();
}

//// ȡ����ť
void ImportDocDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}



