#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QStandardItemModel>

#include "docnotedialog.h"
#include "preferences.h"
#include "utils.h"

DocNoteDialog::DocNoteDialog(QWidget * parent,const QString & dir)
	: QDialog(parent),
	  m_parent(parent),
          m_dir(dir),update(false)
{
	setupUi(this);

        Preferences* p = Preferences::instance();

        // Set UI
        // ����Ŀ��Ŀ¼


        // �б�
        model = new QStandardItemModel();
        // �½�Model
        model->setColumnCount(1);

        this->setWindowIcon(Utils::getIcon("document-import.png"));
        this->setWindowTitle(tr("Document Notes"));

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}


//// ȷ����ť
void DocNoteDialog::confirmBtn_clicked(){
     update = true;
     this->close();
}

//// ȡ����ť
void DocNoteDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}


