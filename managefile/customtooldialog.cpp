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

CustomToolDialog::CustomToolDialog(QWidget * parent): QDialog(parent), update(false)
{
	setupUi(this);
        // Set UI
        this->setWindowIcon(Utils::getIcon("tool.ico"));
        this->setWindowTitle(tr("Custom the tool bar"));

        // ������
        QListWidgetItem *viewTreeItem = new QListWidgetItem;
        viewTreeItem->setIcon(Utils::getIcon("view_tree.png"));
        viewTreeItem->setText(tr("Show/Hide Class Tree"));
        // 0 ��ʾδ�϶�
        viewTreeItem->setData(Qt::UserRole, "0");

        // ȫ��
        QListWidgetItem *fullScreen = new QListWidgetItem;
        fullScreen->setIcon(Utils::getIcon("view_fullscreen.png"));
        fullScreen->setText(tr("Full Screen"));
        // 0 ��ʾδ�϶�
        fullScreen->setData(Qt::UserRole, "0");

        // ������ҳ
        QListWidgetItem *homepageItem = new QListWidgetItem;
        homepageItem->setIcon(Utils::getIcon("homepage.png"));
        homepageItem->setText(tr("HomePage"));
        // 0 ��ʾδ�϶�
        homepageItem->setData(Qt::UserRole, "0");

        // ��������
        QListWidgetItem *inviteItem = new QListWidgetItem;
        inviteItem->setIcon(Utils::getIcon("invite.png"));
        inviteItem->setText(tr("Invite Friends..."));
        // 0 ��ʾδ�϶�
        inviteItem->setData(Qt::UserRole, "0");

        // ��̳
        QListWidgetItem *forumItem = new QListWidgetItem;
        forumItem->setIcon(Utils::getIcon("forum.png"));
        forumItem->setText(tr("Forum"));
        // 0 ��ʾδ�϶�
        forumItem->setData(Qt::UserRole, "0");

        defaultTool->setPixmap(Utils::getPixmap("defaulttoolbar.png"));

        items->addItem(viewTreeItem);
        items->addItem(fullScreen);
        items->addItem(homepageItem);
        items->addItem(inviteItem);
        items->addItem(forumItem);


        selitems->addItem(viewTreeItem);
        selitems->addItem(fullScreen);
        selitems->addItem(homepageItem);
        selitems->addItem(inviteItem);
        selitems->addItem(inviteItem);


//        // ������
//        ToolButton *viewTreeItem = new ToolButton(this);
//        viewTreeItem->setIcon(Utils::getIcon("view_tree.png"));
//        viewTreeItem->setText(tr("Show/Hide Class Tree"));
//        items->addItem(viewTreeItem);



//        // ȫ��
//        ToolButton *fullScreen = new ToolButton(this);
//        fullScreen->setIcon(Utils::getIcon("view_fullscreen.png"));
//        fullScreen->setText(tr("Full Screen"));
//  items->addItem(fullScreen);

//        // ������ҳ
//        ToolButton *homepageItem = new ToolButton(this);
//        homepageItem->setIcon(Utils::getIcon("homepage.png"));
//        homepageItem->setText(tr("HomePage"));
//  items->addItem(homepageItem);

//        // ��������
//        ToolButton *inviteItem = new ToolButton(this);
//        inviteItem->setIcon(Utils::getIcon("invite.png"));
//        inviteItem->setText(tr("Invite Friends..."));

//  items->addItem(inviteItem);
//        // ��̳
//        ToolButton *forumItem = new ToolButton(this);
//        forumItem->setIcon(Utils::getIcon("forum.png"));
//        forumItem->setText(tr("Forum"));
//  items->addItem(forumItem);



//        items->setFileName(tr("&Show/Hide Class Tree"));
//        items->setObjectName(tr("&Show/Hide Class Tree"));
        items->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        selitems->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        defaultTool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(buttonBox, SIGNAL(accepted()), this, SLOT(confirmBtn_clicked()));
}

// ȷ��
void CustomToolDialog::confirmBtn_clicked(){
     update = true;
     this->close();
}


