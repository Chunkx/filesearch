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
#include "customtooldialog.h"
#include "utils.h"
#include "fileutils.h"
#include "preferences.h"

CustomToolDialog::CustomToolDialog(QWidget * parent): QDialog(parent), update(false)
{
	setupUi(this);
        // Set UI
        this->setWindowIcon(Utils::getIcon("tool.ico"));
        this->setWindowTitle(tr("Custom the tool bar"));

        // ��ѡ��Toolbar
        Preferences* p = Preferences::instance();
        QStringList sellitems = p->getSelToolbarItemList();

        for (int var = 0; var < sellitems.length(); ++var) {
            QString text = sellitems.at(var);
            QListWidgetItem* temp = getToolbarItemByText(text);
            selitems->addItem(temp);
        }

        // δѡ��Toolbar
        QStringList waititems = p->getWaitToolbarItemList();
        for (int var = 0; var < waititems.length(); ++var) {
            QString text = waititems.at(var);
            QListWidgetItem* temp = getToolbarItemByText(text);
            items->addItem(temp);
        }

        defaultTool->setPixmap(Utils::getPixmap("defaulttoolbar.png"));

        items->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        selitems->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        defaultTool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(cancelBtn_clicked()));
        defaultTool->installEventFilter(this);
}

// ȷ��
void CustomToolDialog::confirmBtn_clicked(){
     update = true;
     // д��preferences
     Preferences* p = Preferences::instance();
     waitItems.clear();
     sellItems.clear();

     QList<QListWidgetItem*> tempselitems = selitems->getAllItems();
     for (int var = 0; var < tempselitems.size(); ++var) {
         QListWidgetItem*  item = tempselitems.at(var);
         QString temp = item->data(Qt::UserRole).toString();
         sellItems.append(temp);
     }
     // δѡ��Toolbar
     QList<QListWidgetItem*> tempitems = items->getAllItems();
     for (int var = 0; var < tempitems.size() ;++var) {
         QListWidgetItem* item = tempitems.at(var);
         QString temp = item->data(Qt::UserRole).toString();
         waitItems.append(temp);
     }

     p->setSelToolbarItemList(sellItems);
     p->setWaitToolbarItemList(waitItems);

     update = true;
     this->close();
}

// ȡ����ť
void CustomToolDialog::cancelBtn_clicked(){
     update = false;
     this->close();
}
// ����Ĭ�Ϲ�����
bool CustomToolDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == defaultTool) {
        if (event->type() == QEvent::MouseButtonPress) {

            int ret = QMessageBox::question(this, "",
                                            tr("Are you sure that set the toolbar to default status"),
                                            QMessageBox::Yes, QMessageBox::No);
            if(ret == QMessageBox::Yes){
                Preferences* p = Preferences::instance();
                QStringList defaultselitems = p->getDefaultToolbarItemList();
                QStringList defaultWaititems = p->getDefaultWaitToolbarItemList();

                p->setSelToolbarItemList(defaultselitems);
                p->setWaitToolbarItemList(defaultWaititems);
                update = true;
                this->close();
            }
            if(ret == QMessageBox::No){
                return false;
            }
            return true;
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return CustomToolDialog::eventFilter(obj, event);
    }
}

// ����textȡ��ListItem
QListWidgetItem* CustomToolDialog::getToolbarItemByText(const QString & text)
{
        // �ж�
        if(text == "view_tree.png"){
            // ������
            QListWidgetItem *viewTreeItem = new QListWidgetItem;
            viewTreeItem->setIcon(Utils::getIcon("view_tree.png"));
            viewTreeItem->setText(tr("Show/Hide Class Tree"));
            viewTreeItem->setData(Qt::UserRole, "view_tree.png");

            return viewTreeItem;
        }
        // ȫ��
        if(text == "view_fullscreen.png"){
            QListWidgetItem *fullScreen = new QListWidgetItem;
            fullScreen->setIcon(Utils::getIcon("view_fullscreen.png"));
            fullScreen->setText(tr("Full Screen"));
            fullScreen->setData(Qt::UserRole, "view_fullscreen.png");
            return fullScreen;
        }

        // ������ҳ
        if(text == "homepage.png"){
            QListWidgetItem *homepageItem = new QListWidgetItem;
            homepageItem->setIcon(Utils::getIcon("homepage.png"));
            homepageItem->setText(tr("HomePage"));
            homepageItem->setData(Qt::UserRole, "homepage.png");
            return homepageItem;
        }

        // ��������
        if(text == "invite.png"){
            QListWidgetItem *inviteItem = new QListWidgetItem;
            inviteItem->setIcon(Utils::getIcon("invite.png"));
            inviteItem->setText(tr("Invite Friends..."));
            inviteItem->setData(Qt::UserRole, "invite.png");
            return inviteItem;
        }

       // ��̳
       if(text == "forum.png"){

           QListWidgetItem *forumItem = new QListWidgetItem;
           forumItem->setIcon(Utils::getIcon("forum.png"));
           forumItem->setText(tr("Forum"));
           forumItem->setData(Qt::UserRole, "forum.png");
           return forumItem;
       }

       // ��浽�ƶ��豸
      if(text == "document-savetomobi.png"){
          QListWidgetItem *saveToMobiItem = new QListWidgetItem;
          saveToMobiItem->setIcon(Utils::getIcon("document-savetomobi.png"));
          saveToMobiItem->setText(tr("Save To Mobile..."));
          saveToMobiItem->setData(Qt::UserRole, "document-savetomobi.png");
          return saveToMobiItem;
      }

      //  ����
     if(text == "document-import.png"){
         QListWidgetItem *importItem = new QListWidgetItem;
         importItem->setIcon(Utils::getIcon("document-import.png"));
         importItem->setText(tr("Import..."));
         importItem->setData(Qt::UserRole, "document-import.png");
         return importItem;
     }

     //  ����
    if(text == "document-export.png"){
        QListWidgetItem *exportItem = new QListWidgetItem;
        exportItem->setIcon(Utils::getIcon("document-export.png"));
        exportItem->setText(tr("Export..."));
        exportItem->setData(Qt::UserRole, "document-export.png");
        return exportItem;
    }

   // �������
   if(text == "plugin.png"){
       QListWidgetItem *pluginItem = new QListWidgetItem;
       pluginItem->setIcon(Utils::getIcon("plugin.png"));
       pluginItem->setText(tr("Plugins"));
       pluginItem->setData(Qt::UserRole, "plugin.png");
       return pluginItem;
   }

}






