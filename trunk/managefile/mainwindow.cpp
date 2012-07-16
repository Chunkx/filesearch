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

#include "customtooldialog.h"


#include "browser/browsermainwindow.h"
#include "browser/browserapplication.h"
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include "utils.h"
#include "fileutils.h"
#include "db/tagdao.h"
#include "db/dirdao.h"
#include "db/docdao.h"
#include "db/doctagdao.h"
#include "db/resultdao.h"
#include "noteeditor.h"
#include "aboutdialog.h"
#include "logview.h"
#include "preferencesdialog.h"
#include "indexfile.h"

extern NoteEditor *noteEditor;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    createTrayActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showMaximized()));
    trayIcon->show();

    initUI();
    initActions();
    initMenus();
    initStatusbar();
    initToolbar();

    m_appName = tr("Local File Manage");
    isBusySearch = false;
    setContextMenuPolicy(Qt::CustomContextMenu);
}

// �����ĵ��б�
void MainWindow::buildDocList()
{
    QString curUuid = q_myTreeList->getCurUuid();
    QString curType = q_myTreeList->getCurType();
    if(curType != "doc" && curType != "basket" && curType != "tag" ){
        return;
    }

    // ��ʾTag�����е��ļ�
    Preferences* p = Preferences::instance();
    bool isShowDocUnderSub = p->isShowDocUnderSub();

    // �Ƿ���ʾ���ļ����ĵ�
    QList<Doc> docs;
    // ��ʾ��������Ͱ���ļ�
    if(curType == "basket" ){
        if(isShowDocUnderSub){
              // ȡ�������ļ���
              QList<Dir> dirs ;
              DirDao::selectAllSubDirbyDir(dirs, curUuid, "1");
              // �����ļ���ȡ�������ļ�
              for(int i = 0 ; i< dirs.size(); i ++){
                 Dir dir = dirs.at(i);
                 docs.append(DocDao::selectDocsbyDir(dir.DIR_GUID, "1"));
              }
              // ȡ������ɾ�����ļ���ʱ,ƽ�е�ȡ�������ļ�
              if(dirs.size() == 0){
                  docs = DocDao::selectDocsByDelFlg("1");
              }
        }else{
             docs = DocDao::selectDocsbyDir(curUuid,  "1");
        }
        m_doctable->buildDocList(docs);
        return;
    }

    if(curType == "tag" ){
        // ����ѡ���docUuid
        if(isShowDocUnderSub){
              // ȡ�������ӱ�ǩ
              QList<Tag> tags ;
              TagDao::selectAllSubTagbyTag(tags, curUuid);
              // �����ļ���ȡ�������ļ�
              for(int i = 0 ; i< tags.size(); i ++){
                 Tag tag = tags.at(i);
                 docs.append(DocDao::selectDocsByTag(tag));
              }
        }else{
            Tag tag;
            tag.TAG_GUID = curUuid;
            docs = DocDao::selectDocsByTag(tag);
        }
        m_doctable->buildDocList(docs);
        return;
    }

    if(curType == "doc" ){
        if(isShowDocUnderSub){
              // ȡ�������ļ���
              QList<Dir> dirs ;
              // ȡ�õ�ǰĿ¼�µ��ĵ�
              docs.append(DocDao::selectDocsbyDir(curUuid, "0"));
              // �������ļ���ȡ�������ļ�
              DirDao::selectAllSubDirbyDir(dirs, curUuid, "0");
              for(int i = 0 ; i< dirs.size(); i ++){
                 Dir dir = dirs.at(i);
                 docs.append(DocDao::selectDocsbyDir(dir.DIR_GUID, "0"));
              }
        }else{
             docs = DocDao::selectDocsbyDir(curUuid,  "0");
        }
        m_doctable->buildDocList(docs);
   }
}

// ������������Menu
void MainWindow::slotShowSearchSetMenu()
{
    if(m_searchSetMenu->children().length() > 2){
       return;
    }

    Preferences* p = Preferences::instance();
    QString searcheDir = p->getSearchDir();
    QString searcheObj = p->getSearchObject();

    // ����Ĭ��
    if(searcheDir.isEmpty()){
        searcheDir == QString::number(ALLDOC);
    }
    if(searcheObj.isEmpty()){
        searcheObj == QString::number(NAME_CONTENT);
    }

    // ����Menu
    QAction *action1 = new QAction(this);
    action1->setData(ALLDOC);
    action1->setCheckable(true);
    action1->setChecked(searcheDir == QString::number(ALLDOC));
    action1->setText(tr("ALL Documents"));

    // ����Menu
    QAction *action2 = new QAction(this);
    action2->setData(CURRENTDIR);
    action2->setCheckable(true);
    action2->setChecked(searcheDir == QString::number(CURRENTDIR));
    action2->setText(tr("Current Directory"));

    // ����Menu
    QAction *action3 = new QAction(this);
    action3->setData(CURRENTDIRINCLUESUB);
    action3->setCheckable(true);
    action3->setChecked(searcheDir == QString::number(CURRENTDIRINCLUESUB));
    action3->setText(tr("Current Directory(Include Sub Directory)"));

    QActionGroup *grp = new QActionGroup( this );
    grp->setExclusive(true);
    connect(grp, SIGNAL(selected(QAction*)), this, SLOT(setSearchDir(QAction*)));
    grp->addAction(action1);
    grp->addAction(action2);
    grp->addAction(action3);

    // ����Menu
    QAction *action4 = new QAction(this);
    action4->setData(NAME_CONTENT);
    action4->setCheckable(true);
    action4->setChecked(searcheObj == QString::number(NAME_CONTENT));
    action4->setText(tr("Document Name And Content"));

    QAction *action5 = new QAction(this);
    action5->setData(DOC_NAME);
    action5->setCheckable(true);
    action5->setChecked(searcheObj == QString::number(DOC_NAME));
    action5->setText(tr("Document Name Only"));

    QAction *action6 = new QAction(this);
    action6->setData(DOC_CONTENT);
    action6->setCheckable(true);
    action6->setChecked(searcheObj == QString::number(DOC_CONTENT));
    action6->setText(tr("Document Content Only"));

    QActionGroup *grp11 = new QActionGroup(this);
    grp11->setExclusive(true);
    connect(grp11, SIGNAL(selected(QAction*)), this, SLOT(setSearchObject(QAction*)));
    grp11->addAction(action4);
    grp11->addAction(action5);
    grp11->addAction(action6);

//    QAction *action6 = new QAction(this);
//    action6->setData(WEBSEARCH);
//    action6->setCheckable(true);
//    action6->setText(tr("Web Search"));

    QAction *action7 = new QAction(this);
    action7->setData(OPTION);
    action7->setCheckable(false);
    action7->setText(tr("Option"));

//    QAction *action8 = new QAction(this);
//    action8->setData(SAVETOFASTSEARCH);
//    action8->setCheckable(true);
//    action8->setText(tr("Save To Fast Search"));

    m_searchSetMenu->addAction(action1);
    m_searchSetMenu->addAction(action2);
    m_searchSetMenu->addAction(action3);

    m_searchSetMenu->addSeparator();
    m_searchSetMenu->addAction(action4);
    m_searchSetMenu->addAction(action5);
    m_searchSetMenu->addAction(action6);
    m_searchSetMenu->addSeparator();
    m_searchSetMenu->addAction(action7);
//    m_searchSetMenu->addAction(action8);
}

// ��ʼ�����涯��
void MainWindow::initActions()
{

        // �����������
        m_searchSetAction = new QAction(Utils::getIcon("setting.ico"), tr("Search"), this);
        m_searchSetAction->setIconVisibleInMenu(false);
        connect(m_searchSetAction, SIGNAL(triggered()), this, SLOT(slotShowSearchSetMenu()));
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
        connect(saveAsAction, SIGNAL(triggered()), this, SLOT(about()));

        // ��浽�ƶ��豸
        saveToMobiAction = new QAction(Utils::getIcon("document-savetomobi.png"),tr("&Save To Mobile..."), this);
        connect(saveToMobiAction, SIGNAL(triggered()), this, SLOT(exportDlg()));

        // �˳�
        exitAction = new QAction(Utils::getIcon("close.png"),tr("&Exit"), this);
        exitAction->setShortcut(tr("Ctrl+Q"));
        connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

        // ȫ��
        fullScreenAction = new QAction(Utils::getIcon("view_fullscreen.png"),tr("&Full Screen"), this);
        connect(fullScreenAction, SIGNAL(triggered()), this, SLOT(fullScreen()));

        // ��ʾ/���ط�����
        showClassTreeAction = new QAction(Utils::getIcon("view_tree.png"),tr("&Show/Hide Class Tree"), this);
        showClassTreeAction->setShortcut(tr("Ctrl+H"));
        connect(showClassTreeAction, SIGNAL(triggered()), this, SLOT(showClassTree()));

//        // Ƥ��
//        skinAction = new QAction(Utils::getIcon("skins.png"),tr("&Skin"), this);
//        menu_skin = new QMenu(this);
//        skinAction->setMenu(menu_skin);

//        //Ƥ�� Ĭ��
//        defaultAction = new QAction(tr("&Defaut"), this);
//        connect(defaultAction, SIGNAL(triggered()), this, SLOT(about()));

//       // Ƥ�� ����
//        bookAction = new QAction(tr("&Book"), this);
//        connect(bookAction, SIGNAL(triggered()), this, SLOT(about()));

//        //Ƥ�� ��ɫ
//        grayAction = new QAction(tr("&Gray"), this);
//        connect(grayAction, SIGNAL(triggered()), this, SLOT(about()));

        // �Զ��幤����
        customToolbarAction = new QAction(Utils::getIcon("toolbar.png"),tr("&Setting ToolBar"), this);
        connect(customToolbarAction, SIGNAL(triggered()), this, SLOT(customToolBar()));

        // ��ʾ/���ع�����
        showToolbarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Tool Bar"), this);
        connect(showToolbarAction, SIGNAL(triggered()), this, SLOT(showToolBar()));

//        // ��ʾ/����״̬��
//        showSatebarAction = new QAction(Utils::getIcon("status_bar.png"),tr("&Show/Hide Status Bar..."), this);
//        connect(showSatebarAction, SIGNAL(triggered()), this, SLOT(about()));

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
        connect(checkNewAction, SIGNAL(triggered()), this, SLOT(checkNewVersion()));

        // ѡ��
        optionAction = new QAction(Utils::getIcon("option.png"),tr("&Option"), this);
        optionAction->setShortcut(tr("Ctrl+O"));
        connect(optionAction, SIGNAL(triggered()), this, SLOT(option()));

        // �鿴log
        viewLogAction = new QAction(Utils::getIcon("log.png"),tr("&View Log..."), this);
        viewLogAction->setShortcut(tr("Ctrl+L"));
        connect(viewLogAction, SIGNAL(triggered()), this, SLOT(viewLog()));

//        // �������
//        pluginAction = new QAction(Utils::getIcon("plugin.png"),tr("Plugins"), this);
//        pluginAction->setShortcut(tr("Ctrl+P"));
//        connect(pluginAction, SIGNAL(triggered()), this, SLOT(about()));;

        // �˻���Ϣ
        accountInfoAction = new QAction(Utils::getIcon("my_account.ico"),tr("Account Information"), this);
        connect(accountInfoAction, SIGNAL(triggered()), this, SLOT(showAccountInfo()));;

        // ������VIP
        update2VipAction = new QAction(Utils::getIcon("vip.png"),tr("Update to VIP"), this);
        connect(update2VipAction, SIGNAL(triggered()), this, SLOT(updateToVip()));;

        // ע��
        logoffAction = new QAction(Utils::getIcon("logo_off.png"),tr("Logo Off"), this);
        connect(logoffAction, SIGNAL(triggered()), this, SLOT(logoff()));;

//        // ����
//        languageAction = new QAction(Utils::getIcon("language.png"),tr("&Language"), this);
//        menu_language = new QMenu(this);
//        languageAction->setMenu(menu_language);

//        // Ӣ�İ�
//        enAction = new QAction(tr("&English"), this);
//        connect(enAction, SIGNAL(triggered()), this, SLOT(about()));

//        // ���İ�
//        cnAction = new QAction(tr("&Chinese"), this);
//        connect(cnAction, SIGNAL(triggered()), this, SLOT(about()));

//        // ���İ�
//        jpAction = new QAction(tr("&Japanese"), this);
//        connect(jpAction, SIGNAL(triggered()), this, SLOT(about()));


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

//        menu_View->addAction(skinAction);
//        menu_skin->addAction(defaultAction);
//        menu_skin->addAction(bookAction);
//        menu_skin->addAction(grayAction);
//        menu_View->addAction(menu_skin->menuAction());

//        menu_View->addAction(languageAction);
//        menu_language->addAction(cnAction);
//        menu_language->addAction(enAction);
//        menu_language->addAction(jpAction);
//        menu_View->addAction(menu_language->menuAction());

//        menu_View->addSeparator();
//        menu_View->addAction(showSatebarAction);
        menu_View->addSeparator();
        menu_View->addAction(customToolbarAction);

        menu_Tool =  menuBar()->addMenu(tr("&Tool"));
        menu_Tool->addAction(optionAction);
        menu_Tool->addSeparator();
        menu_Tool->addAction(viewLogAction);
        menu_Tool->addSeparator();
//        menu_Tool->addAction(pluginAction);

        menu_Account =  menuBar()->addMenu(tr("&Account"));
        menu_Account->addAction(accountInfoAction);
        menu_Account->addSeparator();
        menu_Account->addAction(update2VipAction);
        menu_Account->addSeparator();
        menu_Account->addAction(logoffAction);
        menu_Account->addSeparator();


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
}

// ״̬��
void MainWindow::initStatusbar()
{
  // QLabel* m_sqliteVersionLabel = new QLabel(this);
  //statusBar()->addPermanentWidget(&m_sqliteVersionLabel);
}

// ������
void MainWindow::initToolbar()
{
    // ��ȡpreferences
    Preferences* p = Preferences::instance();
    QStringList waitItems = p->getWaitToolbarItemList();
    QStringList selItems = p->getSelToolbarItemList();

    toolBar = addToolBar(tr("Tool Bar"));
    if(selItems.length() != 0){
        upateToolBar(waitItems, selItems);
    }else{
        // Ĭ��״̬��
        toolBar->addAction(showClassTreeAction);
        toolBar->addSeparator();
        toolBar->addAction(homepageAction);
        toolBar->addAction(inviteAction);
        toolBar->addAction(bbsAction);
        toolBar->addSeparator();
    }

    // �հ�����
    QWidget *twidget = new QWidget(this);
    twidget->setMinimumWidth(this->geometry().width() * 0.1);
    toolBar->addWidget(twidget);

    m_toolbarSearch = new ToolbarSearch();
    connect(m_toolbarSearch, SIGNAL(search(QUrl)), this, SLOT(openUrl(QUrl)));
    connect(m_toolbarSearch, SIGNAL(textChanged(QString)), this, SLOT(dosearch(QString)));

    toolBar->addWidget(m_toolbarSearch);
    toolBar->addAction(m_searchSetAction);
    toolBar->addSeparator();
    toolBar->addAction(fullScreenAction);

    // �հ�����
    sapcewidget = new QWidget(this);
    sapcewidget->setMinimumWidth(m_doctable->width()*1.2);
    toolBar->addWidget(sapcewidget);
}

// �����ؼ���
void MainWindow::dosearch(QString keyWord)
{

    resultlist.clear();
    // �����˳�
    if(keyWord.isEmpty()){
       m_doctable->buildSearchResult(resultlist);
       return;
    }

    Preferences* p = Preferences::instance();
    searcheObj = p->getSearchObject();

    bool isSetObj = (objdirList.size() != 0);
    // �ļ���
    if(searcheObj == QString::number(DOC_NAME) || searcheObj == QString::number(NAME_CONTENT) ){
        // ��������ǰ��
        QList<Doc> docs= DocDao::selectDocsByName(keyWord);
         // ��ת����Result
         // �����ļ���ȡ�������ļ�
        for(int i = 0 ; i< docs.size(); i ++){
             Doc doc = docs.at(i);
             QString dirUuid = doc.DIR_GUID;
             if(isSetObj){
                 // �������������һ��
                 if(!objdirList.contains(dirUuid)){
                     continue;
                 }
             }

             Result rst;
             rst.DOC_UUID = doc.DOCUMENT_GUID;
             rst.KEY_WORD = keyWord;
             rst.FILE_PATH = doc.DOCUMENT_LOCATION;
             rst.FILE_NAME = doc.DOCUMENT_NAME;
             rst.DESP = "";
             rst.CONTENT = doc.DOCUMENT_NAME;
             rst.SHEET_NAME = "";
             rst.DT_CREATED = doc.DT_CREATED;
             resultlist.push_back(rst);
        }
    }

    // �ļ�����
    if(searcheObj == QString::number(DOC_CONTENT) || searcheObj == QString::number(NAME_CONTENT) ){
        // �������ݿ�
        ResultDao dao;
        QList<Result> dbList = dao.selectByFullEqual(keyWord);
        // ��������
        // �������ں���
        Preferences* p = Preferences::instance();
        bool isIndexing = p->getIsIndexing();

        if(dbList.size() == 0 && !isBusySearch && !isIndexing){
            isBusySearch = true;
            m_keyWord = keyWord;
            QueryIndexFilesObj queryIndexFilesObj;
            queryIndexFilesObj.keyWord = keyWord;
            queryIndexFilesObj.searchType = "all";
            QueryIIndexFilesSign dummy;

            QObject::connect(&dummy, SIGNAL(sig()), &queryIndexFilesObj, SLOT(queryfiles()));
            QObject::connect(&queryIndexFilesObj, SIGNAL(finished()), this, SLOT(nextSearchCanStart()));
            dummy.emitsig();
        }else{
           if(isSetObj){
                // ��·���ų�
                for(int i = 0 ; i< dbList.size(); i ++){
                    Result result = dbList.at(i);
                    Doc doc = DocDao::selectDoc(result.DOC_UUID);
                    // ���������
                    if(objdirList.contains(doc.DIR_GUID)){
                       resultlist.append(result);
                    }
                }
            } else {
                resultlist.append(dbList);
            }
        }
    }

     m_doctable->buildSearchResult(resultlist);
}

// ������
void MainWindow::nextSearchCanStart()
{
    ResultDao dao;

    QList<Result> dbList = dao.selectByFullEqual(m_keyWord);
    bool isSetObj = (objdirList.size() != 0);
    if(isSetObj){
         // ��·���ų�
         for(int i = 0 ; i< dbList.size(); i ++){
             Result result = dbList.at(i);
             Doc doc = DocDao::selectDoc(result.DOC_UUID);
             // ���������
             if(objdirList.contains(doc.DIR_GUID)){
                resultlist.append(result);
             }
         }
     }

    m_doctable->buildSearchResult(resultlist);
    isBusySearch = false;
}

// ������
void MainWindow::openUrl(QUrl url)
{
    browser->loadUrl(url);
}

// �û��ֲ�
void MainWindow::helpDoc()
{
    browser->loadPage("http://www.slfile.net/?page_id=58");
}

// ��̳
void MainWindow::bbs()
{
    browser->loadPage("http://www.slfile.net/?post_type=forum");
}

// ѡ��
void MainWindow::option()
{
    // curPath �����жϸ��ڵ���ӽڵ�
    PreferencesDialog dlg(this);
    dlg.exec();
}

// ��ҳ
void MainWindow::homepage()
{
    browser->loadPage("http://www.slfile.net/");
}

// �鿴Log�ļ�
void MainWindow::viewLog()
{
    LogView dlg(this);

    QString logpath = Utils::getSaveLogPath();
    logpath.append(QDir::separator()).append("mflog.log");
    dlg.open(logpath);

    dlg.exec();
}

void MainWindow::initUI()
{
        setWindowTitle(m_appName);

        splitter = new QSplitter(this);
        // ���ط�����
        m_baseDir = Utils::getLocatePath();
        m_baseDir = QDir::toNativeSeparators(m_baseDir);
        q_myTreeList = new MyTreeView("all", this);

        // �յ�UuId
        q_myTreeList->enableMouse(true);

        m_doctable = new MyTableView(this);

        splitter->addWidget(q_myTreeList);
        splitter->addWidget(m_doctable);

        BrowserApplication* applcation = BrowserApplication::instance();
        browser = applcation->newMainWindow(splitter);

        splitter->addWidget(browser);
        browser->hide();
        browser->setMaximumWidth(0);
        browser->setMinimumWidth(0);


        connect(browser, SIGNAL(exitFullScreen()), this, SLOT(fullScreen()));
        connect(q_myTreeList, SIGNAL(LBtnClk()), this, SLOT(buildDocList()));


        // ������Ҫ����������Ķ���
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
            // ��Note�༭�����Note
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
    Preferences::deleteInstance();
}

void MainWindow::closeEvent(QCloseEvent * e)
{
     Preferences::deleteInstance();
}

// �Զ��幤����
void MainWindow::customToolBar()
{
    CustomToolDialog dlg(this);
    dlg.exec();
    if(dlg.update){
        // ���¹�����
        Preferences* p = Preferences::instance();
        QStringList waitItems = p->getWaitToolbarItemList();
        QStringList selItems = p->getSelToolbarItemList();

        upateToolBar(waitItems, selItems);
    }
}
// ���¹�����
void MainWindow::upateToolBar(QStringList waitItems, QStringList selItems)
{

    QList<QAction*> actions = toolBar->actions();

    // ������
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
    // ȫ��
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
    // ������ҳ
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
    // ��������
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
    // ��̳
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
    // ��浽�ƶ��豸
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
    //  ����
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
    //  ����
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
    // �������
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

    AboutDialog aboutdlg(this);
    aboutdlg.exec();
//    QMessageBox::about(this, tr("About %1").arg(windowTitle()),
//                       tr("Sorry, The function is under construction..."));
}

void MainWindow::checkNewVersion()
{
    // ������������
    qDebug() << "start update programme";
    QProcess::startDetached(QDir::currentPath().append("updater.exe"), QStringList());
    // �˳�������
    QCoreApplication::exit(0);
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

    if(toolBar->isHidden()){
       toolBar->show();
    }else{
       toolBar->hide();
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
// �򿪵������
void MainWindow::importDlg()
{
    q_myTreeList->showImportDlg();
}

// �򿪵�������
void MainWindow::exportDlg()
{
   q_myTreeList->showExportDlg();
}

// ��ʾ�˻���Ϣ
void MainWindow::showAccountInfo(){
    accoutdlg = new AccountDialog(this);
    connect(accoutdlg, SIGNAL(updateVip()), this, SLOT(updateToVip()));;
    accoutdlg->exec();
}
// ������VIP
void MainWindow::updateToVip(){
  // ��������VIP�˺�
  browser->loadPage("http://www.slfile.net/?page_id=115");
}
// ע��
void MainWindow::logoff(){
    // �ı�NoteEditor������
    Preferences* p = Preferences::instance();
    p->setUserName("");
    p->setUserEmail("");
    p->setDisplayName("");
    p->setScore("");
    p->setDtExpired("");
    p->setType("");
    p->setLastOpenDocs("");
    // ��������
    QCoreApplication::exit(773);
}

// ��������Ŀ¼
void MainWindow::setSearchDir(QAction *action){
    int offset = action->data().toInt();
    Preferences* p = Preferences::instance();
    QString dir = QString::number(offset);
    p->setSearchDir(dir);

    // ���ü�������Ŀ¼
    objdirList.clear();
    if(dir == QString::number(CURRENTDIR)){
        QString diruuid = q_myTreeList->getCurUuid();
        QString curtype = q_myTreeList->getCurType();
        if(curtype == "doc" ){
           objdirList.append(diruuid);
        }
    }
    if(dir == QString::number(CURRENTDIRINCLUESUB)){
        QString diruuid = q_myTreeList->getCurUuid();
        QString curtype = q_myTreeList->getCurType();
        if(curtype == "doc" ){
            QList<Dir> dirList;
            DirDao::selectAllSubDirbyDir(dirList, diruuid, "0");
            DirDao::selectAllSubDirbyDir(dirList, diruuid, "1");
            for(int i = 0 ; i< dirList.size(); i ++){
                Dir dir = dirList.at(i);
                objdirList.append(dir.DIR_GUID);
            }
        }
    }
}

// ������������
void MainWindow::setSearchObject(QAction *action){
    int offset = action->data().toInt();
    searcheObj = "";
    Preferences* p = Preferences::instance();
    p->setSearchObject(QString::number(offset));
    searcheObj = QString::number(offset);
}

// ��������
void MainWindow::slotOpenActionUrl(QAction *action)
{
    int offset = action->data().toInt();
    switch (offset) {
        case ALLDOC:

        case CURRENTDIR:

        case CURRENTDIRINCLUESUB:

        case NAME_CONTENT:

        case DOC_NAME:

        case DOC_CONTENT:

        case OPTION:

        case SAVETOFASTSEARCH:
          ;
    }
 }

void MainWindow::windowToggleNoteEditor()
{
    // �ı�NoteEditor������
    Preferences* p = Preferences::instance();
    QString docUuid = p->getSelDocUid();

    Doc doc = DocDao::selectDoc(docUuid);
    noteEditorDW->setWindowTitle(doc.DOCUMENT_NAME);
    noteEditorDW->setWindowIcon(Utils::getIcon("note.png"));

    QString selNoteUid = p->getSelNoteUid();
    // ���Ϊ������ ���Reset
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

    m_doctable->hide();
    q_myTreeList->hide();
}

void MainWindow::windowHideNoteEditor()
{
    noteEditor->setShowWithMain(false);
    noteEditor->hide();
    noteEditorDW->hide();

    m_doctable->show();
    q_myTreeList->show();
}


//void MainWindow::geometryChangeRequested(const QRect &geometry)
//{
//    setGeometry(geometry);
//}
// ��tabҳ��Doc
void MainWindow::openDocInTab()
{
    QString filepath = m_doctable->getCurFilePath();

    // �ı�NoteEditor������
    Preferences* p = Preferences::instance();
    QString suffix = FileUtils::suffix(filepath);
    if(p->sources().contains(suffix.toLower())){
        browser->openTxtInTab(filepath);
    } if(p->htmls().contains(suffix.toLower())){
        browser->loadPage(filepath);
    }if(p->officedoc().contains(suffix.toLower())){
        browser->openDocInTab(filepath);
    }
}

// ϵͳ���̴���
void MainWindow::createTrayActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(Utils::getIcon("file_manager.png"), this);
    trayIcon->setContextMenu(trayIconMenu);
}
void MainWindow::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}
void MainWindow::quit()
{
    // �˳�������
    QCoreApplication::exit(0);
}
