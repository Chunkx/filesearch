#include<QPainter>
#include<QMouseEvent>
#include<QStandardItem>
#include<QHeaderView>
#include<QDir>
#include<QAction>
#include<QMenu>
#include<QMessageBox>
#include<QDesktopServices>
#include<QProcess>
#include<QDebug>
#include <QToolTip>

#include"mytableview.h"
#include"fileUtils.h"
#include "preferences.h"
#include "utils.h"
#include "tableresultview.h"
#include "exportconvertdialog.h"
#include "doctodirdialog.h"
#include "relatedocdialog.h"
#include "notesdialog.h"
#include "doctagsdialog.h"
#include "propofdocdialog.h"
#include "Common.h"
#include "mytreeitemmodel.h"
#include "db/docdao.h"
#include "db/doctagdao.h"
#include "db/notedao.h"
#include "db/dirdao.h"
#include "db/resultdao.h"
#include "printerwidget.h"
#include "QSettings"

MyTableView::MyTableView(QWidget * parent) : QTableView(parent), mouseStatus(true)
{
    delegate = new MyTableDelegate(this);
    model = new MyTableItemModel(parent);

    this->setItemDelegate(delegate);

    horizontalHeader()->setMouseTracking (true);

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setMouseTracking(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setAcceptDrops(true);
    this->setShowGrid(false);

    // �������õ�һ�� ѡ������ؼ�
    // this->setItemDelegateForRow();

//    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
   // this->setAutoFillBackground(true);
//    this->setAlternatingRowColors(true);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);


//    this->setObjectName(QString::fromUtf8("m_table"));
//    this->setAutoFillBackground(true);
//    this->setFrameShape(QFrame::NoFrame);
//    this->setFrameShadow(QFrame::Plain);
//    this->setLineWidth(0);
//    this->setEditTriggers(QAbstractItemView::AllEditTriggers);

//    this->setAlternatingRowColors(true);
//    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
//    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
//     this->setGridStyle(Qt::SolidLine);

//    this->horizontalHeader()->setVisible(true);
//    this->verticalHeader()->setVisible(false);

    connect(this, SIGNAL(entered(QModelIndex)),this, SLOT(showToolTip(QModelIndex)));

//    QDesktopServices::setUrlHandler( "mailto", this, "mailTo" );

    initActions();
    model->setColumnCount(3);
    this->setModel(model);

}

// Action ����
void MyTableView::initActions ()
{
    // ϵͳĬ�ϴ�
    openInSysAction = new QAction(tr("&Open in System soft"), this);
    connect(openInSysAction, SIGNAL(triggered()), this, SLOT(openInSys()));

    // ����tabҳ�д�
    openInTabAction = new QAction(tr("&Open in tab page"), this);
    connect(openInTabAction, SIGNAL(triggered()), this, SLOT(openInTab()));

    // ����Դ��������
    openInSysExploreAction = new QAction(tr("&Open in System Explorer"), this);
    connect(openInSysExploreAction, SIGNAL(triggered()), this, SLOT(openInSysExplore()));

    // ����
    exportDocAction = new QAction(tr("&Export.."), this);
    connect(exportDocAction, SIGNAL(triggered()), this, SLOT(exportConvert()));

    // �Ը�����ʽ�����ʼ�
    emailAsAttachAction = new QAction(tr("&Send Email As Attachment"), this);
    connect(emailAsAttachAction, SIGNAL(triggered()), this, SLOT(sendMail()));

    // �ĵ��ʼ�
    noteOfDocAction = new QAction(tr("&Notes"), this);
    connect(noteOfDocAction, SIGNAL(triggered()), this, SLOT(notes()));

    // �����ĵ�
    relatedDocAction = new QAction(tr("&Related Documents"), this);
    connect(relatedDocAction, SIGNAL(triggered()), this, SLOT(relateDocs()));

    // ��ǩ
    tagAction = new QAction(tr("&Tags"), this);
    connect(tagAction, SIGNAL(triggered()), this, SLOT(docTags()));

    // ɾ��
    deleteAction = new QAction(tr("&Delete(Move to the Wastebasket)"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(delDoc()));

    // ��ԭ
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreDoc()));

    // ����
    encodeAction = new QAction(Utils::getIcon("vip.png"), tr("&Encrypt"), this);
    connect(encodeAction, SIGNAL(triggered()), this, SLOT(about()));

    // �����滻
    findAndReplaceAction = new QAction(Utils::getIcon("vip.png"), tr("&Find and Replace"), this);
    connect(findAndReplaceAction, SIGNAL(triggered()), this, SLOT(about()));

    // ȫѡ
    selectAllAction = new QAction(tr("&Select All"), this);
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAllDoc()));

    // ��ӡ
    printAction = new QAction(tr("&Print"), this);
    connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

    // �߼�
   advancedAction = new QAction(Utils::getIcon("vip.png"),tr("&Advanced"), this);
   QMenu *menu_ad = new QMenu(this);
   advancedAction->setMenu(menu_ad);
   // ת���ĵ�
   convertDocAction = new QAction(tr("&Convert Document"), this);
   connect(convertDocAction, SIGNAL(triggered()), this, SLOT(about()));
   QMenu *menu_covert = new QMenu(this);
   convertDocAction->setMenu(menu_covert);
   office2pdfAction = new QAction(tr("&Convert Office to Pdf"), this);
   pdf2officeAction = new QAction(tr("&Convert Pdf to Office"), this);
   jpg2pdfAction = new QAction(tr("&Convert Picture to Pdf"), this);
   ppt2jpgAction = new QAction(tr("&Convert Ppt to Picture"), this);
   menu_covert->addAction(office2pdfAction);
   menu_covert->addAction(pdf2officeAction);
   menu_covert->addAction(jpg2pdfAction);
   menu_covert->addAction(ppt2jpgAction);

   // �ϲ��ĵ�
   combineAction = new QAction(tr("&Merge Document"), this);
   connect(combineAction, SIGNAL(triggered()), this, SLOT(about()));
   // �޸��ĵ�
   repairAction = new QAction(tr("&Restore And Repair Document"), this);
   connect(repairAction, SIGNAL(triggered()), this, SLOT(about()));
   // ������ӡ�ĵ�
   pprintAction = new QAction(tr("&Print Documents In Batch"), this);
   connect(pprintAction, SIGNAL(triggered()), this, SLOT(about()));
   // �ı��ĵ���ʽ
   pformatChangeAction = new QAction(tr("&Change format in the Document"), this);
   connect(pformatChangeAction, SIGNAL(triggered()), this, SLOT(about()));
   menu_ad->addAction(convertDocAction);
   menu_ad->addAction(combineAction);
   menu_ad->addAction(repairAction);
   menu_ad->addAction(pprintAction);
   menu_ad->addAction(pformatChangeAction);

   Preferences* p = Preferences::instance();
   // �ƶ����ļ���
   moveToDirAction = new QAction(tr("&Move to..."), this);
   connect(moveToDirAction, SIGNAL(triggered()), this, SLOT(moveToDir()));
   // ���Ƶ��ļ���
   copyToDirAction = new QAction(tr("&Copy to..."), this);
   connect(copyToDirAction, SIGNAL(triggered()), this, SLOT(copyToDir()));
   // ѡ�� <ѡ���һ�� �Լ��ڶ��е�����>
   optionOfDocTableAction = new QAction(tr("&Option"), this);
   connect(optionOfDocTableAction, SIGNAL(triggered()), this, SLOT(about()));
   option_submenu = new QMenu(this);
   option_submenu->setStyleSheet(
               "QMenu::item{height: 25px}"
               "QMenu::item:hover{background-color:rgb(100,100,100)}"
               "QMenu::item:selected{background-color:rgb(128,128,128)}"
   );
   optionOfDocTableAction->setMenu(option_submenu);
   QString selModel = p->getTableSelMode();
   oneRowAction = new QAction(tr("&One Row"), this);
   oneRowAction->setData(ONE_ROW);
   oneRowAction->setChecked(selModel.toInt() == ONE_ROW);
   oneRowAction->setCheckable(true);
   twoRowAction = new QAction(tr("&Two Rows"), this);
   twoRowAction->setData(TWO_ROWS);
   twoRowAction->setCheckable(true);
   twoRowAction->setChecked(selModel.toInt() == TWO_ROWS);
   twoRowOptionAction = new QAction(tr("&The second Row's Option"), this);
   showNotesAction = new QAction(tr("&Show notes of Document on Tooltip"), this);
   showNotesAction->setCheckable(true);
   showNotesAction->setChecked(p->isShowNotesOnTips());
   connect(showNotesAction, SIGNAL(triggered()), this, SLOT(setShowNotesTips()));

   option_submenu->addAction(oneRowAction);
   option_submenu->addAction(twoRowAction);
   option_submenu->addSeparator();
   option_submenu->addAction(twoRowOptionAction);
   option_submenu->addSeparator();
   option_submenu->addAction(showNotesAction);
   connect(option_submenu, SIGNAL(triggered(QAction*)), this, SLOT(slotShowTableOption(QAction*)));
   // �趨�ڶ��е�����Menu
   secondRowSetMenu();
   twoRowOptionAction->setMenu(m_secondRowSetMenu);
   connect(m_secondRowSetMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotShowSecondRowContent(QAction*)));

   // ����
   propAction = new QAction(tr("&Properties"), this);
   connect(propAction, SIGNAL(triggered()), this, SLOT(propOfDoc()));

   // �Ҽ�
   cmenu = new QMenu(this);
   cmenu->setStyleSheet(
               "QMenu::item{height: 25px}"
               "QMenu::item:hover{background-color:rgb(100,100,100)}"
               "QMenu::item:selected{background-color:rgb(128,128,128)}"
   );
   cmenu->hide();

   // �Ҽ���˵�
   cmenu->addAction(openInSysAction);
   cmenu->addAction(openInTabAction);
   cmenu->addAction(openInSysExploreAction);
   cmenu->addSeparator();
   cmenu->addAction(exportDocAction);
   cmenu->addAction(emailAsAttachAction);
   cmenu->addSeparator();
   cmenu->addAction(noteOfDocAction);
   cmenu->addAction(relatedDocAction);
   cmenu->addAction(tagAction);
   cmenu->addSeparator();
   cmenu->addAction(encodeAction);
   cmenu->addAction(findAndReplaceAction);
   cmenu->addAction(selectAllAction);
   cmenu->addAction(printAction);

   cmenu->addSeparator();
   cmenu->addAction(advancedAction);
   cmenu->addSeparator();

   cmenu->addAction(moveToDirAction);
   cmenu->addAction(copyToDirAction);
   cmenu->addAction(optionOfDocTableAction);
   cmenu->addAction(propAction);
}

// ���ڴ�С���ڸ��еĳ���
void MyTableView::resizeEvent(QResizeEvent * event){
    int tablewidth = this->width();
    this->setColumnWidth(0, tablewidth);
}

void MyTableView::buildDocList(QList<Doc> doclist)
{
    Preferences* p = Preferences::instance();
    model->clear();
    qDebug("buildDocList start");

    // ȡ��ѡ��ģʽ
    QString selModel = p->getTableSelMode();
    int intselModel = selModel.toInt();

    for (int var = 0; var < doclist.size(); ++var) {
         Doc doc = doclist.at(var);

         Document document = Utils::convertDoc(doc);
         QStandardItem* item = new QStandardItem();
         item->setData(QVariant::fromValue(document), Qt::DisplayRole);

         model->appendRow(item);

         if(intselModel == ONE_ROW){
            this->setRowHeight(var, 25);
         } else {
            this->setRowHeight(var, 65);
         }
    }
    // ���ø��б���,ʹ��ռ��ȫ��
    int tablewidth = this->width();
    this->setColumnWidth(0, tablewidth);

    qDebug("buildDocList end");
}

bool sortResult(const  Result &p1, const Result &p2)
{
    return p1.FILE_NAME <  p2.FILE_NAME;
}

void MyTableView::buildSearchResult(QList<Result> resultlist)
{
    model->clear();
    qDebug("buildResultList start");

    qSort(resultlist.begin(), resultlist.end(), sortResult);
    QString letter = "";
    for(int i = 0; i < resultlist.size(); i ++)
    {
        Result result = resultlist.at(i);

        TableResult tableResult = Utils::convertResult(result);

        if(letter != tableResult.filename())
        {
            letter = tableResult.filename();
            QStandardItem *item = new QStandardItem(letter);
            model->appendRow(item);
        }

        QStandardItem *item = new QStandardItem();
        item->setData(QVariant::fromValue(tableResult), Qt::DisplayRole);
        model->appendRow(item);
    }

    // ���ø��б���,ʹ��ռ��ȫ��
    int tablewidth = this->width();
    this->setColumnWidth(0, tablewidth);

    qDebug("buildResultList end");
}

// Show tooltip
void MyTableView::showToolTip(const QModelIndex &index)
{
    if (!index.isValid()) {
       return;
    }
    Preferences* p = Preferences::instance();
    bool isShowNotes = p->isShowNotesOnTips();
    if(isShowNotes){
       setCurUuidByIndex(index);
       if(! curUuid.isEmpty()){
            QString tips;
            QList<Note> list = NoteDao::selectNotesbyDocUuId(curUuid);
            for (int var = 0; var < list.size(); ++var) {
                Note note = list.at(var);
                QString notes = note.NOTE_CONTENT.trimmed();
                tips.append(notes.length() > 100 ? notes.left(100) : notes + "\n");
            }
            QToolTip::showText(QCursor::pos(), tips);
       }
    }
}

// ���˫��
void MyTableView::mouseDoubleClickEvent(QMouseEvent *event)
{

        if(true == mouseStatus && Qt::LeftButton == event->button())
        {
                curPoint = event->pos();
                QModelIndex  index = indexAt(curPoint);
                if(index.isValid()){

                    setCurUuidByIndex(index);
                    curItem = model->itemFromIndex(index);

                    // ����ѡ�е��ļ����� �����ļ�������ʾ
                    Preferences* p = Preferences::instance();
                    p->setOpenDoc(curPath);

                    emit LBtnDbClk();
                }

        }
}
// �Ҽ�����
void MyTableView::mousePressEvent(QMouseEvent *event)
{

        curPoint = event->pos();
        QModelIndex  index = indexAt(curPoint);

        if(index.isValid()){
            setCurUuidByIndex(index);
            curItem = model->itemFromIndex(index);
            // ����˳�
            if( Qt::LeftButton == event->button()){
                return;
            }
            // �Ҽ���ʾ�˵�
            if(true == mouseStatus )
            {
                tableContextMenuOpened();
            }
        }
}

QPoint MyTableView::getCurPoint(){
    return curPoint;
}

QString MyTableView::getCurUuid(){
    return curUuid;
}

QString MyTableView::getCurFilePath(){
    return curPath;
}

void MyTableView::enableMouse(bool yesOrNo)
{
    mouseStatus = yesOrNo;
}

bool MyTableView::getMouseStatus()
{
    return mouseStatus;
}
// ��ϵͳ���ߴ�
void MyTableView::openInSys()
{
    QFileInfo fileInfo(curPath);
    if(!fileInfo.exists()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Confirm The original file  has Deleted Or Moved. "), QMessageBox::Yes);
        return;
    }
    QDesktopServices::openUrl ( QUrl::fromLocalFile(curPath) );
}
// ������Tabҳ��
void MyTableView::openInTab()
{
    // �൱��˫��
    emit LBtnDbClk();
}

// ����Դ�������д�
void MyTableView::openInSysExplore()
{
    // ����Ҫ �ж��ļ��Ƿ����
    QFileInfo fileInfo(curPath);
    if(!fileInfo.exists()){
        QMessageBox::warning(this, tr("Warning"), tr("Please Confirm The original file  has Deleted Or Moved. "), QMessageBox::Yes);
    }

    QString path = fileInfo.absolutePath();
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

// ��ʾ����ת���ĵ�����
void MyTableView::exportConvert()
{
    bool hasSelRight = false;
    // ��ѡ���ĵ�
    if(!curPath.isEmpty()) {
        hasSelRight = true;
        ExportConvertDialog dlg(this, curPath);
        dlg.exec();
        if(dlg.update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(0, tr("Warning"), tr("Please Select an Document."), QMessageBox::Yes);
        return;
    }
}

// ��ʾ����ת���ĵ�����
void MyTableView::sendMail()
{

    QFileInfo fileInfo(curPath);
    if(!fileInfo.exists()){
        QMessageBox::critical(this, tr("Error"), tr("Please Confirm The original file  has Deleted Or Moved. "), QMessageBox::Yes);
        return;
    }

//    Common* com = new Common();
//    QString mailcontent = "mailto:";
//    mailcontent.append("test@tom.com");
//    mailcontent.append("&subject=");
//    mailcontent.append(fileInfo.fileName());
//    mailcontent.append("&body=");
//    mailcontent.append(fileInfo.fileName() + tr("\n\nReview this document please."));
//    mailcontent.append("&attachment=");
//    mailcontent.append(curPath);

    // com->mailTo(QUrl(mailcontent, QUrl::TolerantMode));
    // QProcess::execute("7z", QStringList() << "e" << "C:\\docxTest\\1\\1.zip"  << "-oC:\\docxTest\\1\\unzip\\");

//    QString mail = QSettings( "HKEY_CURRENT_USER\\Software\\Clients\\Mail",
//            QSettings::NativeFormat ).value( "." ).toString();

//    QProcess* foo= new QProcess( this );
//    foo->setProcessChannelMode(QProcess::MergedChannels);
//    foo->setEnvironment( QProcess::systemEnvironment() );
//    foo->start( "outlook" );

//    if( !foo->waitForFinished() ) {
//        qDebug() << QDir( foo->workingDirectory() ).entryList();
//        qDebug() << "Fail:" << foo->errorString();
//        qDebug() << "Exit = " << foo->exitCode();
//    } else {
//        qDebug() << "Output:" << foo->readAll();
//    }


    QString regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Office\\";
    QStringList officeverList;
    officeverList << "14.0";
    officeverList << "12.0";
    officeverList << "11.0";
    officeverList << "10.0";
    officeverList << "9.0";
    officeverList << "8.0";
    QString outLookPath;
    for (int var = 0; var < officeverList.length(); ++var) {
        QString version = officeverList.at(var);
        QString temp = regPath + version + "\\Outlook\\InstallRoot\\";
        QSettings *settings = new QSettings(temp, QSettings::NativeFormat);
        outLookPath = settings->value("Path", "").toString();
        QFileInfo outPath(outLookPath);
        if(outPath.exists()){
            break;
        }
    }

    QString program = outLookPath + "\\OUTLOOK.EXE";
    QStringList arguments;
    arguments.append("/a");
    arguments.append(curPath);
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);


    // ȥ�����з���
//    >     QString s = "{a fishy string?}";
//    >     QByteArray ba = QUrl::toPercentEncoding(s, QByteArray(),
//    > s.toLatin1());
//    >     qDebug() << ba;

}

// ��ʾ���ĵ��ʼ�
void MyTableView::notes()
{
    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    p->setSelDocUid(curUuid);

    m_notesdlg = new NotesDialog(this);
    connect(m_notesdlg, SIGNAL(showAddNoteWidget()), this, SLOT(showMainAddNoteWidget()));
    connect(m_notesdlg, SIGNAL(closeNoteWidget()), this, SLOT(hideMainAddNoteWidget()));

    bool hasSelRight = false;
    // ��ѡ���ĵ�
    if(!curUuid.isEmpty()) {
        hasSelRight = true;
        m_notesdlg->exec();
        if(m_notesdlg->update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(0, tr("Warning"), tr("Please Select an Document."), QMessageBox::Yes);
        return;
    }
}

// ��ʾNotes
void MyTableView::showNoteDialog()
{
    notes();
}

// ��ʾ���ĵ��ʼ�
void MyTableView::showMainAddNoteWidget()
{
    emit showAddNoteWidget();
}

// �����ĵ��ʼ�
void MyTableView::hideMainAddNoteWidget()
{
    if(m_notesdlg->needCloseNoteWiget){
          emit hideNoteWidget();
    }
}

// ��ʾ�������ĵ�
void MyTableView::relateDocs()
{
    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    p->setSelDocUid(curUuid);

    m_relatedocdlg = new RelateDocDialog(this);

    bool hasSelRight = false;
    // ��ѡ���ĵ�
    if(!curPath.isEmpty()) {
        hasSelRight = true;
        m_relatedocdlg->exec();
        if(m_relatedocdlg->update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(0, tr("Warning"), tr("Please Select an Document."), QMessageBox::Yes);
        return;
    }
}

// ��ʾ���ĵ�Tag
void MyTableView::docTags()
{
    // ����ѡ���docUuid
    m_doctagsdlg = new DocTagsDialog(this, curUuid);

    connect(m_doctagsdlg, SIGNAL(reloadTagTree()), this, SLOT(reloadMainTagTree()));

    bool hasSelRight = false;
    // ��ѡ���ĵ�
    if(!curUuid.isEmpty()) {
        hasSelRight = true;
        m_doctagsdlg->exec();
        if(m_doctagsdlg->update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(0, tr("Warning"), tr("Please Select an Document."), QMessageBox::Yes);
        return;
    }
}

// ɾ���ĵ�(��ѡɾ��)
void MyTableView::delDoc()
{
    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    p->setSelDocUid(curUuid);

    int ret = QMessageBox::question(this, "", tr("Are you sure that delete the document ?"),
                                    QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes){
            if(!curUuid.isEmpty()){
                 // ��֤�Ҽ�ѡ���ɾ��
                DocDao::deleteDoc(curUuid);

                model->removeRow(curItem->row());

                // ѡ�е��߼�ɾ��
                QItemSelectionModel *selections = this->selectionModel();
                QModelIndexList selected = selections->selectedIndexes();
                QMap<int, int> rowMap;

                foreach (QModelIndex index, selected)
                {
                    QStandardItem* item = model->itemFromIndex(index);
                    QString  docUuid = qvariant_cast<QString>(item->data(Qt::UserRole));
                    DocDao::deleteDoc(docUuid);

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
     }
    if(ret == QMessageBox::No){
           return;
    }
}

// �ָ��ĵ�(��ѡ�ָ�)
void MyTableView::restoreDoc(){
    // ����ѡ���docUuid
    Preferences* p = Preferences::instance();
    p->setSelDocUid(curUuid);

    int ret = QMessageBox::question(this, "", tr("Are you sure that restore the document ?"),
                                    QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes){
            if(!curUuid.isEmpty()){
                 // ��֤�Ҽ�ѡ���ɾ��
                Doc doc = DocDao::selectDoc(curUuid);
                QString dirUUid = doc.DIR_GUID;

                // �õ�һĿ¼��
                QList<Dir> parentDirs;
                DirDao::selectAllParentDirbyDir(parentDirs, dirUUid);
                Dir curDir = DirDao::selectDir(dirUUid);
                parentDirs.append(curDir);

                bool hasNotExistParentDir = false;
                Dir dir;
                for (int var = 0; var < parentDirs.size(); var++) {
                    // �ļ���
                    dir = parentDirs.at(var);
                    QString delFlg = dir.DELETE_FLAG;
                    if(delFlg == "1"){
                        hasNotExistParentDir = true;
                        break;
                    }
                }
                // �������Ŀ¼������
                if(!hasNotExistParentDir){
                   DocDao::restoreDoc(curUuid);
                }else{
                  // ����ѡ��Ŀ¼
                    DocToDirDialog dlg(this, curPath, curUuid, false);
                    dlg.exec();
                    if(dlg.update){
                    }
                }

                model->removeRow(curItem->row());


                // ѡ�е��߼�ɾ��
                QItemSelectionModel *selections = this->selectionModel();
                QModelIndexList selected = selections->selectedIndexes();
                QMap<int, int> rowMap;

                foreach (QModelIndex index, selected)
                {
                    QStandardItem* item = model->itemFromIndex(index);
                    QString  docUuid = qvariant_cast<QString>(item->data(Qt::UserRole));
                    DocDao::restoreDoc(docUuid);

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
     }
    if(ret == QMessageBox::No){
           return;
    }
}

// ���Ҽ��˵�
void MyTableView::tableContextMenuOpened()
{
    QPoint pos = getCurPoint();

    if (cmenu->actions().count() > 0){
        // �ı���ɫ
        QModelIndex  index = indexAt(pos);
        QStandardItem * selrange = model->itemFromIndex(index);
        Doc doc = DocDao::selectDoc(curUuid);
        if(doc.DELETE_FLAG == "1") {
            cmenu->removeAction(deleteAction);
            cmenu->addAction(restoreAction);
        }else{
            cmenu->removeAction(restoreAction);
            cmenu->addAction(deleteAction);
        }
//        selrange->setData(QBrush(QColor(185, 210, 235)), Qt::BackgroundRole);
        cmenu->exec(this->viewport()->mapToGlobal(pos));
    }
}

// ��������Notes
void MyTableView::showMainNotes()
{
    emit shownotes();
}

// ֪ͨ����������Load��ǩ��
void MyTableView::reloadMainTagTree()
{
    emit reloadTagTree();
}

// TODO ѡ��ȫ���ļ�
void MyTableView::selectAllDoc()
{
      this->selectAll();
}

// ��ӡѡ���ļ�
void MyTableView::print()
{
    PrinterWidget diew(this);
    diew.printer();
}

// �ƶ����ļ���
void MyTableView::moveToDir()
{

    DocToDirDialog dlg(this, curPath, curUuid, false);
    dlg.exec();
    if(dlg.update){
          // ɾ��������ѡ�еĽڵ�
          model->removeRow(curItem->row());
    }
}

// �������ļ���
void MyTableView::copyToDir()
{
    DocToDirDialog dlg(this, curPath, curUuid, true);
    dlg.exec();
    if(dlg.update){
        // do nothing
    }
}

// �趨���ڶ���ѡ����Ŀ
void MyTableView::secondRowSetMenu()
{
    m_secondRowSetMenu = new QMenu(this);
    m_secondRowSetMenu->setStyleSheet(
                "QMenu::item{height: 25px}"
                "QMenu::item:hover{background-color:rgb(100,100,100)}"
                "QMenu::item:selected{background-color:rgb(128,128,128)}"
    );

    Preferences* p = Preferences::instance();
    QStringList selFields = p->getTableSelField();

    QAction *action1 = new QAction(this);
    action1->setData(CREATE_DATE);
    action1->setCheckable(true);
    action1->setText(tr("Create Date"));
    action1->setChecked(selFields.contains(QString::number(CREATE_DATE)));

    QAction *action2 = new QAction(this);
    action2->setData(MODIFIED_DATE);
    action2->setCheckable(true);
    action2->setText(tr("Modified Date"));
    action2->setChecked(selFields.contains(QString::number(MODIFIED_DATE)));

    QAction *action3 = new QAction(this);
    action3->setData(ACCESS_DATE);
    action3->setCheckable(true);
    action3->setText(tr("Accessed Date"));
    action3->setChecked(selFields.contains(QString::number(ACCESS_DATE)));

    QAction *action4 = new QAction(this);
    action4->setData(FILE_SIZE);
    action4->setCheckable(true);
    action4->setText(tr("Size"));
    action4->setChecked(selFields.contains(QString::number(FILE_SIZE)));

    QAction *action5 = new QAction(this);
    action5->setData(AUTHOR);
    action5->setCheckable(true);
    action5->setText(tr("Author"));
    action5->setChecked(selFields.contains(QString::number(AUTHOR)));

    QAction *action6 = new QAction(this);
    action6->setData(READ_COUNT);
    action6->setCheckable(true);
    action6->setText(tr("Read Count"));
    action6->setChecked(selFields.contains(QString::number(READ_COUNT)));

    QAction *action7 = new QAction(this);
    action7->setData(RELATED_COUNT);
    action7->setCheckable(true);
    action7->setText(tr("Related Count"));
    action7->setChecked(selFields.contains(QString::number(RELATED_COUNT)));

    QAction *action8 = new QAction(this);
    action8->setData(TAGS);
    action8->setCheckable(true);
    action8->setText(tr("Tags"));
    action8->setChecked(selFields.contains(QString::number(TAGS)));

    QAction *action9 = new QAction(this);
    action9->setData(URL);
    action9->setCheckable(true);
    action9->setText(tr("URL"));
    action9->setChecked(selFields.contains(QString::number(URL)));

    QAction *action10 = new QAction(this);
    action10->setData(LOCATION);
    action10->setCheckable(true);
    action10->setText(tr("Location"));
    action10->setChecked(selFields.contains(QString::number(LOCATION)));

    m_secondRowSetMenu->addAction(action1);
    m_secondRowSetMenu->addAction(action2);
    m_secondRowSetMenu->addAction(action3);
    m_secondRowSetMenu->addAction(action4);
    m_secondRowSetMenu->addAction(action5);
    m_secondRowSetMenu->addAction(action6);
    m_secondRowSetMenu->addAction(action7);
    m_secondRowSetMenu->addAction(action8);
    m_secondRowSetMenu->addAction(action9);
    m_secondRowSetMenu->addAction(action10);

}

// �趨���ڶ���ѡ����Ŀ
void MyTableView::slotShowSecondRowContent(QAction* action)
{
    QList<QAction*> actionList = m_secondRowSetMenu->actions();
    QAction* tmpaction;
    QList<int> selField;
    QStringList selFields;
    foreach(tmpaction, actionList){
        if(tmpaction->isChecked()){
            selField.append(tmpaction->data().toInt());
            selFields.append(tmpaction->data().toString());
        }
    }

    Preferences* p = Preferences::instance();
    p->setTableSelField(selFields);

    // updateSecRow(selField);

    this->repaint();
}


// �趨��ѡ��Menu
void MyTableView::slotShowTableOption(QAction* action)
{
    int offset = action->data().toInt();

    Preferences* p = Preferences::instance();
    // �趨��������Ŀ
    if(offset == 1){
       twoRowAction->setChecked(!oneRowAction->isChecked());
       p->setTableSelMode(QString::number(ONE_ROW));
       int rowCount = model->rowCount();
       for(int i = 0; i< rowCount; i++){
           if(i%2 == 1){
              this->hideRow(i);
           }
       }
    }
    // �趨��������Ŀ
    if(offset == 2){
       oneRowAction->setChecked(!twoRowAction->isChecked());
       p->setTableSelMode(QString::number(TWO_ROWS));
       int rowCount = model->rowCount();
       for(int i = 0; i< rowCount; i++){
           if(i%2 == 1){
              this->showRow(i);
           }
       }
    }
}

// ��ʾ�ĵ�����
void MyTableView::propOfDoc()
{
    // ȡ�õ�ǰ�ĵ�
    Doc doc = DocDao::selectDoc(curUuid);
    // �жϲ�����
    if(doc.DIR_GUID == ""){
        int ret = QMessageBox::warning(this, tr("Warning"),
                 tr("Please Confirm The original file has Deleted Or Moved. "), QMessageBox::Yes);

        if(ret == QMessageBox::Yes){
            return;
        }
    }

    m_propOfdocdlg = new PropOfDocDialog(this, curUuid);

    bool hasSelRight = false;
    // ��ѡ���ĵ�
    if(!curUuid.isEmpty()) {
        hasSelRight = true;
        m_propOfdocdlg->exec();
        if(m_propOfdocdlg->update){
          // �����κβ���
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(0, tr("Warning"), tr("Please Select an Document."), QMessageBox::Yes);
        return;
    }
}

// �����Ƿ����ĵ�������ʾNotesTips
void MyTableView::setShowNotesTips()
{
    Preferences* p = Preferences::instance();
    p->setShowNotesOnTips(showNotesAction->isChecked());
}

// ��ʾ�ĵ�����
QString MyTableView::getAvailableField(Doc doc)
{
    Preferences* p = Preferences::instance();
    QStringList fields = p->getTableSelField();
    QString field;
    QString temp;
    foreach (field, fields) {
        int selFied = field.toInt();
        if(selFied == CREATE_DATE){
           temp.append(doc.DT_CREATED);
        }
        if(selFied == MODIFIED_DATE){
           temp.append(doc.DT_MODIFIED);
        }
        if(selFied == ACCESS_DATE){
           temp.append(doc.DT_ACCESSED);
        }
        if(selFied == FILE_SIZE){
           // ����size K
           QFileInfo file(doc.DOCUMENT_LOCATION);
           temp.append(QString::number(file.size() / 1000));
        }
        if(selFied == AUTHOR){
           temp.append(doc.DOCUMENT_AUTHOR);
        }
        if(selFied == READ_COUNT){
           temp.append(doc.DOCUMENT_READ_COUNT);
        }
        if(selFied == RELATED_COUNT){
           temp.append(doc.DOCUMENT_RELATE_COUNT);
        }
        if(selFied == TAGS){
           // ȡ��Tags
            QList<Tag> sellitems = DocTagDao::selectTagsbyDocUuId(doc.DOCUMENT_GUID);
            QString tags;
            for (int var = 0; var < sellitems.length(); ++var) {
                Tag tag = sellitems.at(var);
                tags.append(tag.TAG_NAME);
                tags.append("/");
            }
            temp.append(tags);
        }
        if(selFied == URL){
           temp.append(doc.DOCUMENT_URL);
        }
        if(selFied == LOCATION){
           temp.append(doc.DOCUMENT_LOCATION);
        }
        temp.append(" ");
    }

    return temp;
}
// ��ʾ�ĵ�����
void MyTableView::setCurUuidByIndex(QModelIndex index){

    if(index.data().canConvert<Document>()){
            Document tp = index.data().value<Document>();
            curUuid = tp.docUuid();
            curPath = tp.docLoc();
    } else{
        if(index.data().canConvert<TableResult>()){
            TableResult tp = index.data().value<TableResult>();
            curUuid = tp.docuuid();
            curPath = tp.filepath();
        }
    }
}





