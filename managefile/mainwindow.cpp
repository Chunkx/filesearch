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
#include "mytreeview.h"
#include "importdocdialog.h"
#include "exportdocdialog.h"
#include "createsubdirdialog.h"
#include "movetodirdialog.h"
#include "movetotagdialog.h"
#include "customtooldialog.h"
#include "propofdirdialog.h"
#include "propoftagdialog.h"
#include "createtagdialog.h"
#include "browser/browsermainwindow.h"
#include "browser/browserapplication.h"
#include "utils.h"
#include "fileutils.h"
#include "db/tagdao.h"
#include "db/dirdao.h"
#include "db/docdao.h"
#include "noteeditor.h"

extern NoteEditor *noteEditor;

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

// 生成文档列表
void MainWindow::buildDocList()
{
    QString curUuid = q_myTreeList->getCurUuid();
    QString curType = q_myTreeList->getCurType();
    if(curType != "doc" ){
        return;
    }

    // 是否显示子文件夹文档
    QList<Doc> docs;
    if(isShowDocUnderSub){
          // 取得所有文件夹
          QList<Dir> dirs ;
          //DirDao::clearSelDirList();
          DirDao::selectAllSubDirbyDir(dirs, curUuid);
          //QList<Dir> dirs = DirDao::getSelDirList();
          // 根据文件夹取得所有文件
          for(int i = 0 ; i< dirs.size(); i ++){
             Dir dir = dirs.at(i);
             docs.append(DocDao::selectDocsbyDir(dir.DIR_GUID));
          }
    }else{
         docs = DocDao::selectDocsbyDir(curUuid);
    }
    m_doctable->buildDocList(docs);
}

// 搜索界面设置Menu
void MainWindow::slotShowSearchSetMenu()
{
    if(m_searchSetMenu->children().length() > 2){
      return;
    }

    // 设置Menu
    QAction *action1 = new QAction(this);
    action1->setData(ALLDOC);
    action1->setCheckable(true);
    action1->setText(tr("ALL Documents"));

    // 设置Menu
    QAction *action2 = new QAction(this);
    action2->setData(CURRENTDIR);
    action2->setCheckable(true);
    action2->setText(tr("Current Directory"));

    // 设置Menu
    QAction *action3 = new QAction(this);
    action3->setData(CURRENTDIRINCLUESUB);
    action3->setCheckable(true);
    action3->setText(tr("Current Directory(Include Sub Directory)"));

    // 设置Menu
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

// 初始化界面动作
void MainWindow::initActions()
{

        // 设置搜索面板
        m_searchSetAction = new QAction(tr("Search"), this);
        m_searchSetAction->setIconVisibleInMenu(false);
        m_searchSetMenu = new QMenu(this);
        m_searchSetAction->setMenu(m_searchSetMenu);
        connect(m_searchSetMenu, SIGNAL(aboutToShow()), this, SLOT(slotShowSearchSetMenu()));
        connect(m_searchSetMenu, SIGNAL(triggered(QAction*)),this, SLOT(slotOpenActionUrl(QAction*)));

        // 导入
        importAction = new QAction(Utils::getIcon("document-import.png"),tr("&Import..."), this);
        importAction->setShortcut(tr("Ctrl+I"));
        connect(importAction, SIGNAL(triggered()), this, SLOT(importDlg()));

        // 导出
        exportAction = new QAction(Utils::getIcon("document-export.png"),tr("&Export..."), this);
        exportAction->setShortcut(tr("Ctrl+E"));
        connect(exportAction, SIGNAL(triggered()), this, SLOT(exportDlg()));

        // 另存为
        saveAsAction = new QAction(Utils::getIcon("document-save-as.png"),tr("&Save As..."), this);
        saveAsAction->setShortcut(tr("Ctrl+S"));
        connect(saveAsAction, SIGNAL(triggered()), this, SLOT(about()));

        // 另存到移动设备
        saveToMobiAction = new QAction(Utils::getIcon("document-savetomobi.png"),tr("&Save To Mobile..."), this);
        connect(saveToMobiAction, SIGNAL(triggered()), this, SLOT(about()));

        // 退出
        exitAction = new QAction(Utils::getIcon("close.png"),tr("&Exit"), this);
        exitAction->setShortcut(tr("Ctrl+Q"));
        connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

        // 全屏
        fullScreenAction = new QAction(Utils::getIcon("view_fullscreen.png"),tr("&Full Screen"), this);
        fullScreenAction->setShortcut(tr("Ctrl+F"));
        connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreen()));

        // 显示/隐藏分类树
        showClassTreeAction = new QAction(Utils::getIcon("view_tree.png"),tr("&Show/Hide Class Tree"), this);
        showClassTreeAction->setShortcut(tr("Ctrl+H"));
        connect(showClassTreeAction, SIGNAL(triggered()), this, SLOT(showClassTree()));

        // 皮肤
        skinAction = new QAction(Utils::getIcon("skins.png"),tr("&Skin"), this);
        menu_skin = new QMenu(this);
        skinAction->setMenu(menu_skin);

        //皮肤 默认
        defaultAction = new QAction(tr("&Defaut"), this);
        connect(defaultAction, SIGNAL(triggered()), this, SLOT(about()));

       // 皮肤 书香
        bookAction = new QAction(tr("&Book"), this);
        connect(bookAction, SIGNAL(triggered()), this, SLOT(about()));

        //皮肤 灰色
        grayAction = new QAction(tr("&Gray"), this);
        connect(grayAction, SIGNAL(triggered()), this, SLOT(about()));

        // 自定义工具栏
        customToolbarAction = new QAction(Utils::getIcon("toolbar.png"),tr("&Setting ToolBar"), this);
        connect(customToolbarAction, SIGNAL(triggered()), this, SLOT(customToolBar()));

        // 显示/隐藏工具栏
        showToolbarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Tool Bar"), this);
        connect(showToolbarAction, SIGNAL(triggered()), this, SLOT(showToolBar()));

        // 显示/隐藏状态栏
        showSatebarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Status Bar..."), this);
        connect(showSatebarAction, SIGNAL(triggered()), this, SLOT(about()));

        // 用户手册
        userBookAction = new QAction(Utils::getIcon("help_viewer.png"),tr("&Help"), this);
        userBookAction->setShortcut(tr("F2"));
        connect(userBookAction, SIGNAL(triggered()), this, SLOT(helpDoc()));

        // 个人主页
        homepageAction = new QAction(Utils::getIcon("homepage.png"),tr("&HomePage"), this);
        connect(homepageAction, SIGNAL(triggered()), this, SLOT(homepage()));

        // 邀请朋友
        inviteAction = new QAction(Utils::getIcon("invite.png"),tr("&Invite Friends..."), this);
        connect(inviteAction, SIGNAL(triggered()), this, SLOT(about()));

        // 论坛
        bbsAction = new QAction(Utils::getIcon("forum.png"),tr("&Forum"), this);
        connect(bbsAction, SIGNAL(triggered()), this, SLOT(bbs()));

        // 关于
        aboutAction = new QAction(Utils::getIcon("about.png"),tr("&About"), this);
        connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

        // 检查新版本
        checkNewAction = new QAction(Utils::getIcon("update_product.png"),tr("&Update"), this);
        connect(checkNewAction, SIGNAL(triggered()), this, SLOT(about()));

        // 选项
        optionAction = new QAction(Utils::getIcon("option.png"),tr("&Option"), this);
        optionAction->setShortcut(tr("Ctrl+O"));
        connect(optionAction, SIGNAL(triggered()), this, SLOT(about()));

        // 查看log
        viewLogAction = new QAction(Utils::getIcon("log.png"),tr("&View Log..."), this);
        viewLogAction->setShortcut(tr("Ctrl+L"));
        connect(viewLogAction, SIGNAL(triggered()), this, SLOT(about()));

        // 插件管理
        pluginAction = new QAction(Utils::getIcon("plugin.png"),tr("Plugins"), this);
        pluginAction->setShortcut(tr("Ctrl+P"));
        connect(pluginAction, SIGNAL(triggered()), this, SLOT(about()));;

        // 皮肤
        languageAction = new QAction(Utils::getIcon("language.png"),tr("&Language"), this);
        menu_language = new QMenu(this);
        languageAction->setMenu(menu_language);

        // 英文版
        enAction = new QAction(tr("&English"), this);
        connect(enAction, SIGNAL(triggered()), this, SLOT(about()));

        // 中文版
        cnAction = new QAction(tr("&Chinese"), this);
        connect(cnAction, SIGNAL(triggered()), this, SLOT(about()));

        // 日文版
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

        showSubDirDoc= new QAction(tr("&Show docs under sub Dir"), this);
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
        connect(makeSubTag, SIGNAL(triggered()), this, SLOT(newTag()));
        moveToTag= new QAction(tr("&Move to Tag"), this);
        connect(moveToTag, SIGNAL(triggered()), this, SLOT(movetoTag()));
        moveToRootTag= new QAction(tr("&Move to Root Tag"), this);
        connect(moveToRootTag, SIGNAL(triggered()), this, SLOT(moveToRoot()));
        delTag= new QAction(tr("&Delete"), this);
        connect(delTag, SIGNAL(triggered()), this, SLOT(deleteTag()));
        renameTag= new QAction(tr("&Rename"), this);
        connect(renameTag, SIGNAL(triggered()), this, SLOT(renameSubTag()));
        showSubDirTag= new QAction(tr("&Show docs under sub Tag"), this);
        showSubDirTag->setCheckable(true);
        connect(showSubDirTag, SIGNAL(triggered()), this, SLOT(setShowSubTagDoc()));

        showSubDirTag->setChecked(p->isShowDocUnderSubTag());

        propOfTag= new QAction(tr("&Properties"), this);
        connect(propOfTag, SIGNAL(triggered()), this, SLOT(showPropOfTag()));

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

        // 右键
        contextMenu = new QMenu(this);
        contextMenu->setStyleSheet(
                    "QMenu::item{height: 25px}"
                    "QMenu::item:hover{background-color:rgb(100,100,100)}"
                    "QMenu::item:selected{background-color:rgb(128,128,128)}"
        );
        contextMenu->hide();
}

// 初始化浏览器
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

    // 加入spplitter
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

//    // 改变m_tabwiget大小时
//    connect(m_doctable, SIGNAL(resize(QResizeEvent *event)), this, SLOT(resizeSpace()));

    slotUpdateWindowTitle();
    loadDefaultState();
    m_tabWidget->newTab();

    int size = m_tabWidget->lineEditStack()->sizeHint().height();
    m_navigationBar->setIconSize(QSize(size, size));
}


//// 调节空白区域大小
//void MainWindow::resizeSpace()
//{
//     sapcewidget->setMinimumWidth(m_doctable->width()*1.2);
//}

// 状态栏
void MainWindow::initStatusbar()
{
  // QLabel* m_sqliteVersionLabel = new QLabel(this);
  //statusBar()->addPermanentWidget(&m_sqliteVersionLabel);
}

// 工具栏
void MainWindow::initToolbar()
{
    // 读取preferences
    Preferences* p = Preferences::instance();
    QStringList waitItems = p->getWaitToolbarItemList();
    QStringList selItems = p->getSelToolbarItemList();

    toolBar = addToolBar(tr("Tool Bar"));
    if(selItems.length() != 0){
        upateToolBar(waitItems, selItems);
    }else{
        // 默认状态下
        toolBar->addAction(showClassTreeAction);
        toolBar->addSeparator();
        toolBar->addAction(homepageAction);
        toolBar->addAction(inviteAction);
        toolBar->addAction(bbsAction);
        toolBar->addSeparator();
    }

    // 空白区域
    QWidget *twidget = new QWidget(this);
    twidget->setMinimumWidth(this->geometry().width() * 0.1);
    toolBar->addWidget(twidget);

    m_toolbarSearch = new ToolbarSearch();
    connect(m_toolbarSearch, SIGNAL(search(QUrl)), SLOT(loadUrl(QUrl)));

    toolBar->addWidget(m_toolbarSearch);
    toolBar->addAction(m_searchSetAction);
    toolBar->addSeparator();
    toolBar->addAction(fullScreenAction);

    // 空白区域
    sapcewidget = new QWidget(this);
    sapcewidget->setMinimumWidth(m_doctable->width()*1.2);
    toolBar->addWidget(sapcewidget);
}

// 用户手册
void MainWindow::helpDoc()
{
     m_tabWidget->newTab();
     loadUrl(QUrl("http://www.slfile.net/?page_id=58"));
}

// 论坛
void MainWindow::bbs()
{
    m_tabWidget->newTab();
    loadUrl(QUrl("http://www.slfile.net/?post_type=forum"));

}

// 主页
void MainWindow::homepage()
{
    m_tabWidget->newTab();
    loadUrl(QUrl("http://www.slfile.net/"));
}

// 设置是否显示子文件夹下的文件
void MainWindow::setShowSubDirDoc()
{
    // 反向选择
    isShowDocUnderSub = showSubDirDoc->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSub(isShowDocUnderSub);
}

// 设置是否显示子标签下的文件
void MainWindow::setShowSubTagDoc()
{
    // 反向选择
    isShowDocUnderTag = showSubDirTag->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSubTag(isShowDocUnderTag);
}

// 打开导入界面
void MainWindow::importDlg()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中子节点
    if(!curType.isEmpty() && curType != "alldocs" && curType != "alltags") {
        hasSelRight = true;
        ImportDocDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            int row= dlg.model->rowCount();
            // 存储每一层文件夹名 <1, dirList>
            QMap<int, QStringList> writeMap;

           QString parentUuId;
            for (int var = 0; var < row; ++var) {

                QStandardItem* temp = dlg.model->item(var);
                QString path = temp->text();

                QString filepath = path.left(path.lastIndexOf(QDir::separator()));
                filepath = filepath.replace("\"","");

                // 设置目标目录
                QString destDir = filepath.remove(0, dlg.m_importDir.length());
                QStringList dirs = destDir.split(QDir::separator());

                // 建立目录
                for (int i = 0; i < dirs.length(); ++i) {
                    // 插入文档
                    if(i = dirs.length() - 1){
                        QUuid docUuid = QUuid::createUuid();
                        QFileInfo fileinfo(path);
                        Doc doc;
                        doc.DOCUMENT_GUID = docUuid;
                        doc.DOCUMENT_TITLE = fileinfo.fileName();
                        // 父目录
                        if(parentUuId.isEmpty()){
                           // 选择树节点UuId

                        }

                        doc.DIR_GUID = parentUuId;
                        doc.DOCUMENT_LOCATION = path;
                        doc.DOCUMENT_NAME = fileinfo.fileName();
                        doc.DOCUMENT_SEO = "";
                        doc.DOCUMENT_URL = "";
                        doc.DOCUMENT_AUTHOR = fileinfo.owner();
                        doc.DOCUMENT_KEYWORDS = "";
                        doc.DOCUMENT_TYPE = "";
                        doc.DOCUMENT_OWNER = fileinfo.owner();
                        doc.DT_CREATED = fileinfo.created().toString("yyyy-MM-dd hh:mm:ss");
                        doc.DT_MODIFIED = fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
                        doc.DT_ACCESSED = fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss");
                        doc.DOCUMENT_ICON_INDEX = 0;
                        doc.DOCUMENT_SYNC = 0;
                        doc.DOCUMENT_PROTECT = "";
                        doc.DOCUMENT_ENCODE= "0";
                        doc.DOCUMENT_READ_COUNT = 0;
                        doc.DOCUMENT_RELATE_COUNT = 0;
                        doc.DOCUMENT_INDEXFLG = "0";
                        doc.DOCUMENT_OPERFLG = "";
                        doc.DELETE_FLAG = "0";
                        doc.MF_VERSION = 0;

                        DocDao::insertDoc(doc);
                    }

                      QString tmpDir = dirs.at(i);
                      if(tmpDir.isEmpty()){
                          continue;
                      }

                      QStringList exitedDirs = writeMap[i];
                      if(exitedDirs.contains(tmpDir)){
                           continue;
                      } else {
                          QUuid uuid = QUuid::createUuid();
                          Dir dir;
                          dir.DIR_GUID = uuid;
                          dir.DIR_NAME = tmpDir;
                          dir.DIR_PARENT_UUID = parentUuId;
                          dir.DIR_ORDER = var;
                          dir.MF_VERSION = 1;

                          DirDao::insertDir(dir);
                          exitedDirs.append(tmpDir);
                          writeMap.insert(i, exitedDirs);
                          parentUuId = uuid;
                      }
                }
            }

            // 重新加载树节点
            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->delSubItems(curItem);
            QString uuId = q_myTreeList->getCurUuid();
            q_myTreeList->loadDirs(uuId, curItem);
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub directory."), QMessageBox::Yes);
        return;
    }
}

// 打开导出界面
void MainWindow::exportDlg()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中子节点
    if(!curType.isEmpty() && curType != "alldocs" && curType != "alltags") {
        hasSelRight = true;
        ExportDocDialog dlg(this, m_baseDir, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
          // 不做任何操作
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub directory."), QMessageBox::Yes);
        return;
    }
}

// 查看文件夹属性
void MainWindow::properties()
{
    QString curType = q_myTreeList->getCurType();
    QString curTitle = q_myTreeList->getCurTitle();
    bool hasSelRight = false;

    // 需选中子节点
    if(curType == "doc") {
        hasSelRight = true;
        PropOfDirDialog dlg(this, curTitle, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
          // 不做任何操作
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

void MainWindow::initUI()
{
        setWindowTitle(m_appName);

        splitter = new QSplitter(this);
        // 加载分类树
        m_baseDir = Utils::getLocatePath();
        m_baseDir = QDir::toNativeSeparators(m_baseDir);
        q_myTreeList = new MyTreeView("all", this);

        // 空的UuId
        q_myTreeList->enableMouse(true);

        m_doctable = new MyTableView(this);

        splitter->addWidget(q_myTreeList);
        splitter->addWidget(m_doctable);

        connect(q_myTreeList, SIGNAL(RBtnClk()), this, SLOT(treeContextMenuOpened()));
        connect(q_myTreeList, SIGNAL(LBtnClk()), this, SLOT(buildDocList()));

        // 后期需要调用主界面的动作
        connect(m_doctable, SIGNAL(LBtnDbClk()), this, SLOT(openDocInTab()));
        connect(m_doctable, SIGNAL(showAddNoteWidget()), this, SLOT(windowToggleNoteEditor()));
        connect(m_doctable, SIGNAL(hideNoteWidget()), this, SLOT(windowHideNoteEditor()));
        connect(m_doctable, SIGNAL(reloadTagTree()), q_myTreeList, SLOT(reloadTagTree()));


        // Dock widgets ///////////////////////////////////////////////
        //QDockWidget *dw;
        //if (settings.value("/satellite/noteeditor/isDockWindow",true).toBool())
        //{
            noteEditorDW = new QDockWidget(tr("Note Editor"), this);
            noteEditorDW->setWidget (noteEditor);
            noteEditorDW->setObjectName ("NoteEditor");
            addDockWidget(Qt::RightDockWidgetArea, noteEditorDW);
            noteEditorDW->hide();
            noteEditor->setShowWithMain(true);
            // 从Note编辑界面打开Note
            connect(noteEditor, SIGNAL(showMainNotes()), this, SLOT(shownotes()));

        //} else{
          //  noteEditorDW=NULL;
        //}


        setCentralWidget(splitter);
}

void MainWindow::shownotes(){
    m_doctable->showNoteDialog();
}


MainWindow::~MainWindow()
{
    m_autoSaver->changeOccurred();
    m_autoSaver->saveIfNeccessary();
    Preferences::deleteInstance();
}

// 创建子文件夹
void MainWindow::createSubDir()
{
    QString curType = q_myTreeList->getCurType();
    QString curUuid = q_myTreeList->getCurUuid();
    bool hasSelRight = false;

    // 需选中 总节点和子节点
    if(curType == "doc") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, curUuid, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
            // 刷新选中的树
            QStandardItem* curItem = q_myTreeList->getCurItem();
            q_myTreeList->addItemByParentItem(curItem, dlg.dirName->text(), dlg.m_newUuid, "doc", "folder.ico");
            q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// 移动文件夹
void MainWindow::moveDir()
{
    QString curType = q_myTreeList->getCurType();
    QString curUuid = q_myTreeList->getCurUuid();
    QString curTitle = q_myTreeList->getCurTitle();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    bool hasSelRight = false;

    // 需选中 文件 子节点
    if(curType == "doc") {
        hasSelRight = true;
        MoveToDirDialog dlg(this, curUuid, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
              // 取得子界面选中的path
              QString toDirUuid = dlg.m_toUuid;
              // 删除主界面选中的节点
              curItem->parent()->removeRow(curItem->row());

              // 设置主界面的节点 (子界面新建文件夹情况下不成功)
              q_myTreeList->setCurItemByUuid(toDirUuid, curType);

              QStandardItem* curItem = q_myTreeList->getCurItem();
              q_myTreeList->addItemByParentItem(curItem, curTitle, curUuid, "doc", "folder.ico");
              q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}


// 改变子文件夹名称
void MainWindow::renameSubDir()
{
    QString curType = q_myTreeList->getCurType();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    QString curTitle = q_myTreeList->getCurTitle();
    bool hasSelRight = false;

    // 需选中 总节点和子节点
    if(!curType.isEmpty() && curType != "alltags") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Directory name:"), QLineEdit::Normal, curTitle, &ok);
        if (ok && !text.isEmpty()) {
                if (text == curTitle){
                  return;
                }
                // 改变树节点
                curItem->setData(text, Qt::DisplayRole);
                // 改变目录名称
                Dir dir;
                dir.DIR_GUID = q_myTreeList->getCurUuid();
                dir.DIR_NAME = text;
                DirDao::updateDir(dir);
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// 新建根节点
void MainWindow::createRootDir()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中 总节点
    if(curType == "alldocs") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Roor Directory name:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
                if (text == ""){
                  return;
                }
                // 刷新选中的树
                QString tempPath = m_baseDir;
                tempPath.append(QDir::separator()).append(text);

                // 新建文件夹
                QDir *mdir = new QDir(m_baseDir);
                if(mdir->exists()){
                    if(!mdir->mkdir(text)){
                        QMessageBox::warning(this, tr("Error"), tr("Create New Root Directory failed. "), QMessageBox::Yes);
                        return;
                    }
                }

                QStandardItem* curItem = q_myTreeList->getCurItem();
                q_myTreeList->addItemByParentItem(curItem, text, tempPath, "doc", "folder.ico");
                curItem->setIcon(Utils::getIcon("expander_open.png"));
                q_myTreeList->expand(q_myTreeList->getCurIndex());

        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// 新建根标签
void MainWindow::newTag()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中 标签 节点
    if(curType == "alltags" || curType == "tag") {
        hasSelRight = true;
        // curPath 用于判断根节点和子节点
        CreateTagDialog dlg(this, q_myTreeList->getCurPath());
        dlg.exec();
        if(dlg.update){
              QStandardItem* curItem = q_myTreeList->getCurItem();
              q_myTreeList->addItemByParentItem(curItem,  dlg.m_tagname, dlg.m_newTagUuId, "tag", "tag.ico");
              curItem->setIcon(Utils::getIcon("expander_open.png"));
              q_myTreeList->expand(q_myTreeList->getCurIndex());
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// 删除标签
void MainWindow::deleteTag()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;
    QString title = q_myTreeList->getCurTitle();
    QString curUuId = q_myTreeList->getCurUuid();

    // 需选中 标签 节点
    if(curType == "tag") {
        hasSelRight = true;
        int ret = QMessageBox::warning(this, QString(),
                           tr("Are you sure you want to delete the %1 tag?").arg(title),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);

        if (ret == QMessageBox::No) {
            return;
        }
        if(ret == QMessageBox::Yes){
          q_myTreeList->delSubTree();
          // 删除Tag
          TagDao::deleteTag(curUuId);
        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an Sub Tag."), QMessageBox::Yes);
        return;
    }
}

// 改变子标签名称
void MainWindow::renameSubTag()
{
    QString curType = q_myTreeList->getCurType();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    QString curTitle = q_myTreeList->getCurTitle();
    QString curUuId = q_myTreeList->getCurUuid();

    bool hasSelRight = false;

    // 需选中 总节点和子节点
    if(curType == "tag") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Tag name:"), QLineEdit::Normal, curTitle, &ok);
        if (ok && !text.isEmpty()) {
                if (text == curTitle){
                  return;
                }
                // 改变树节点
                curItem->setData(text, Qt::DisplayRole);
                // 改变标签名称
                // 删除Tag
                Tag tag;
                tag.TAG_GUID = curUuId;
                tag.TAG_NAME = text;
                TagDao::updateTag(tag);
        }

    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// 显示标签参数
void MainWindow::showPropOfTag()
{
    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中 标签 节点
    if(curType == "tag") {
        QString tagname = q_myTreeList->getCurTitle();
        QStandardItem*  curItem = q_myTreeList->getCurItem();
        QString curUuId = q_myTreeList->getCurUuid();
        Tag tag = TagDao::selectTag(curUuId);
        hasSelRight = true;
        // curPath 用于判断根节点和子节点
        // getCurPath 用于存UuId
        PropOfTagDialog dlg(this, tag.TAG_GUID, tagname, tag.TAG_DESCRIPTION);
        dlg.exec();
        if(dlg.update){
            if (dlg.m_tagname.isEmpty()){
              // 提示出错
              return;
            }
            // 改变树节点
            curItem->setData(dlg.m_tagname, Qt::DisplayRole);

        }
    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

void MainWindow:: moveToRoot(){

    QString curType = q_myTreeList->getCurType();
    bool hasSelRight = false;

    // 需选中 标签 节点
    if(curType == "tag" ) {
        QString curUuid = q_myTreeList->getCurUuid();
        Tag tag = TagDao::selectTag(curUuid);
        hasSelRight = true;
        tag.TAG_GROUP_GUID = "";
        TagDao::updateTag(tag);
        // 重新load的标签树
        // 删除当前的节点
        q_myTreeList->delSubTree();

        // 设置主界面节点
        q_myTreeList->addItem(1, tag.TAG_NAME, tag.TAG_GUID, "expander_normal.png");

    }
    // 如果没有选中子目录节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an Tag."), QMessageBox::Yes);
        return;
    }
}

// 移动标签
void MainWindow::movetoTag()
{
    QString curType = q_myTreeList->getCurType();
    QStandardItem* curItem = q_myTreeList->getCurItem();
    QString curTitle = q_myTreeList->getCurTitle();
    QString curPath = q_myTreeList->getCurPath();
    QString curUuId = q_myTreeList->getCurUuid();
    bool hasSelRight = false;

    // 需选中 文件 子节点
    if(curType == "tag") {
        hasSelRight = true;
        MoveToTagDialog dlg(this, curPath);
        dlg.exec();
        if(dlg.update){
              // 取得子界面选中的path
              QString mselUuId = dlg.m_selUuId;
              // 删除主界面选中的节点
              curItem->parent()->removeRow(curItem->row());

              // 设置主界面的节点 (子界面新建文件夹情况下不成功)
              q_myTreeList->setCurItemByUuid(curUuId,  curType);

              QString curPath = q_myTreeList->getCurPath();
              if(!curPath.isEmpty()){
                  QStandardItem* curItem = q_myTreeList->getCurItem();
                  q_myTreeList->addItemByParentItem(curItem, curTitle, mselUuId, "tag", "tag.ico");
                  curItem->setIcon(Utils::getIcon("expander_open.png"));
                  q_myTreeList->expand(q_myTreeList->getCurIndex());
              }
        }
    }
    // 如果没有选中子标签节点
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub tag."), QMessageBox::Yes);
        return;
    }
}

// 自定义工具栏
void MainWindow::customToolBar()
{
    CustomToolDialog dlg(this);
    dlg.exec();
    if(dlg.update){
        // 更新工具栏
        upateToolBar(dlg.waitItems, dlg.sellItems);
    }
}
// 更新工具栏
void MainWindow::upateToolBar(QStringList waitItems, QStringList selItems)
{

    QList<QAction*> actions = toolBar->actions();

    // 分类树
    if(selItems.contains("view_tree.png") && ! waitItems.contains("view_tree.png")){
        if(!actions.contains(showClassTreeAction)){
             toolBar->addAction(showClassTreeAction);
        }
    }
    if(! selItems.contains("view_tree.png") && waitItems.contains("view_tree.png")){
         if(actions.contains(showClassTreeAction)){
             toolBar->removeAction(showClassTreeAction);
         }
    }
    // 全屏
    if(selItems.contains("view_fullscreen.png") && ! waitItems.contains("view_fullscreen.png")){
          if(!actions.contains(fullScreenAction)){
               toolBar->addAction(fullScreenAction);
          }
    }
    if(! selItems.contains("view_fullscreen.png") && waitItems.contains("view_fullscreen.png")){
          if(actions.contains(fullScreenAction)){
            toolBar->removeAction(fullScreenAction);
          }
    }
    // 个人主页
    if(selItems.contains("homepage.png") && ! waitItems.contains("homepage.png")){
          if(!actions.contains(homepageAction)){
             toolBar->addAction(homepageAction);
          }
    }
    if(! selItems.contains("homepage.png") && waitItems.contains("homepage.png")){
          if(actions.contains(homepageAction)){
             toolBar->removeAction(homepageAction);
          }
    }
    // 邀请朋友
    if(selItems.contains("invite.png") && ! waitItems.contains("invite.png")){
          if(!actions.contains(inviteAction)){
             toolBar->addAction(inviteAction);
          }
    }
    if(! selItems.contains("invite.png") && waitItems.contains("invite.png")){
         if(actions.contains(inviteAction)){
             toolBar->removeAction(inviteAction);
         }
    }
    // 论坛
    if(selItems.contains("forum.png") && ! waitItems.contains("forum.png")){
         if(!actions.contains(bbsAction)){
             toolBar->addAction(bbsAction);
         }
    }
    if(!selItems.contains("forum.png") && waitItems.contains("forum.png")){
          if(actions.contains(bbsAction)){
              toolBar->removeAction(bbsAction);
          }
    }
    // 另存到移动设备
    if(selItems.contains("document-savetomobi.png") && ! waitItems.contains("document-savetomobi.png")){
            if(!actions.contains(saveToMobiAction)){
               toolBar->addAction(saveToMobiAction);
            }
    }
    if(! selItems.contains("document-savetomobi.png") && waitItems.contains("document-savetomobi.png")){
           if(actions.contains(saveToMobiAction)){
               toolBar->removeAction(saveToMobiAction);
           }
    }
    //  导入
    if(selItems.contains("document-import.png") && ! waitItems.contains("document-import.png")){
          if(!actions.contains(importAction)){
             toolBar->addAction(importAction);
          }
    }
    if(! selItems.contains("document-import.png") && waitItems.contains("document-import.png")){
          if(actions.contains(importAction)){
             toolBar->removeAction(importAction);
          }
    }
    //  导出
    if(selItems.contains("document-export.png") && ! waitItems.contains("document-export.png")){
           if(!actions.contains(exportAction)){
              toolBar->addAction(exportAction);
           }
    }
    if(! selItems.contains("document-export.png") && waitItems.contains("document-export.png")){
           if(actions.contains(exportAction)){
             toolBar->removeAction(exportAction);
           }
    }
    // 插件管理
    if(selItems.contains("plugin.png") && ! waitItems.contains("plugin.png")){
         if(!actions.contains(pluginAction)){
               toolBar->addAction(pluginAction);
          }
    }
    if(! selItems.contains("plugin.png") && waitItems.contains("plugin.png")){
         if(actions.contains(pluginAction)){
             toolBar->removeAction(pluginAction);
          }
    }
    toolBar->update();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(windowTitle()), 
                       tr("Sorry, The function is under construction..."));
}

// 删除子文件夹
void MainWindow::delSubDir()
{
    QString curTitle = q_myTreeList->getCurTitle();
    QString curUuid = q_myTreeList->getCurUuid();
    int ret = QMessageBox::question(this, "",
                                    tr("Are you sure that delete the directory \"%1\"?").arg(curTitle),
                                    QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes){
       q_myTreeList->delSubTree();
       DirDao::deleteDir(curUuid);
    }

    if(ret == QMessageBox::No){
       return;
    }
}

// 全屏
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

// 显示/隐藏分类树
void MainWindow::showClassTree()
{
    if(q_myTreeList->isHidden()){
       q_myTreeList->show();
    }else{
       q_myTreeList->hide();
    }
}
// 显示/隐藏工具栏
void MainWindow::showToolBar()
{

    if(toolBar->isHidden()){
       toolBar->show();
    }else{
       toolBar->hide();
    }
}
// 当树节点被选中
void MainWindow::treeItemActivated()
{
        QString curPath = q_myTreeList->getCurPath();
        // 如果
        if(curPath == "alldocs" ||curPath == "alltags"){
          // 表内显示所有文档

        } else{

        }
}

// 打开右键菜单
void MainWindow::treeContextMenuOpened()
{
        tableTree_currentItemChanged();
        QPoint pos = q_myTreeList->getCurPoint();
        if (contextMenu->actions().count() > 1){
           contextMenu->exec(q_myTreeList->viewport()->mapToGlobal(pos));
        }
}
// 打开右键菜单
void MainWindow::tableTree_currentItemChanged()
{
        contextMenu->clear();
        QString type = q_myTreeList->getCurType();

        // 如果选择Root
        if(type == "alldocs"){
            contextMenu->addAction(makeRootDir);
            contextMenu->addSeparator();
            contextMenu->addAction(dirSort);
            contextMenu->addSeparator();
            contextMenu->addAction(protectRootDir);
            contextMenu->addSeparator();
            contextMenu->addAction(optionOfDir);
        } else if(type == "alltags"){
            //Tag ContextMenu
            contextMenu->addAction(makeTag);
            contextMenu->addSeparator();
            contextMenu->addAction(showSubDirTag);
            contextMenu->addAction(propOfTag);
            contextMenu->addSeparator();
        }  else if(type == "tag"){
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
       }  else if(type == "doc"){
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
        }  else if(type == "basket"){


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

// 搜索设置
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

void MainWindow::windowToggleNoteEditor()
{
    // 改变NoteEditor的属性
    Preferences* p = Preferences::instance();
    QString docUuid = p->getSelDocUid();

    Doc doc = DocDao::selectDoc(docUuid);
    noteEditorDW->setWindowTitle(doc.DOCUMENT_NAME);
    noteEditorDW->setWindowIcon(Utils::getIcon("note.png"));

    QString selNoteUid = p->getSelNoteUid();
    // 如果为空则是 清空Reset
    if(selNoteUid.isEmpty()){
       noteEditor->reset();
    }else{
        QString notesPath = Utils::getLocateNotesPath();
        QString filename = notesPath.append(QDir::separator());
        filename.append(QDir::separator());
        filename.append(selNoteUid);
        filename.append(".html");
        // load noteUuid
        QFile f( filename );
        if ( !f.open( QIODevice::ReadOnly ) ){
              return;
        }
        QTextStream ts( &f );
        noteEditor->setText(ts.readAll());
        noteEditor->setFilename(filename);
    }
    windowShowNoteEditor();
}

void MainWindow::windowShowNoteEditor()
{
    noteEditor->setShowWithMain(true);
    noteEditor->show();
    noteEditorDW->show();
}

void MainWindow::windowHideNoteEditor()
{
    noteEditor->setShowWithMain(false);
    noteEditor->hide();
    noteEditorDW->hide();
}


// 以tab页打开Doc
void MainWindow::openDocInTab()
{
    QString filepath = m_doctable->getCurFilePath();

    m_tabWidget->currentLineEdit()->setText(filepath);
    m_tabWidget->newDocTab(true, filepath);
    m_tabWidget->loadDocInCurrentTab(filepath);
}




