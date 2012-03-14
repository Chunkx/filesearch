/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <QtNetwork>
#include <QtWebKit>
#include <QtXmlPatterns>

#include <QTreeWidget>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>
#include <QTime>
#include <QPoint>

#include <QInputDialog>
#include <QFileInfo>
#include <QDirModel>
#include <QTreeWidgetItem>
#include <QList>
#include <QDebug>
#include <QUuid>

#include "preferences.h"
#include "mainwindow.h"
#include "myTreeList.h"
#include "importdocdialog.h"
#include "exportdocdialog.h"
#include "createsubdirdialog.h"
#include "movetodirdialog.h"
#include "propofdirdialog.h"
#include "utils.h"
#include "fileutils.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags), m_view(0)

{
    initUI();
    initActions();
    initMenus();
    initStatusbar();
    initToolbar();

    QWebSettings *defaultSettings = QWebSettings::globalSettings();
    defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
    defaultSettings->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::setOfflineStoragePath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    QWebSettings::setOfflineStorageDefaultQuota(500000);

     m_rssModel = new QStandardItemModel(this);
     setContextMenuPolicy(Qt::CustomContextMenu);
}

// �����ĵ��б�
void MainWindow::buildDocList()
{
    QString curPath = q_myTreeList->getCurPath();
    if(curPath == "alldocs" || curPath == "alltags" ){
        // �����ܸ�Ŀ¼ TODO
        return;
    }

    // �Ƿ���ʾ���ļ����ĵ�
    QStringList files;
    if(isShowDocUnderSub){
        QString datPath = curPath + QDir::separator();
        files = FileUtils::readAllDatFile(datPath, files);
    }else{
        QString datPath = curPath + QDir::separator() + "files.dat";
        files = FileUtils::readFile(datPath);
    }
    m_doctable->buildDocList(files);
}

// ��ʼ�����涯��
void MainWindow::initActions()
{
        // ����
        importAction = new QAction(Utils::getIcon("document-import.png"),tr("&Import..."), this);
        importAction->setShortcut(tr("Ctrl+I"));
        connect(importAction, SIGNAL(triggered()), this, SLOT(importDlg()));

        // ����
        exportAction = new QAction(Utils::getIcon("document-export.png"),tr("&Export..."), this);
        exportAction->setShortcut(tr("Ctrl+E"));
        connect(exportAction, SIGNAL(triggered()), this, SLOT(exportDlg()));

        // ���Ϊ
        saveAsAction = new QAction(Utils::getIcon("document-save-as.png"),tr("&Save As..."), this);
        saveAsAction->setShortcut(tr("Ctrl+S"));
        connect(saveAsAction, SIGNAL(triggered()), this, SLOT(about()));

        // ��浽�ƶ��豸
        saveToMobiAction = new QAction(Utils::getIcon("document-savetomobi.png"),tr("&Save To Mobile..."), this);
        connect(saveToMobiAction, SIGNAL(triggered()), this, SLOT(about()));

        // �˳�
        exitAction = new QAction(Utils::getIcon("close.png"),tr("&Exit"), this);
        exitAction->setShortcut(tr("Ctrl+Q"));
        connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

        // ȫ��
        fullScreenAction = new QAction(Utils::getIcon("view_fullscreen.png"),tr("&Full Screen"), this);
        fullScreenAction->setShortcut(tr("Ctrl+F"));
        connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreen()));

        // ��ʾ/���ط�����
        showClassTreeAction = new QAction(Utils::getIcon("view_tree.png"),tr("&Show/Hide Class Tree"), this);
        showClassTreeAction->setShortcut(tr("Ctrl+T"));
        connect(showClassTreeAction, SIGNAL(triggered()), this, SLOT(showClassTree()));

        // Ƥ��
        skinAction = new QAction(Utils::getIcon("skins.png"),tr("&Skin"), this);
        menu_skin = new QMenu(this);
        skinAction->setMenu(menu_skin);

        //Ƥ�� Ĭ��
        defaultAction = new QAction(tr("&Defaut"), this);
        connect(defaultAction, SIGNAL(triggered()), this, SLOT(about()));

       // Ƥ�� ����
        bookAction = new QAction(tr("&Book"), this);
        connect(bookAction, SIGNAL(triggered()), this, SLOT(about()));

        //Ƥ�� ��ɫ
        grayAction = new QAction(tr("&Gray"), this);
        connect(grayAction, SIGNAL(triggered()), this, SLOT(about()));

        // �Զ��幤����
        customToolbarAction = new QAction(Utils::getIcon("toolbar.png"),tr("&Setting ToolBar"), this);
        connect(customToolbarAction, SIGNAL(triggered()), this, SLOT(about()));

        // ��ʾ/���ع�����
        showToolbarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Tool Bar"), this);
        connect(showToolbarAction, SIGNAL(triggered()), this, SLOT(showToolBar()));

        // ��ʾ/����״̬��
        showSatebarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Status Bar..."), this);
        connect(showSatebarAction, SIGNAL(triggered()), this, SLOT(about()));

        // �û��ֲ�
        userBookAction = new QAction(Utils::getIcon("help_viewer.png"),tr("&Help"), this);
        userBookAction->setShortcut(tr("F2"));
        connect(userBookAction, SIGNAL(triggered()), this, SLOT(helpDoc()));

        // ������ҳ
        homepageAction = new QAction(Utils::getIcon("homepage.png"),tr("&HomePage"), this);
        connect(homepageAction, SIGNAL(triggered()), this, SLOT(homepage()));

        // ��������
        inviteAction = new QAction(Utils::getIcon("invite.png"),tr("&Invite Friends..."), this);
        connect(inviteAction, SIGNAL(triggered()), this, SLOT(about()));

        // ��̳
        bbsAction = new QAction(Utils::getIcon("forum.png"),tr("&Forum"), this);
        connect(bbsAction, SIGNAL(triggered()), this, SLOT(bbs()));

        // ����
        aboutAction = new QAction(Utils::getIcon("about.png"),tr("&About"), this);
        connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

        // ����°汾
        checkNewAction = new QAction(Utils::getIcon("update_product.png"),tr("&Update"), this);
        connect(checkNewAction, SIGNAL(triggered()), this, SLOT(about()));

        // ѡ��
        optionAction = new QAction(Utils::getIcon("option.png"),tr("&Option"), this);
        optionAction->setShortcut(tr("Ctrl+O"));
        connect(optionAction, SIGNAL(triggered()), this, SLOT(about()));

        // �鿴log
        viewLogAction = new QAction(Utils::getIcon("log.png"),tr("&View Log..."), this);
        viewLogAction->setShortcut(tr("Ctrl+L"));
        connect(viewLogAction, SIGNAL(triggered()), this, SLOT(about()));

        // �������
        pluginAction = new QAction(Utils::getIcon("plugin.png"),tr("Plugins"), this);
        pluginAction->setShortcut(tr("Ctrl+P"));
        connect(pluginAction, SIGNAL(triggered()), this, SLOT(about()));;

        // Ƥ��
        languageAction = new QAction(Utils::getIcon("language.png"),tr("&Language"), this);
        menu_language = new QMenu(this);
        languageAction->setMenu(menu_language);

        // Ӣ�İ�
        enAction = new QAction(tr("&English"), this);
        connect(enAction, SIGNAL(triggered()), this, SLOT(about()));

        // ���İ�
        cnAction = new QAction(tr("&Chinese"), this);
        connect(cnAction, SIGNAL(triggered()), this, SLOT(about()));

        // ���İ�
        jpAction = new QAction(tr("&Japanese"), this);
        connect(jpAction, SIGNAL(triggered()), this, SLOT(about()));


        //Doc ContextMenu
        makeSubDir = new QAction(tr("&New Sub Dir"), this);
        connect(makeSubDir, SIGNAL(triggered()), this, SLOT(createSubDir()));
        moveToDir= new QAction(tr("&Move To Dir"), this);
        connect(moveToDir, SIGNAL(triggered()), this, SLOT(moveDir()));
        delDir= new QAction(tr("&Delete"), this);
        connect(delDir, SIGNAL(triggered()), this, SLOT(delSubDir()));
        renameDir= new QAction(tr("&Rename"), this);
        connect(renameDir, SIGNAL(triggered()), this, SLOT(renameSubDir()));
        subDirSort= new QAction(tr("&Sort SubDirs"), this);
        connect(subDirSort, SIGNAL(triggered()), this, SLOT(about()));

        showSubDirDoc= new QAction(tr("&Show docs under subDir"), this);
        showSubDirDoc->setCheckable(true);

        Preferences* p = Preferences::instance();
        showSubDirDoc->setChecked(p->isShowDocUnderSub());

        connect(showSubDirDoc, SIGNAL(triggered()), this, SLOT(setShowSubDirDoc()));

        protectDir= new QAction(tr("&Protect"), this);
        connect(protectDir, SIGNAL(triggered()), this, SLOT(about()));
        propOfDir= new QAction(tr("&Properties"), this);
        connect(propOfDir, SIGNAL(triggered()), this, SLOT(properties()));

        //Root ContextMenu
        makeRootDir= new QAction(tr("&New Root Dir"), this);
        connect(makeRootDir, SIGNAL(triggered()), this, SLOT(createRootDir()));
        dirSort= new QAction(tr("&Sort Dir"), this);
        connect(dirSort, SIGNAL(triggered()), this, SLOT(about()));
        protectRootDir= new QAction(tr("&Protect"), this);
        connect(protectRootDir, SIGNAL(triggered()), this, SLOT(about()));
        optionOfDir= new QAction(tr("&Option"), this);
        connect(optionOfDir, SIGNAL(triggered()), this, SLOT(about()));

        //Tag ContextMenu
        makeSubTag= new QAction(tr("&New Sub Tag"), this);
        connect(makeSubTag, SIGNAL(triggered()), this, SLOT(about()));
        moveToTag= new QAction(tr("&Move to Tag"), this);
        connect(moveToTag, SIGNAL(triggered()), this, SLOT(about()));
        moveToRootTag= new QAction(tr("&Move to Root Tag"), this);
        connect(moveToRootTag, SIGNAL(triggered()), this, SLOT(about()));
        delTag= new QAction(tr("&Delete"), this);
        connect(delTag, SIGNAL(triggered()), this, SLOT(about()));
        renameTag= new QAction(tr("&Rename"), this);
        connect(renameTag, SIGNAL(triggered()), this, SLOT(about()));
        showSubDirTag= new QAction(tr("&Show docs under subDir"), this);
        connect(showSubDirTag, SIGNAL(triggered()), this, SLOT(about()));
        propOfTag= new QAction(tr("&Properties"), this);
        connect(propOfTag, SIGNAL(triggered()), this, SLOT(about()));

        //Root Tag ContextMenu
        makeTag= new QAction(tr("&New Tag"), this);
        connect(makeTag, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::initMenus()
{
        menu_File = menuBar()->addMenu(tr("&File"));
        menu_File->addAction(importAction);
        menu_File->addAction(exportAction);
        menu_File->addSeparator();
        menu_File->addAction(saveAsAction);
        menu_File->addAction(saveToMobiAction);
        menu_File->addSeparator();
        menu_File->addAction(exitAction);

        menu_View = menuBar()->addMenu(tr("&View"));
        menu_View->addAction(fullScreenAction);
        menu_View->addAction(showClassTreeAction);
        menu_View->addAction(showToolbarAction);
        menu_View->addAction(skinAction);

        menu_skin->addAction(defaultAction);
        menu_skin->addAction(bookAction);
        menu_skin->addAction(grayAction);
        menu_View->addAction(menu_skin->menuAction());

        menu_View->addAction(languageAction);
        menu_language->addAction(cnAction);
        menu_language->addAction(enAction);
        menu_language->addAction(jpAction);
        menu_View->addAction(menu_language->menuAction());

        menu_View->addSeparator();
        menu_View->addAction(showSatebarAction);
        menu_View->addSeparator();
        menu_View->addAction(customToolbarAction);

        menu_Tool =  menuBar()->addMenu(tr("&Tool"));
        menu_Tool->addAction(optionAction);
        menu_Tool->addSeparator();
        menu_Tool->addAction(viewLogAction);
        menu_Tool->addSeparator();
        menu_Tool->addAction(pluginAction);

        menu_Help =  menuBar()->addMenu(tr("&Help"));
        menu_Help->addAction(userBookAction);
        menu_Help->addAction(homepageAction);
        menu_Help->addSeparator();
        menu_Help->addAction(inviteAction);
        menu_Help->addSeparator();
        menu_Help->addAction(checkNewAction);
        menu_Help->addSeparator();
        menu_Help->addAction(bbsAction);
        menu_Help->addAction(aboutAction);
        menu_Help->addSeparator();

        // �Ҽ�
        contextMenu = new QMenu(this);
        contextMenu->setStyleSheet(
                    "QMenu::item{height: 20px;}"
                    "QMenu::item:hover{background-color:rgb(100,100,100)}"
                    "QMenu::item:selected{background-color:rgb(128,128,128)}"
        );
        contextMenu->hide();
}
// ״̬��
void MainWindow::initStatusbar()
{
//    QLabel* m_sqliteVersionLabel = new QLabel(this);
//    statusBar()->addPermanentWidget(&m_sqliteVersionLabel);
}

// ������
void MainWindow::initToolbar()
{
    toolBar = addToolBar(tr("Tool Bar"));
    toolBar->addAction(showClassTreeAction);
    toolBar->addAction(fullScreenAction);
    toolBar->addSeparator();


    label = new QLabel(tr("Find &what:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    lineEdit->setMaximumWidth(120);
    findButton = new QPushButton(tr("&Search"));
    findButton->setDefault(true);
    findButton->setEnabled(false);

    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableFindButton(const QString&)));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));

    toolBar->addWidget(label);
    toolBar->addWidget(lineEdit);
    toolBar->addWidget(findButton);
}

// �û��ֲ�
void MainWindow::helpDoc()
{
   m_view->load(QUrl("http://www.slfile.net/?page_id=58"));
}

// ��̳
void MainWindow::bbs()
{
   m_view->load(QUrl("http://www.slfile.net/?post_type=forum"));
}

// ��ҳ
void MainWindow::homepage()
{
   m_view->load(QUrl("http://www.slfile.net/"));
}

// �����Ƿ���ʾ���ļ����µ��ļ�
void MainWindow::setShowSubDirDoc()
{
    // ����ѡ��
    isShowDocUnderSub = showSubDirDoc->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSub(isShowDocUnderSub);
}

void MainWindow::enableFindButton(const QString &text)
{
     findButton->setEnabled(!text.isEmpty());
}

void MainWindow::findClicked()
{
     QString text = lineEdit->text();
     //emit findPrevious(text, cs);
}

// �򿪵������
void MainWindow::importDlg()
{
    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alldocs" && curPath != "alltags") {
        hasSelRight = true;
        ImportDocDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            int row= dlg.model->rowCount();
            // ����·���б�
            FileUtils::copyDirectory(QDir(dlg.m_importDir),QDir(curPath));


            QMap<QString, QStringList> writeMap;

            for (int var = 0; var < row; ++var) {
                QStandardItem* temp = dlg.model->item(var);
                QString path = temp->text();

                QString filepath = path.left(path.lastIndexOf(QDir::separator()));
                filepath = filepath.replace("\"","");

                // ����Ŀ��Ŀ¼
                QString destDir = filepath.remove(0, dlg.m_importDir.length());
                QString tempPath = curPath;
                QString datfilepath = tempPath.append(destDir).append(QDir::separator()).append("files.dat");

                QUuid uuid = QUuid::createUuid();
                if(writeMap.contains(datfilepath)){
                    QStringList files = writeMap[datfilepath];
                    files << uuid.toString().append(",").append(path);
                    writeMap.insert(datfilepath, files);
                } else {
                    QStringList files;
                    files << uuid.toString().append(",").append(path);
                    writeMap.insert(datfilepath, files);
                }

            }

            // һ��д���ļ�
            QMapIterator<QString, QStringList> rowMapIterator(writeMap);
            rowMapIterator.toBack();
            while (rowMapIterator.hasPrevious())
            {
                rowMapIterator.previous();
                QString datfilepath = rowMapIterator.key();
                QStringList files = rowMapIterator.value();
                FileUtils::writeFile(datfilepath, files);
            }

            // �����Ŀ¼<�������ļ���Ҳ��files.dat�ļ�>
            FileUtils::delDirectory(QDir(curPath));
            // ���¼������ڵ�
            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->delSubItems(curItem);
            QString curPath = q_myTreeList->getCurPath();
            q_myTreeList->loadDirByLay(curPath, 1, curItem);
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub directory."), QMessageBox::Yes);
        return;
    }
}

// �򿪵�������
void MainWindow::exportDlg()
{
    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alldocs" && curPath != "alltags") {
        hasSelRight = true;
        ExportDocDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub directory."), QMessageBox::Yes);
        return;
    }
}

// �鿴�ļ�������
void MainWindow::properties()
{
    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alltags") {
        hasSelRight = true;
        PropOfDirDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}



void MainWindow::initUI()
{
        setWindowTitle(m_appName);

        splitter = new QSplitter(this);

        m_view = new QWebView(this);
        m_view->move(-1000, -1000);
        m_view->page()->mainFrame()->load(QUrl("http://www.slfile.net"));


        // ���ط�����
        m_baseDir = Utils::getLocatePath();
        m_baseDir = QDir::toNativeSeparators(m_baseDir);
        q_myTreeList = new myTreeList("", this);

        q_myTreeList->loadDirByLay(m_baseDir, 0, 0);
        q_myTreeList->enableMouse(true);

        m_doctable = new MyTableView(this);



        splitter->addWidget(q_myTreeList);
        splitter->addWidget(m_doctable);
        splitter->addWidget(m_view);

        // connect(q_myTreeList, SIGNAL(LBtnDbClk()), this, SLOT(showChildTree()));
        connect(q_myTreeList, SIGNAL(RBtnClk()), this, SLOT(treeContextMenuOpened()));
        connect(q_myTreeList, SIGNAL(LBtnClk()), this, SLOT(buildDocList()));

        setCentralWidget(splitter);
}


////// �����ļ���Ŀ¼ȡ����Ŀ¼���ṹ
//int MainWindow::loadDirByLay(QString parentPath, int lay, QStandardItem *curItem){
//    //Ŀ¼
//    QDir dir(parentPath);
//    if (!dir.exists()) {
//        return -1;
//    }
//    // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
//    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

//    //�ļ�������
//    dir.setSorting(QDir::DirsFirst);

//    //ת����һ��list
//    QFileInfoList list = dir.entryInfoList();
//    if(list.size()< 1 ) {
//        return -1;
//    }

//    int i=0;
//    // ����Ŀ¼
//    do{
//        QFileInfo fileInfo = list.at(i);
//        QString filename = fileInfo.fileName();
//        QString filepath = fileInfo.filePath();
//        filepath = QDir::toNativeSeparators(filepath);
//        //������ļ���
//        bool bisDir = fileInfo.isDir();
//        if(bisDir) {
//            if(lay == 0){
//              q_myTreeList->addItem(lay, filename, filepath, "expander_normal.png");
//            }else{
//              q_myTreeList->addItemByParentItem(curItem, filename, filepath, "expander_normal.png");
//            }
//        }
//        i++;
//    } while(i < list.size());
//}


MainWindow::~MainWindow()
{
}

// �������ļ���
void MainWindow::createSubDir()
{
    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alltags") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            // ˢ��ѡ�е���
            QString tempPath = curPath;
            tempPath.append(QDir::separator()).append(dlg.dirName->text());

            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->addItemByParentItem(curItem, dlg.dirName->text(), tempPath, "expander_normal.png");
            curItem->setIcon(Utils::getIcon("expander_open.png"));
            q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// �ƶ��ļ���
void MainWindow::moveDir()
{
    QString curPath = q_myTreeList->getCurPath();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    bool hasSelRight = false;

    // ��ѡ�� �ļ� �ӽڵ�
    if(!curPath.isEmpty() && curPath != "alltags" && curPath != "alldocs") {
        hasSelRight = true;
        MoveToDirDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
              // ȡ���ӽ���ѡ�е�path
              QString mselDir = dlg.m_curPath;
              // ɾ��������ѡ�еĽڵ�
              curItem->parent()->removeRow(curItem->row());


              // ����������Ľڵ� (�ӽ����½��ļ�������²��ɹ�)
              q_myTreeList->setCurItemByPath(mselDir);

              QString curPath = q_myTreeList->getCurPath();
              if(!curPath.isEmpty()){
                  QStandardItem* curItem = q_myTreeList->getCurItem();
                  QString temp = dlg.m_seldir;
                  temp = temp.right(temp.length() - temp.lastIndexOf(QDir::separator()) - 1);
                  q_myTreeList->addItemByParentItem(curItem, temp, dlg.m_seldir, "expander_normal.png");
                  curItem->setIcon(Utils::getIcon("expander_open.png"));
                  q_myTreeList->expand(q_myTreeList->getCurIndex());
              }
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}


// �ı����ļ�������
void MainWindow::renameSubDir()
{
    QString curPath = q_myTreeList->getCurPath();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    QString curTitle = q_myTreeList->getCurTitle();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curPath.isEmpty() && curPath != "alltags") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Directory name:"), QLineEdit::Normal, curTitle, &ok);
        if (ok && !text.isEmpty()) {
                if (text == curTitle){
                  return;
                }
                // �ı����ڵ�
                curItem->setData(text, Qt::DisplayRole);
                // �ı�Ŀ¼����
                QString newPath = curPath;
                QDir dir(curPath);
                newPath = newPath.left(curPath.lastIndexOf(QDir::separator())).append(QDir::separator()).append(text);
                dir.rename(curPath, newPath);
        }

    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// �½����ڵ�
void MainWindow::createRootDir()
{
    QString curPath = q_myTreeList->getCurPath();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ�
    if(curPath == "alldocs") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Roor Directory name:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
                if (text == ""){
                  return;
                }
                // ˢ��ѡ�е���
                QString tempPath = m_baseDir;
                tempPath.append(QDir::separator()).append(text);

                // �½��ļ���
                QDir *mdir = new QDir(m_baseDir);
                if(mdir->exists()){
                    if(!mdir->mkdir(text)){
                        QMessageBox::warning(this, tr("Error"), tr("Create New Root Directory failed. "), QMessageBox::Yes);
                        return;
                    }
                }

                QStandardItem* curItem = q_myTreeList->getCurItem();
                q_myTreeList->addItemByParentItem(curItem, text, tempPath, "expander_normal.png");
                curItem->setIcon(Utils::getIcon("expander_open.png"));
                q_myTreeList->expand(q_myTreeList->getCurIndex());

        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}




void MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(windowTitle()), 
                       tr("Local file Manage Version"));
}

// ɾ�����ļ���
void MainWindow::delSubDir()
{
    q_myTreeList->delSubTree();
}

// ȫ��
void MainWindow::fullScreen()
{
    if(m_doctable->isHidden()){
       m_doctable->show();
    }else{
       m_doctable->hide();
    }

    if(q_myTreeList->isHidden()){
       q_myTreeList->show();
    }else{
       q_myTreeList->hide();
    }
}

// ��ʾ/���ط�����
void MainWindow::showClassTree()
{
    if(q_myTreeList->isHidden()){
       q_myTreeList->show();
    }else{
       q_myTreeList->hide();
    }
}
// ��ʾ/���ع�����
void MainWindow::showToolBar()
{

    if(toolBar->isHidden()){
       toolBar->show();
    }else{
       toolBar->hide();
    }
}
// �����ڵ㱻ѡ��
void MainWindow::treeItemActivated()
{
        QString curPath = q_myTreeList->getCurPath();
        // ���
        if(curPath == "alldocs" ||curPath == "alltags"){
          // ������ʾ�����ĵ�

        } else{

        }
}

// ���Ҽ��˵�
void MainWindow::treeContextMenuOpened()
{
        tableTree_currentItemChanged();
        QPoint pos = q_myTreeList->getCurPoint();
        if (contextMenu->actions().count() > 1){
           contextMenu->exec(q_myTreeList->viewport()->mapToGlobal(pos));
        }
}
// ���Ҽ��˵�
void MainWindow::tableTree_currentItemChanged()
{
        contextMenu->clear();

        QString tcurPath = q_myTreeList->getCurPath();
        // ���ѡ��Root
        if(tcurPath == "alldocs"){
            contextMenu->addAction(makeRootDir);
            contextMenu->addSeparator();
            contextMenu->addAction(dirSort);
            contextMenu->addSeparator();
            contextMenu->addAction(protectRootDir);
            contextMenu->addSeparator();
            contextMenu->addAction(optionOfDir);
        } else if(tcurPath == "alltags"){
            //Tag ContextMenu
            contextMenu->addAction(makeTag);
            contextMenu->addSeparator();
            contextMenu->addAction(showSubDirTag);
            contextMenu->addAction(propOfTag);
            contextMenu->addSeparator();
        }  else if(tcurPath == "tag"){
            contextMenu->addAction(makeSubTag);
            contextMenu->addSeparator();
            contextMenu->addAction(moveToTag);
            contextMenu->addAction(moveToRootTag);
            contextMenu->addAction(delTag);
            contextMenu->addAction(renameTag);
            contextMenu->addSeparator();
            contextMenu->addAction(showSubDirTag);
            contextMenu->addSeparator();
            contextMenu->addAction(propOfTag);
        } else {
            contextMenu->addAction(makeSubDir);
            contextMenu->addSeparator();
            contextMenu->addAction(moveToDir);
            contextMenu->addAction(delDir);
            contextMenu->addAction(renameDir);
            contextMenu->addSeparator();
            contextMenu->addAction(subDirSort);
            contextMenu->addSeparator();
            contextMenu->addAction(showSubDirDoc);
            contextMenu->addAction(subDirSort);
            contextMenu->addAction(protectDir);
            contextMenu->addAction(subDirSort);
            contextMenu->addAction(propOfDir);
        }
        return;
}


void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::clearHistory()
{
    //ui.webView->page()->mainFrame()->evaluateJavaScript("clearHistory()");
    //statusBar()->showMessage(tr("History cleared"));
}
