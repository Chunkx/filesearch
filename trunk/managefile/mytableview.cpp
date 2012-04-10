#include<QPainter>
#include<QMouseEvent>
#include<QStandardItem>
#include<QHeaderView>
#include<QDir>
#include<QAction>
#include<QMenu>
#include<QMessageBox>
#include<QDesktopServices>

#include"mytableview.h"
#include"fileUtils.h"
#include "preferences.h"
#include "utils.h"

#ifndef NDEBUG
#include<QDebug>
#endif

static int n_orow;

MyTableView::MyTableView(QWidget * parent) : QTableView(parent), mouseStatus(true)
{
    delegate = new MyItemDelegate(parent);
    themodel = new MyStandardItemModel(parent);

    this->setModel(themodel);
    this->setItemDelegate(delegate);

    horizontalHeader()->setMouseTracking (true);
    horizontalHeader ()->installEventFilter (this);

    this->setStyleSheet(
                "QTableView{selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0.5, y2: 0.5,stop: 0 #FF92BB, stop: 1 white);}"
                "QTableView::QTableCornerButton::section {background: red;border: 2px outset red;}"
                "QTableView::item:hover{background-color:rgb(128, 128, 128)}"

    );
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setMouseTracking(true);//important
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setAcceptDrops(true);
    initActions();
}

// ����ƿ��¼�
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
    connect(openInSysExploreAction, SIGNAL(triggered()), this, SLOT(about()));

    // ����
    exportDocAction = new QAction(tr("&Export.."), this);
    connect(exportDocAction, SIGNAL(triggered()), this, SLOT(about()));

    // �Ը�����ʽ�����ʼ�
    emailAsAttachAction = new QAction(tr("&Send Email As Attachment"), this);
    connect(emailAsAttachAction, SIGNAL(triggered()), this, SLOT(about()));

    // �ĵ��ʼ�
    noteOfDocAction = new QAction(tr("&Notes"), this);
    connect(noteOfDocAction, SIGNAL(triggered()), this, SLOT(about()));

    // �����ĵ�
    relatedDocAction = new QAction(tr("&Related Documents"), this);
    connect(relatedDocAction, SIGNAL(triggered()), this, SLOT(about()));

    // ��ǩ
    tagAction = new QAction(tr("&Tags"), this);
    connect(tagAction, SIGNAL(triggered()), this, SLOT(about()));

    // ɾ��
    deleteAction = new QAction(tr("&Delete(Move to the Wastebasket)"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(about()));

    // ����
    encodeAction = new QAction(Utils::getIcon("vip.png"), tr("&Encrypt"), this);
    connect(encodeAction, SIGNAL(triggered()), this, SLOT(about()));

    // �����滻
    findAndReplaceAction = new QAction(Utils::getIcon("vip.png"), tr("&Find and Replace"), this);
    connect(findAndReplaceAction, SIGNAL(triggered()), this, SLOT(about()));

    // ȫѡ
    selectAllAction = new QAction(tr("&Select All"), this);
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(about()));

    // ��ӡ
    printAction = new QAction(tr("&Print"), this);
    connect(printAction, SIGNAL(triggered()), this, SLOT(about()));

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
   restoreAction = new QAction(tr("&Restore And Repair Document"), this);
   connect(restoreAction, SIGNAL(triggered()), this, SLOT(about()));
   // ������ӡ�ĵ�
   pprintAction = new QAction(tr("&Print Documents In Batch"), this);
   connect(pprintAction, SIGNAL(triggered()), this, SLOT(about()));
   // �ı��ĵ���ʽ
   pformatChangeAction = new QAction(tr("&Change format in the Document"), this);
   connect(pformatChangeAction, SIGNAL(triggered()), this, SLOT(about()));
   menu_ad->addAction(convertDocAction);
   menu_ad->addAction(combineAction);
   menu_ad->addAction(restoreAction);
   menu_ad->addAction(pprintAction);
   menu_ad->addAction(pformatChangeAction);


   // �ƶ����ļ���
   moveToDirAction = new QAction(tr("&Move to..."), this);
   connect(moveToDirAction, SIGNAL(triggered()), this, SLOT(about()));
   // ���Ƶ��ļ���
   copyToDirAction = new QAction(tr("&Copy to..."), this);
   connect(copyToDirAction, SIGNAL(triggered()), this, SLOT(about()));
   // ѡ��
   optionOfDocTableAction = new QAction(tr("&Option"), this);
   connect(optionOfDocTableAction, SIGNAL(triggered()), this, SLOT(about()));
   // ����
   propAction = new QAction(tr("&Properties"), this);
   connect(propAction, SIGNAL(triggered()), this, SLOT(about()));

   cmenu = new QMenu(this);
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
   cmenu->addAction(deleteAction);
   cmenu->addAction(optionOfDocTableAction);
   cmenu->addAction(propAction);
}

// ����ƿ��¼�
void MyTableView::leaveEvent (QEvent * event )
{
        MyStandardItemModel *m = (MyStandardItemModel *)model();
        m->setHoverRow(-1);
        int columnCount = m->columnCount();
        for (int i = columnCount - 1; i >= 0; i--)
        {
                update(model()->index(n_orow, i));

        }
        n_orow = -1;
}

// ������
void MyTableView::updateRow(int row)
{
        if (row == n_orow){
           return;
        }
        MyStandardItemModel *m = (MyStandardItemModel *)model();
        m->setHoverRow(row);
        int columnCount = model()->columnCount();
        for (int i = columnCount - 1; i >= 0; i--)
        {

                update(model()->index(n_orow, i));
                update(model()->index(row, i));

        }
        n_orow = row;
}

void MyTableView::wheelEvent ( QWheelEvent * event )
{
        QTableView::wheelEvent (event);
        int nrow = indexAt(event->pos()).row();
        updateRow(nrow);

}

bool MyTableView::eventFilter (QObject* object,QEvent* event)
{
   if ( object == horizontalHeader () )
   {
           if ( event->type() == QEvent::Enter )
           {
                 leaveEvent(event);
           }
   }
   return QTableView::eventFilter (object, event);
}

// ���ڴ�С���ڸ��еĳ���
void MyTableView::resizeEvent(QResizeEvent * event){
    int tablewidth = this->width();
    this->setColumnWidth(0, tablewidth * 1);
}

void MyTableView::buildDocList(QStringList files)
{
    Preferences* p = Preferences::instance();
    themodel->clear();
    qDebug("buildDocList start");

    themodel->setRowCount(files.size());
    themodel->setColumnCount(1);

    for (int var = 0; var < files.size(); ++var) {
         QString str = files.at(var);
         QStringList tempArr = str.split(",");
         if(tempArr.length() != 2){
           continue;
         }
         str = tempArr.at(1);

         // ������Ϊ��ʱ
         if(str.isEmpty()){
              continue;
         }
         int dotpos = str.lastIndexOf(".");
         int splitpos = str.lastIndexOf(QDir::separator());
         QString filename = str.right(str.length() - splitpos - 1);
         QString icon = str.right(str.length() - dotpos - 1).toLower();
         QString dotsuffix = str.right(str.length() - dotpos);
         QString suffix = "*" + dotsuffix;

         QList<QStandardItem*> items;
         QStandardItem* item = new QStandardItem();

         if(p->word().contains(suffix, Qt::CaseInsensitive)){

            // item->setData(str, wordItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->excel().contains(suffix, Qt::CaseInsensitive)){
             // item->setData(str, excelItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->ppt().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, pptItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->pdf().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, pdfItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->htmls().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, htmlItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->pics().contains(suffix, Qt::CaseInsensitive)){
            //item->setData(str, picItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->swfs().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, swfItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->sources().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, sourceItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->txts().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, txtItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }
         if(p->movies().contains(suffix, Qt::CaseInsensitive)){
            // item->setData(str, txtItemType);
            item->setData(filename, Qt::DisplayRole);
            item->setData(str, Qt::ToolTipRole);
         }

         if(p->allsupported().contains(suffix, Qt::CaseInsensitive)){
             icon = icon.append(".ico");
             item->setIcon(Utils::getIcon(icon));
             items.append(item);
             themodel->insertRow(var, items);
         }
    }
    // ���ø��б���,ʹ��ռ��ȫ��
    int tablewidth = this->width();
    this->setColumnWidth(0, tablewidth * 1);
    qDebug("buildDocList start");
}

void MyTableView::mouseMoveEvent(QMouseEvent * event)
{
    // �ı���ɫ
    int nrow = indexAt(event->pos()).row();
    curPoint = event->pos();
    updateRow(nrow);

    int column=this->columnAt(event->x());
    int row=this->rowAt(event->y());
    if(column==0 && row!=-1){
        this->setCursor(Qt::PointingHandCursor);
    } else {
        this->setCursor(Qt::ArrowCursor);
    }
}

// ���˫��
void MyTableView::mouseDoubleClickEvent(QMouseEvent *event)
{

        if(true == mouseStatus && Qt::LeftButton == event->button())
        {
                QModelIndex  index = this->currentIndex();
                QStandardItem *curItem = themodel->itemFromIndex(index);
                curPath = qvariant_cast<QString>(curItem->data(Qt::ToolTipRole));
                emit LBtnDbClk();
        }
}
// �Ҽ�����
void MyTableView::mousePressEvent(QMouseEvent *event)
{
        // ����˳�
        if( Qt::LeftButton == event->button()){
          return;
        }

        // �Ҽ���ʾ�˵�
        if(true == mouseStatus )
        {
            curPoint = event->pos();
            QModelIndex  index = indexAt(curPoint);
            QStandardItem *curItem = themodel->itemFromIndex(index);
            curPath = qvariant_cast<QString>(curItem->data(Qt::ToolTipRole));
            tableContextMenuOpened();
        }
}

QPoint MyTableView::getCurPoint(){
    return curPoint;
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
    QDesktopServices::openUrl ( QUrl::fromLocalFile(curPath) );
}
// ������Tabҳ��
void MyTableView::openInTab()
{
    // �൱��˫��
    emit LBtnDbClk();
}

// ���Ҽ��˵�
void MyTableView::tableContextMenuOpened()
{
    QPoint pos = getCurPoint();
    if (cmenu->actions().count() > 0){
       cmenu->exec(this->viewport()->mapToGlobal(pos));
    }
}




