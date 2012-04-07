#include<QPainter>
#include<QMouseEvent>
#include<QStandardItem>
#include<QHeaderView>
#include<QDir>
#include"mytableview.h"
#include"fileUtils.h"
#include "preferences.h"
#include "utils.h"

#ifndef NDEBUG
#include<QDebug>
#endif

static int n_orow;

MyTableView::MyTableView(QWidget * parent) :QTableView(parent), mouseStatus(true)
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

}

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

void MyTableView::updateRow(int row)
{
        if (row == n_orow)
                return;
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

                QString curPath = qvariant_cast<QString>(curItem->data());
//                curIndex = index;
//                curTitle = index.data().toString();

                emit LBtn_DbClk();
        }
}
// �Ҽ�����
void MyTableView::mousePressEvent(QMouseEvent *event)
{

        // ����˳�
        if( Qt::LeftButton == event->button()){
          return;
        }

        if(true == mouseStatus )
        {
            QModelIndex  index = this->currentIndex();
            QStandardItem *curItem = themodel->itemFromIndex(index);

            QString curPath = qvariant_cast<QString>(curItem->data());
            curPoint = event->pos();
            emit RBtn_Clk();
        }
}

QPoint MyTableView::getCurPoint(){
    return curPoint;
}
void MyTableView::enableMouse(bool yesOrNo)
{
        mouseStatus = yesOrNo;
}

bool MyTableView::getMouseStatus()
{
        return mouseStatus;
}

