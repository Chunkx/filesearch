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
#include "doctagsdialog.h"
#include "utils.h"
#include "fileutils.h"
#include "preferences.h"
#include "db/doctagdao.h"
#include "db/tagdao.h"

DocTagsDialog::DocTagsDialog(QWidget * parent): QDialog(parent), update(false)
{
	setupUi(this);
        // Set UI
        this->setWindowIcon(Utils::getIcon("tool.ico"));
        this->setWindowTitle(tr("Custom the tool bar"));

        // ��ѡ��Toolbar
        Preferences* p = Preferences::instance();
        m_selDocUuId = p->getSelDocUid();

        // ���е�tag
        QList<QString> selUuIds;
        QList<Tag> sellitems = DocTagDao::selectTagsbyDocUuId(m_selDocUuId);
        for (int var = 0; var < sellitems.length(); ++var) {
            Tag tag = sellitems.at(var);
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(Utils::getIcon("tag.ico"));
            item->setText(tag.TAG_NAME);
            item->setData(Qt::UserRole, "tag.ico");
            item->setData(Qt::UserRole + 1, tag.TAG_GUID);
            selitems->addItem(item);
            selUuIds.append(tag.TAG_GUID);
        }

        // δѡ��Toolbar
        QList<Tag> allTags = TagDao::selectAllTags();
        for (int var = 0; var < allTags.length(); ++var) {
            Tag tag = allTags.at(var);
            // ��ȥ��ѡ��
            if(!selUuIds.contains(tag.TAG_GUID)){
                QListWidgetItem *item = new QListWidgetItem;
                item->setIcon(Utils::getIcon("tag.ico"));
                item->setText(tag.TAG_NAME);
                item->setData(Qt::UserRole, "tag.ico");
                item->setData(Qt::UserRole + 1, tag.TAG_GUID);
                items->addItem(item);
            }
        }

        items->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        selitems->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
}

// ȷ��
void DocTagsDialog::confirmBtn_clicked(){
     // ȫ��ɾ��
     DocTagDao::deleteDocTagByDoc(m_selDocUuId);
     // ���Tag
     QList<QListWidgetItem*> tempselitems = selitems->getAllItems();
     for (int var = 0; var < tempselitems.size(); ++var) {
         QListWidgetItem*  item = tempselitems.at(var);
         QString selTagUuid = item->data(Qt::UserRole + 1).toString();
         DocTag doctag;
         doctag.DOCUMENT_GUID = m_selDocUuId;
         doctag.TAG_GUID = selTagUuid;
         DocTagDao::insertDocTag(doctag);
     }

     update = true;
     this->close();
}

// ȡ����ť
void DocTagsDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}
// ����Ĭ�Ϲ�����
bool DocTagsDialog::eventFilter(QObject *obj, QEvent *event)
{
//    if (obj == defaultTool) {
//        if (event->type() == QEvent::MouseButtonPress) {

//            int ret = QMessageBox::question(this, "",
//                                            tr("Are you sure that set the toolbar to default status"),
//                                            QMessageBox::Yes, QMessageBox::No);
//            if(ret == QMessageBox::Yes){
//                Preferences* p = Preferences::instance();
//                QStringList defaultselitems = p->getDefaultToolbarItemList();
//                QStringList defaultWaititems = p->getDefaultWaitToolbarItemList();
//                waitItems.clear();
//                sellItems.clear();
//                sellItems.append(defaultselitems);
//                waitItems.append(defaultWaititems);

//                p->setSelToolbarItemList(sellItems);
//                p->setWaitToolbarItemList(waitItems);
//                update = true;
//                this->close();
//            }
//            if(ret == QMessageBox::No){
//                return false;
//            }
//            return true;
//        } else {
//            return false;
//        }
//    } else {
//        // pass the event on to the parent class
//        return CustomToolDialog::eventFilter(obj, event);
//    }

    return "";
}






