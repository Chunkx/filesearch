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
#include "createtagdialog.h"
#include "browser/browsermainwindow.h"
#include "browser/browserapplication.h"
#include "utils.h"
#include "fileutils.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
    , m_tabWidget(new TabWidget(this))
    , m_autoSaver(new AutoSaver(this))
    , m_historyBack(0)
    , m_historyForward(0)
    , m_stop(0)
    , m_reload(0)
{
    initUI();
    initActions();
    initMenus();
    initStatusbar();
    initToolbar();
    initBrowser();


    m_appName = tr("Local File Manage");
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

// ������������Menu
void MainWindow::slotShowSearchSetMenu()
{
    if(m_searchSetMenu->children().length() > 2){
      return;
    }

    // ����Menu
    QAction *action1 = new QAction(this);
    action1->setData(ALLDOC);
    action1->setCheckable(true);
    action1->setText(tr("ALL Documents"));

    // ����Menu
    QAction *action2 = new QAction(this);
    action2->setData(CURRENTDIR);
    action2->setCheckable(true);
    action2->setText(tr("Current Directory"));

    // ����Menu
    QAction *action3 = new QAction(this);
    action3->setData(CURRENTDIRINCLUESUB);
    action3->setCheckable(true);
    action3->setText(tr("Current Directory(Include Sub Directory)"));

    // ����Menu
    QAction *action4 = new QAction(this);
    action4->setData(ALLTEXT);
    action4->setCheckable(true);
    action4->setText(tr("ALL Text"));

    QAction *action5 = new QAction(this);
    action5->setData(DOCNAMEONLY);
    action5->setCheckable(true);
    action5->setText(tr("Document Name Only"));

    QAction *action6 = new QAction(this);
    action6->setData(WEBSEARCH);
    action6->setCheckable(true);
    action6->setText(tr("Web Search"));

    QAction *action7 = new QAction(this);
    action7->setData(OPTION);
    action7->setCheckable(true);
    action7->setText(tr("Option"));

    QAction *action8 = new QAction(this);
    action8->setData(SAVETOFASTSEARCH);
    action8->setCheckable(true);
    action8->setText(tr("Save To Fast Search"));

    m_searchSetMenu->addAction(action1);
    m_searchSetMenu->addAction(action2);
    m_searchSetMenu->addAction(action3);
    m_searchSetMenu->addSeparator();
    m_searchSetMenu->addAction(action4);
    m_searchSetMenu->addAction(action5);
    m_searchSetMenu->addSeparator();
    m_searchSetMenu->addAction(action6);
    m_searchSetMenu->addSeparator();
    m_searchSetMenu->addAction(action7);
    m_searchSetMenu->addAction(action8);
}

// ��ʼ�����涯��
void MainWindow::initActions()
{

        // �����������
        m_searchSetAction = new QAction(tr("Search"), this);
        m_searchSetAction->setIconVisibleInMenu(false);
        m_searchSetMenu = new QMenu(this);
        m_searchSetAction->setMenu(m_searchSetMenu);
        connect(m_searchSetMenu, SIGNAL(aboutToShow()), this, SLOT(slotShowSearchSetMenu()));
        connect(m_searchSetMenu, SIGNAL(triggered(QAction*)),this, SLOT(slotOpenActionUrl(QAction*)));


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
        showClassTreeAction->setShortcut(tr("Ctrl+H"));
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
        connect(makeTag, SIGNAL(triggered()), this, SLOT(newTag()));
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

// ��ʼ�������
void MainWindow::initBrowser()
{

    setupToolBar();
    QWidget *centralWidget = new QWidget(this);

    BookmarksModel *boomarksModel = BrowserApplication::bookmarksManager()->bookmarksModel();
    m_bookmarksToolbar = new BookmarksToolBar(boomarksModel, centralWidget);

    //connect(m_bookmarksToolbar, SIGNAL(openUrl(QUrl)), m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));

    //connect(m_bookmarksToolbar->toggleViewAction(), SIGNAL(toggled(bool)), this, SLOT(updateBookmarksToolbarActionText(bool)));

//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->setSpacing(0);
//    layout->setMargin(0);
//#if defined(Q_WS_MAC)
//    layout->addWidget(m_bookmarksToolbar);
//    layout->addWidget(new QWidget); // <- OS X tab widget style bug
//#else
    //addToolBarBreak();
    //addToolBar(m_bookmarksToolbar);
//#endif
//    layout->addWidget(m_tabWidget);
//    centralWidget->setLayout(layout);

    // ����spplitter
    splitter->addWidget(m_tabWidget);

    connect(m_tabWidget, SIGNAL(loadPage(QString)),
        this, SLOT(loadPage(QString)));
    connect(m_tabWidget, SIGNAL(setCurrentTitle(QString)),
        this, SLOT(slotUpdateWindowTitle(QString)));
    connect(m_tabWidget, SIGNAL(showStatusBarMessage(QString)),
              statusBar(), SLOT(showMessage(QString)));
    connect(m_tabWidget, SIGNAL(linkHovered(QString)),
                statusBar(), SLOT(showMessage(QString)));
    connect(m_tabWidget, SIGNAL(loadProgress(int)),
            this, SLOT(slotLoadProgress(int)));
    connect(m_tabWidget, SIGNAL(tabsChanged()),
            m_autoSaver, SLOT(changeOccurred()));
    connect(m_tabWidget, SIGNAL(geometryChangeRequested(QRect)),
            this, SLOT(geometryChangeRequested(QRect)));
    connect(m_tabWidget, SIGNAL(printRequested(QWebFrame*)),
            this, SLOT(printRequested(QWebFrame*)));
    connect(m_tabWidget, SIGNAL(menuBarVisibilityChangeRequested(bool)),
            menuBar(), SLOT(setVisible(bool)));
   connect(m_tabWidget, SIGNAL(statusBarVisibilityChangeRequested(bool)),
           statusBar(), SLOT(setVisible(bool)));
    connect(m_tabWidget, SIGNAL(toolBarVisibilityChangeRequested(bool)),
            m_navigationBar, SLOT(setVisible(bool)));
    connect(m_tabWidget, SIGNAL(toolBarVisibilityChangeRequested(bool)),
            m_bookmarksToolbar, SLOT(setVisible(bool)));
#if defined(Q_WS_MAC)
    connect(m_tabWidget, SIGNAL(lastTabClosed()),
            this, SLOT(close()));
#else
    connect(m_tabWidget, SIGNAL(lastTabClosed()),
            m_tabWidget, SLOT(newTab()));
#endif

//    // �ı�m_tabwiget��Сʱ
//    connect(m_doctable, SIGNAL(resize(QResizeEvent *event)), this, SLOT(resizeSpace()));

    slotUpdateWindowTitle();
    loadDefaultState();
    m_tabWidget->newTab();

    int size = m_tabWidget->lineEditStack()->sizeHint().height();
    m_navigationBar->setIconSize(QSize(size, size));
}


//// ���ڿհ������С
//void MainWindow::resizeSpace()
//{
//     sapcewidget->setMinimumWidth(m_doctable->width()*1.2);
//}

// ״̬��
void MainWindow::initStatusbar()
{
  // QLabel* m_sqliteVersionLabel = new QLabel(this);
  //statusBar()->addPermanentWidget(&m_sqliteVersionLabel);
}

// ������
void MainWindow::initToolbar()
{
    toolBar = addToolBar(tr("Tool Bar"));
    toolBar->addAction(showClassTreeAction);
    toolBar->addAction(fullScreenAction);
    toolBar->addSeparator();
    toolBar->addAction(homepageAction);
    toolBar->addAction(inviteAction);
    toolBar->addAction(bbsAction);
    toolBar->addSeparator();

    // �հ�����
    QWidget *twidget = new QWidget(this);
    twidget->setMinimumWidth(this->geometry().width() * 0.1);
    toolBar->addWidget(twidget);

    m_toolbarSearch = new ToolbarSearch();
    connect(m_toolbarSearch, SIGNAL(search(QUrl)), SLOT(loadUrl(QUrl)));

    toolBar->addWidget(m_toolbarSearch);
    toolBar->addAction(m_searchSetAction);

    // �հ�����
    sapcewidget = new QWidget(this);
    sapcewidget->setMinimumWidth(m_doctable->width()*1.2);
    toolBar->addWidget(sapcewidget);
}

// �û��ֲ�
void MainWindow::helpDoc()
{
     m_tabWidget->newTab();
     loadUrl(QUrl("http://www.slfile.net/?page_id=58"));
}

// ��̳
void MainWindow::bbs()
{
    m_tabWidget->newTab();
    loadUrl(QUrl("http://www.slfile.net/?post_type=forum"));

}

// ��ҳ
void MainWindow::homepage()
{
    m_tabWidget->newTab();
    loadUrl(QUrl("http://www.slfile.net/"));
}

// �����Ƿ���ʾ���ļ����µ��ļ�
void MainWindow::setShowSubDirDoc()
{
    // ����ѡ��
    isShowDocUnderSub = showSubDirDoc->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSub(isShowDocUnderSub);
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
        // ���ط�����
        m_baseDir = Utils::getLocatePath();
        m_baseDir = QDir::toNativeSeparators(m_baseDir);
        q_myTreeList = new myTreeList("", this);

        q_myTreeList->loadDirByLay(m_baseDir, 0, 0);
        q_myTreeList->enableMouse(true);

        m_doctable = new MyTableView(this);

        splitter->addWidget(q_myTreeList);
        splitter->addWidget(m_doctable);

        // connect(q_myTreeList, SIGNAL(LBtnDbClk()), this, SLOT(showChildTree()));
        connect(q_myTreeList, SIGNAL(RBtnClk()), this, SLOT(treeContextMenuOpened()));
        connect(q_myTreeList, SIGNAL(LBtnClk()), this, SLOT(buildDocList()));

        setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    m_autoSaver->changeOccurred();
    m_autoSaver->saveIfNeccessary();
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

// �½�����ǩ
void MainWindow::newTag()
{
    QString curPath = q_myTreeList->getCurPath();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    bool hasSelRight = false;

    // ��ѡ�� ��ǩ �ڵ�
    if(curPath == "alltags" ||
            (curPath.indexOf(QDir::separator()) == -1 && curPath != "alldocs")) {
        hasSelRight = true;
        // curPath �����жϸ��ڵ���ӽڵ�
        // getCurPath ���ڴ�UuId
        CreateTagDialog dlg(this, curPath);
        dlg.exec();
        if(dlg.update){
              // ȡ���ӽ�������Tag��UuId
              QString mselDir = dlg.m_newTagUuId;
              // ɾ��������ѡ�еĽڵ�
              curItem->parent()->removeRow(curItem->row());

              // ����������Ľڵ� (�ӽ����½��ļ�������²��ɹ�)
              q_myTreeList->setCurItemByPath(mselDir);

              QString curPath = q_myTreeList->getCurPath();
              if(!curPath.isEmpty()){
                  QStandardItem* curItem = q_myTreeList->getCurItem();
                  q_myTreeList->addItemByParentItem(curItem,  dlg.m_tagname, dlg.m_newTagUuId, "expander_normal.png");
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



void MainWindow::loadDefaultState()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("BrowserMainWindow"));
    QByteArray data = settings.value(QLatin1String("defaultState")).toByteArray();
    restoreState(data);
    settings.endGroup();
}

QSize MainWindow::sizeHint() const
{
    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

void MainWindow::save()
{
    BrowserApplication::instance()->saveSession();

    QSettings settings;
    settings.beginGroup(QLatin1String("BrowserMainWindow"));
    QByteArray data = saveState(false);
    settings.setValue(QLatin1String("defaultState"), data);
    settings.endGroup();
}

static const qint32 BrowserMainWindowMagic = 0xba;

QByteArray MainWindow::saveState(bool withTabs) const
{
    int version = 2;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << qint32(BrowserMainWindowMagic);
    stream << qint32(version);

    stream << size();
    stream << !m_navigationBar->isHidden();
    stream << !m_bookmarksToolbar->isHidden();
    //stream << !statusBar()->isHidden();
    if (withTabs)
        stream << tabWidget()->saveState();
    else
        stream << QByteArray();
    return data;
}

bool MainWindow::restoreState(const QByteArray &state)
{
    int version = 2;
    QByteArray sd = state;
    QDataStream stream(&sd, QIODevice::ReadOnly);
    if (stream.atEnd())
        return false;

    qint32 marker;
    qint32 v;
    stream >> marker;
    stream >> v;
    if (marker != BrowserMainWindowMagic || v != version)
        return false;

    QSize size;
    bool showToolbar;
    bool showBookmarksBar;
    bool showStatusbar;
    QByteArray tabState;

    stream >> size;
    stream >> showToolbar;
    stream >> showBookmarksBar;
    stream >> showStatusbar;
    stream >> tabState;

    resize(size);

    m_navigationBar->setVisible(showToolbar);
    updateToolbarActionText(showToolbar);

    m_bookmarksToolbar->setVisible(showBookmarksBar);
    updateBookmarksToolbarActionText(showBookmarksBar);

    //statusBar()->setVisible(showStatusbar);
    updateStatusbarActionText(showStatusbar);

    if (!tabWidget()->restoreState(tabState))
        return false;

    return true;
}

void MainWindow::setupToolBar()
{
    setUnifiedTitleAndToolBarOnMac(true);
    m_navigationBar = addToolBar(tr("Navigation"));
    connect(m_navigationBar->toggleViewAction(), SIGNAL(toggled(bool)),
            this, SLOT(updateToolbarActionText(bool)));

    m_viewToolbar = new QAction(this);
    updateToolbarActionText(true);
    m_viewToolbar->setShortcut(tr("Ctrl+|"));
    connect(m_viewToolbar, SIGNAL(triggered()), this, SLOT(slotViewToolbar()));
    //viewMenu->addAction(m_viewToolbar);

    m_historyBack = new QAction(tr("Back"), this);
    m_tabWidget->addWebAction(m_historyBack, QWebPage::Back);
    m_historyBack->setShortcuts(QKeySequence::Back);
    m_historyBack->setIconVisibleInMenu(false);

    m_historyForward = new QAction(tr("Forward"), this);
    m_tabWidget->addWebAction(m_historyForward, QWebPage::Forward);
    m_historyForward->setShortcuts(QKeySequence::Forward);
    m_historyForward->setIconVisibleInMenu(false);

    m_historyBack->setIcon(style()->standardIcon(QStyle::SP_ArrowBack, 0, this));
    m_historyBackMenu = new QMenu(this);
    m_historyBack->setMenu(m_historyBackMenu);
    connect(m_historyBackMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotAboutToShowBackMenu()));
    connect(m_historyBackMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(slotOpenActionUrl(QAction*)));
    m_navigationBar->addAction(m_historyBack);

    m_historyForward->setIcon(style()->standardIcon(QStyle::SP_ArrowForward, 0, this));
    m_historyForwardMenu = new QMenu(this);
    connect(m_historyForwardMenu, SIGNAL(aboutToShow()),
            this, SLOT(slotAboutToShowForwardMenu()));
    connect(m_historyForwardMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(slotOpenActionUrl(QAction*)));
    m_historyForward->setMenu(m_historyForwardMenu);
    m_navigationBar->addAction(m_historyForward);

    m_stopReload = new QAction(this);
    m_reloadIcon = style()->standardIcon(QStyle::SP_BrowserReload);
    m_stopReload->setIcon(m_reloadIcon);

    m_navigationBar->addAction(m_stopReload);

    m_navigationBar->addWidget(m_tabWidget->lineEditStack());

    m_chaseWidget = new ChaseWidget(this);
    m_navigationBar->addWidget(m_chaseWidget);


}

void MainWindow::slotShowBookmarksDialog()
{
    BookmarksDialog *dialog = new BookmarksDialog(this);
    connect(dialog, SIGNAL(openUrl(QUrl)),
            m_tabWidget, SLOT(loadUrlInCurrentTab(QUrl)));
    dialog->show();
}

void MainWindow::slotAddBookmark()
{
    WebView *webView = currentTab();
    QString url = webView->url().toString();
    QString title = webView->title();
    AddBookmarkDialog dialog(url, title);
    dialog.exec();
}

void MainWindow::slotViewToolbar()
{
    if (m_navigationBar->isVisible()) {
        updateToolbarActionText(false);
        m_navigationBar->close();
    } else {
        updateToolbarActionText(true);
        m_navigationBar->show();
    }
    m_autoSaver->changeOccurred();
}

void MainWindow::slotViewBookmarksBar()
{
    if (m_bookmarksToolbar->isVisible()) {
        updateBookmarksToolbarActionText(false);
        m_bookmarksToolbar->close();
    } else {
        updateBookmarksToolbarActionText(true);
        m_bookmarksToolbar->show();
    }
    m_autoSaver->changeOccurred();
}

void MainWindow::updateStatusbarActionText(bool visible)
{
    m_viewStatusbar->setText(!visible ? tr("Show Status Bar") : tr("Hide Status Bar"));
}

void MainWindow::updateToolbarActionText(bool visible)
{
    m_viewToolbar->setText(!visible ? tr("Show Toolbar") : tr("Hide Toolbar"));
}

void MainWindow::updateBookmarksToolbarActionText(bool visible)
{
    m_viewBookmarkBar->setText(!visible ? tr("Show Bookmarks bar") : tr("Hide Bookmarks bar"));
}

void MainWindow::slotViewStatusbar()
{
    if (statusBar()->isVisible()) {
        updateStatusbarActionText(false);
        statusBar()->close();
    } else {
        updateStatusbarActionText(true);
        statusBar()->show();
    }
    m_autoSaver->changeOccurred();
}

void MainWindow::loadUrl(const QUrl &url)
{
    if (!currentTab() || !url.isValid())
        return;

    m_tabWidget->currentLineEdit()->setText(QString::fromUtf8(url.toEncoded()));
    m_tabWidget->loadUrlInCurrentTab(url);
}

void MainWindow::slotDownloadManager()
{
    BrowserApplication::downloadManager()->show();
}

void MainWindow::slotSelectLineEdit()
{
    m_tabWidget->currentLineEdit()->selectAll();
    m_tabWidget->currentLineEdit()->setFocus();
}

void MainWindow::slotFileSaveAs()
{
    BrowserApplication::downloadManager()->download(currentTab()->url(), true);
}

void MainWindow::slotPreferences()
{
    SettingsDialog *s = new SettingsDialog(this);
    s->show();
}

void MainWindow::slotUpdateStatusbar(const QString &string)
{
    statusBar()->showMessage(string, 2000);
}

void MainWindow::slotUpdateWindowTitle(const QString &title)
{
    if (title.isEmpty()) {
        setWindowTitle(tr("Qt Demo Browser"));
    } else {
#if defined(Q_WS_MAC)
        setWindowTitle(title);
#else
        setWindowTitle(tr("%1 - Qt Demo Browser", "Page title and Browser name").arg(title));
#endif
    }
}

void MainWindow::slotAboutApplication()
{
    QMessageBox::about(this, tr("About"), tr(
        "Version %1"
        "<p>This demo demonstrates Qt's "
        "webkit facilities in action, providing an example "
        "browser for you to experiment with.<p>"
        "<p>QtWebKit is based on the Open Source WebKit Project developed at <a href=\"http://webkit.org/\">http://webkit.org/</a>."
        ).arg(QCoreApplication::applicationVersion()));
}

void MainWindow::slotFileNew()
{
    BrowserApplication::instance()->newMainWindow();
    BrowserMainWindow *mw = BrowserApplication::instance()->mainWindow();
    mw->slotHome();
}

void MainWindow::slotFileOpen()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Web Resource"), QString(),
            tr("Web Resources (*.html *.htm *.svg *.png *.gif *.svgz);;All files (*.*)"));

    if (file.isEmpty())
        return;

    loadPage(file);
}

void MainWindow::slotFilePrintPreview()
{
#ifndef QT_NO_PRINTER
    if (!currentTab())
        return;
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(this);
    connect(dialog, SIGNAL(paintRequested(QPrinter*)),
            currentTab(), SLOT(print(QPrinter*)));
    dialog->exec();
#endif
}

void MainWindow::slotFilePrint()
{
    if (!currentTab())
        return;
    printRequested(currentTab()->page()->mainFrame());
}

void MainWindow::printRequested(QWebFrame *frame)
{
#ifndef QT_NO_PRINTER
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    frame->print(&printer);
#endif
}

void MainWindow::slotPrivateBrowsing()
{
    QWebSettings *settings = QWebSettings::globalSettings();
    bool pb = settings->testAttribute(QWebSettings::PrivateBrowsingEnabled);
    if (!pb) {
        QString title = tr("Are you sure you want to turn on private browsing?");
        QString text = tr("<b>%1</b><br><br>When private browsing in turned on,"
            " webpages are not added to the history,"
            " items are automatically removed from the Downloads window," \
            " new cookies are not stored, current cookies can't be accessed," \
            " site icons wont be stored, session wont be saved, " \
            " and searches are not added to the pop-up menu in the Google search box." \
            "  Until you close the window, you can still click the Back and Forward buttons" \
            " to return to the webpages you have opened.").arg(title);

        QMessageBox::StandardButton button = QMessageBox::question(this, QString(), text,
                               QMessageBox::Ok | QMessageBox::Cancel,
                               QMessageBox::Ok);
        if (button == QMessageBox::Ok) {
            settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
        }
    } else {
        settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);

        QList<BrowserMainWindow*> windows = BrowserApplication::instance()->mainWindows();
        for (int i = 0; i < windows.count(); ++i) {
            BrowserMainWindow *window = windows.at(i);
            window->m_lastSearch = QString::null;
            window->tabWidget()->clear();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_tabWidget->count() > 1) {
        int ret = QMessageBox::warning(this, QString(),
                           tr("Are you sure you want to close the window?"
                              "  There are %1 tabs open").arg(m_tabWidget->count()),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    event->accept();
    deleteLater();
}

void MainWindow::slotEditFind()
{
    if (!currentTab())
        return;
    bool ok;
    QString search = QInputDialog::getText(this, tr("Find"),
                                          tr("Text:"), QLineEdit::Normal,
                                          m_lastSearch, &ok);
    if (ok && !search.isEmpty()) {
        m_lastSearch = search;
        if (!currentTab()->findText(m_lastSearch))
            slotUpdateStatusbar(tr("\"%1\" not found.").arg(m_lastSearch));
    }
}

void MainWindow::slotEditFindNext()
{
    if (!currentTab() && !m_lastSearch.isEmpty())
        return;
    currentTab()->findText(m_lastSearch);
}

void MainWindow::slotEditFindPrevious()
{
    if (!currentTab() && !m_lastSearch.isEmpty())
        return;
    currentTab()->findText(m_lastSearch, QWebPage::FindBackward);
}

void MainWindow::slotViewZoomIn()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(currentTab()->zoomFactor() + 0.1);
}

void MainWindow::slotViewZoomOut()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(currentTab()->zoomFactor() - 0.1);
}

void MainWindow::slotViewResetZoom()
{
    if (!currentTab())
        return;
    currentTab()->setZoomFactor(1.0);
}

void MainWindow::slotViewZoomTextOnly(bool enable)
{
    if (!currentTab())
        return;
    currentTab()->page()->settings()->setAttribute(QWebSettings::ZoomTextOnly, enable);
}

void MainWindow::slotViewFullScreen(bool makeFullScreen)
{
    if (makeFullScreen) {
        showFullScreen();
    } else {
        if (isMinimized())
            showMinimized();
        else if (isMaximized())
            showMaximized();
        else showNormal();
    }
}

void MainWindow::slotViewPageSource()
{
    if (!currentTab())
        return;

    QString markup = currentTab()->page()->mainFrame()->toHtml();
    QPlainTextEdit *view = new QPlainTextEdit(markup);
    view->setWindowTitle(tr("Page Source of %1").arg(currentTab()->title()));
    view->setMinimumWidth(640);
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->show();
}

void MainWindow::slotHome()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("MainWindow"));
    QString home = settings.value(QLatin1String("home"), QLatin1String("http://qt.nokia.com/")).toString();
    loadPage(home);
}

void MainWindow::slotWebSearch()
{
    m_toolbarSearch->lineEdit()->selectAll();
    m_toolbarSearch->lineEdit()->setFocus();
}

void MainWindow::slotToggleInspector(bool enable)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, enable);
    if (enable) {
        int result = QMessageBox::question(this, tr("Web Inspector"),
                                           tr("The web inspector will only work correctly for pages that were loaded after enabling.\n"
                                           "Do you want to reload all pages?"),
                                           QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            m_tabWidget->reloadAllTabs();
        }
    }
}

void MainWindow::slotSwapFocus()
{
    if (currentTab()->hasFocus())
        m_tabWidget->currentLineEdit()->setFocus();
    else
        currentTab()->setFocus();
}

void MainWindow::loadPage(const QString &page)
{
    QUrl url = QUrl::fromUserInput(page);
    loadUrl(url);
}

TabWidget *MainWindow::tabWidget() const
{
    return m_tabWidget;
}

WebView *MainWindow::currentTab() const
{
    return m_tabWidget->currentWebView();
}

void MainWindow::slotLoadProgress(int progress)
{
    if (progress < 100 && progress > 0) {
        m_chaseWidget->setAnimated(true);
        disconnect(m_stopReload, SIGNAL(triggered()), m_reload, SLOT(trigger()));
        if (m_stopIcon.isNull())
            m_stopIcon = style()->standardIcon(QStyle::SP_BrowserStop);
        m_stopReload->setIcon(m_stopIcon);
        connect(m_stopReload, SIGNAL(triggered()), m_stop, SLOT(trigger()));
        m_stopReload->setToolTip(tr("Stop loading the current page"));
    } else {
        m_chaseWidget->setAnimated(false);
        disconnect(m_stopReload, SIGNAL(triggered()), m_stop, SLOT(trigger()));
        m_stopReload->setIcon(m_reloadIcon);
        connect(m_stopReload, SIGNAL(triggered()), m_reload, SLOT(trigger()));
        m_stopReload->setToolTip(tr("Reload the current page"));
    }
}

void MainWindow::slotAboutToShowBackMenu()
{
    m_historyBackMenu->clear();
    if (!currentTab())
        return;
    QWebHistory *history = currentTab()->history();
    int historyCount = history->count();
    for (int i = history->backItems(historyCount).count() - 1; i >= 0; --i) {
        QWebHistoryItem item = history->backItems(history->count()).at(i);
        QAction *action = new QAction(this);
        action->setData(-1*(historyCount-i-1));
        QIcon icon = BrowserApplication::instance()->icon(item.url());
        action->setIcon(icon);
        action->setText(item.title());
        m_historyBackMenu->addAction(action);
    }
}

void MainWindow::slotAboutToShowForwardMenu()
{
    m_historyForwardMenu->clear();
    if (!currentTab())
        return;
    QWebHistory *history = currentTab()->history();
    int historyCount = history->count();
    for (int i = 0; i < history->forwardItems(history->count()).count(); ++i) {
        QWebHistoryItem item = history->forwardItems(historyCount).at(i);
        QAction *action = new QAction(this);
        action->setData(historyCount-i);
        QIcon icon = BrowserApplication::instance()->icon(item.url());
        action->setIcon(icon);
        action->setText(item.title());
        m_historyForwardMenu->addAction(action);
    }
}

void MainWindow::slotAboutToShowWindowMenu()
{
    m_windowMenu->clear();
    m_windowMenu->addAction(m_tabWidget->nextTabAction());
    m_windowMenu->addAction(m_tabWidget->previousTabAction());
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(tr("Downloads"), this, SLOT(slotDownloadManager()), QKeySequence(tr("Alt+Ctrl+L", "Download Manager")));

    m_windowMenu->addSeparator();
    QList<BrowserMainWindow*> windows = BrowserApplication::instance()->mainWindows();
    for (int i = 0; i < windows.count(); ++i) {
        BrowserMainWindow *window = windows.at(i);
        QAction *action = m_windowMenu->addAction(window->windowTitle(), this, SLOT(slotShowWindow()));
        action->setData(i);
        action->setCheckable(true);
//        if (window == this)
//            action->setChecked(true);
    }
}

void MainWindow::slotShowWindow()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        QVariant v = action->data();
        if (v.canConvert<int>()) {
            int offset = qvariant_cast<int>(v);
            QList<BrowserMainWindow*> windows = BrowserApplication::instance()->mainWindows();
            windows.at(offset)->activateWindow();
            windows.at(offset)->currentTab()->setFocus();
        }
    }
}
// ��������
void MainWindow::slotOpenActionUrl(QAction *action)
{
    int offset = action->data().toInt();

    if(action->isChecked()){
       action->setChecked(true);
    }else{
       action->setChecked(false);
    }

    switch (offset) {
        case ALLDOC:

        case CURRENTDIR:

        case CURRENTDIRINCLUESUB:

        case ALLTEXT:

        case DOCNAMEONLY:
        case OPTION:

        case SAVETOFASTSEARCH:
          ;
    }
 }

void MainWindow::geometryChangeRequested(const QRect &geometry)
{
    setGeometry(geometry);
}


