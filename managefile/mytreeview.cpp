//Ĭ�ϵ����ṹֻ��һ����������
#include <QStandardItem>
#include <QMessageBox>
#include <QModelIndex>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QUuid>
#include <QInputDialog>


#include "mytreeview.h"
#include "utils.h"
#include "fileutils.h"
#include "mytreeitemmodel.h"
#include "db/tagdao.h"
#include "db/docdao.h"
#include "db/dirdao.h"
#include "db/doctagdao.h"
#include "db/relatedocdao.h"
#include "db/notedao.h"
#include "preferences.h"

#include "importdocdialog.h"
#include "exportdocdialog.h"
#include "createsubdirdialog.h"
#include "movetodirdialog.h"
#include "movetotagdialog.h"
#include "propofdirdialog.h"
#include "propoftagdialog.h"
#include "createtagdialog.h"
#include "sortsubdirsdialog.h"

static  QModelIndex preindex;

MyTreeView::MyTreeView(QString title, QWidget *parent) : treeTitle("tree"), QTreeView(parent), numSubTree(2),
    mouseStatus(true), curTitle("title")
{
	//Ĭ���������������νṹ
        model = new MyTreeItemModel(parent);
        delegate = new MyTreeDelegate(parent);

        model->setHeaderData(0, Qt::Horizontal, title);

        this->setModel(model);
        this->setItemDelegate(delegate);

        QStandardItem  *allDocItem = new QStandardItem(tr("AllDocs"));
        allDocItem->setData("", UUID);  // UUid
        allDocItem->setData("alldocs", NODE_TYPE); // Type
        allDocItem->setData("folder.ico",  Qt::DecorationRole);


        QStandardItem  *allTagItem = new QStandardItem(tr("AllTags"));
        allTagItem->setData("", UUID);
        allTagItem->setData("alltags", NODE_TYPE);
        allTagItem->setData("tags.ico",  Qt::DecorationRole);

        QStandardItem  *wasteasketItem = new QStandardItem(tr("WasteBasket"));
        wasteasketItem->setData("", UUID);
        wasteasketItem->setData("basket", NODE_TYPE);
        wasteasketItem->setData("basket.ico",  Qt::DecorationRole);


                  //  "QTreeView::item:selected{background-color:rgb(128,128,128)}"
        //                      "QTreeView::item:hover{background-color:rgb(100,100,100)}"
        // QCSS
        this->setStyleSheet(
                    "QTreeView::branch {image:none;}"
                    "QTreeView::item{height: 25px;}"


                    "QTreeView::branch:closed:has-children:has-siblings { image: url(:/icons/expander_normal.png);}"
                    "QTreeView::branch:has-children:!has-siblings:closed { image: url(:/icons/expander_normal.png); }"
                    "QTreeView::branch:open:has-children:has-siblings { image: url(:/icons/expander_open.png);}"
                    "QTreeView::branch:open:has-children:!has-siblings { image: url(:/icons/expander_open.png); }"
        );

        if(title == "all"){
            model->setItem(0,0,allDocItem);
            model->setItem(1,0,allTagItem);
            model->setItem(2,0,wasteasketItem);
            loadDirs("", allDocItem);
            loadTagByParent("", allTagItem);
        }
        if(title == "tag"){
            model->setItem(1, 0, allTagItem);
            loadTagByParent("", allTagItem);
        }
        if(title == "doc"){
            model->setItem(0, 0, allDocItem);
            loadDirs("", allDocItem);
        }
        if(title == "basket"){
            model->setItem(0, 0, wasteasketItem);
            loadDelDirs("", wasteasketItem);
        }

        this->setMouseTracking(true);

        // ת�Ƶ�Tree�ڲ�ʵ��
        connect(this, SIGNAL(LBtnDbClk()), this, SLOT(showChildTree()));

        connect(this->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),
                this, SLOT(currentItemChanged(const QModelIndex &, const QModelIndex &)));

        this->setAnimated(true);
        //this->setColumnWidth(0,160);

        this->setDragEnabled(true);
        this->setAcceptDrops(true);

        m_appName = tr("File Manage");

        this->header()->hide();
        initActions();
}

// Action ����
void MyTreeView::initActions (){

    //Doc ContextMenu
    makeSubDir = new QAction(tr("&New Sub Dir"), this);
    connect(makeSubDir, SIGNAL(triggered()), this, SLOT(createSubDir()));
    moveToDir= new QAction(tr("&Move To Dir"), this);
    connect(moveToDir, SIGNAL(triggered()), this, SLOT(moveDir()));
    delDir= new QAction(tr("&Delete"), this);
    connect(delDir, SIGNAL(triggered()), this, SLOT(delSubDir()));
    renameDir= new QAction(tr("&Rename"), this);
    connect(renameDir, SIGNAL(triggered()), this, SLOT(renameSubDir()));

    // ����
    importDir = new QAction(Utils::getIcon("document-import.png"),tr("&Import..."), this);
    importDir->setShortcut(tr("Ctrl+I"));
    connect(importDir, SIGNAL(triggered()), this, SLOT(importDlg()));
    // ����
    exportDir = new QAction(Utils::getIcon("document-export.png"),tr("&Export..."), this);
    exportDir->setShortcut(tr("Ctrl+E"));
    connect(exportDir, SIGNAL(triggered()), this, SLOT(exportDlg()));

    subDirSort= new QAction(tr("&Sort SubDirs"), this);
    connect(subDirSort, SIGNAL(triggered()), this, SLOT(sortSubDirs()));

    showSubDirDoc= new QAction(tr("&Show docs under sub Dir"), this);
    showSubDirDoc->setCheckable(true);

    Preferences* p = Preferences::instance();
    showSubDirDoc->setChecked(p->isShowDocUnderSub());

    connect(showSubDirDoc, SIGNAL(triggered()), this, SLOT(setShowSubDirDoc()));

    protectDir= new QAction(Utils::getIcon("vip.png"), tr("&Protect"), this);
    connect(protectDir, SIGNAL(triggered()), this, SLOT(about()));
    propOfDir= new QAction(tr("&Properties"), this);
    connect(propOfDir, SIGNAL(triggered()), this, SLOT(properties()));

    //Root ContextMenu
    makeRootDir= new QAction(tr("&New Root Dir"), this);
    connect(makeRootDir, SIGNAL(triggered()), this, SLOT(createRootDir()));
    dirSort= new QAction(tr("&Sort Dir"), this);
    connect(dirSort, SIGNAL(triggered()), this, SLOT(sortSubDirs()));
    protectRootDir= new QAction(tr("&Protect"), this);
    connect(protectRootDir, SIGNAL(triggered()), this, SLOT(about()));
    optionOfDir= new QAction(tr("&Option"), this);
    connect(optionOfDir, SIGNAL(triggered()), this, SLOT(properties()));

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

    //Root Basket ContextMenu
    clearBasket= new QAction(tr("&Clear Basket"), this);
    connect(clearBasket, SIGNAL(triggered()), this, SLOT(dropBasket()));

    // �Ҽ�
    contextMenu = new QMenu(this);
    contextMenu->setStyleSheet(
                "QMenu::item{height: 25px}"
                "QMenu::item:hover{background-color:rgb(100,100,100)}"
                "QMenu::item:selected{background-color:rgb(128,128,128)}"
    );
    contextMenu->hide();
}

// ����
void MyTreeView::mousePressEvent(QMouseEvent *event)
{
        // �����ʾ�б�
        if( Qt::LeftButton == event->button()){
            curPoint = event->pos();
            curIndex = indexAt(curPoint);

            curItem = model->itemFromIndex(curIndex);
            if(curItem){
                changeColor();
                curTitle = curIndex.data().toString();
                curUuId =  qvariant_cast<QString>(curItem->data(UUID));
                curType = qvariant_cast<QString>(curItem->data(NODE_TYPE));
                emit LBtnClk();
                return;
            }
        }

        // �Ҽ���ʾ�˵�
        if(true == mouseStatus )
        {
            curPoint = event->pos();
            curIndex = indexAt(curPoint);
            curItem = model->itemFromIndex(curIndex);
            if(curItem){
                changeColor();
                curTitle = curIndex.data().toString();
                curUuId =  qvariant_cast<QString>(curItem->data(UUID));
                curType = qvariant_cast<QString>(curItem->data(NODE_TYPE));
                treeContextMenuOpened();
            }
        }
}

// ���Ҽ��˵�
void MyTreeView::treeContextMenuOpened()
{
    QPoint pos = getCurPoint();

    tableTree_currentItemChanged();
    if (contextMenu->actions().count() > 0){
        contextMenu->exec(this->viewport()->mapToGlobal(pos));
    }
}

// ���Ҽ��˵�
void MyTreeView::tableTree_currentItemChanged()
{
        contextMenu->clear();
        QString type = getCurType();
        QString uuid = getCurUuid();

        // ���ѡ��Root
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
            contextMenu->addAction(importDir);
            contextMenu->addAction(exportDir);
            contextMenu->addSeparator();
            contextMenu->addAction(subDirSort);
            contextMenu->addSeparator();
            contextMenu->addAction(showSubDirDoc);
            contextMenu->addAction(subDirSort);
            contextMenu->addAction(protectDir);
            contextMenu->addAction(subDirSort);
            contextMenu->addAction(propOfDir);
        } else if(type == "basket" && uuid.isEmpty()){
            contextMenu->addAction(clearBasket);
        } else if(type == "basket" && ! uuid.isEmpty()){
            contextMenu->addAction(makeSubDir);
            contextMenu->addSeparator();
            contextMenu->addAction(moveToDir);
            contextMenu->addAction(delDir);
            contextMenu->addAction(renameDir);
            contextMenu->addSeparator();
            contextMenu->addAction(importDir);
            contextMenu->addAction(exportDir);
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


MyTreeView::~MyTreeView()
{
        delete model;
}

void MyTreeView::setTreeTitle(QString title)
{
	model->setHeaderData(0, Qt::Horizontal, title);
}

void MyTreeView::setSubTreeTitle(int subTree, QString title)
{
	model->item(subTree)->setText(title);
}

void MyTreeView::addItem(int subTree, QString itemName, QString path, QString icon)
{
        QStandardItem  *parenItem = model->item(subTree);
        QStandardItem  *childItem =  new QStandardItem(itemName);
        childItem->setData(path, UUID);

        childItem->setIcon(Utils::getIcon(icon));
        parenItem->appendRow(childItem);
}

void MyTreeView::addItemByParentItem(QStandardItem *parentItem, QString itemName, QString uuid, QString type, QString icon)
{
        QStandardItem  *childItem =  new QStandardItem(itemName);
        childItem->setData(uuid, UUID);
        childItem->setData(type, NODE_TYPE);
        childItem->setData(icon,  Qt::DecorationRole);
        parentItem->appendRow(childItem);
}

bool MyTreeView::delSubItems(QStandardItem *parenItem)
{
    // ���ֱ�ӵ�
    for(int i = 0; i < parenItem->rowCount(); i++)
    {
        for(int j = 0; j < parenItem->columnCount(); j++)
        {
            model->removeRow(i, parenItem->index());
            model->removeColumn(j, parenItem->index());
        }
    }
    parenItem->setRowCount(0);
    return true;
}

void MyTreeView::addItems(int subTree, QList<QString> nameList)
{
	QStandardItem  *parenItem = model->item(subTree);
	QList<QStandardItem*> childItems;
	QStandardItem  *tmpItem = NULL;
	int   i = 0;
	
	for(i = 0; i < nameList.size(); i++)
	{
		if(tmpItem != NULL)
		{
			tmpItem = NULL;
		}
		tmpItem = new QStandardItem(nameList.at(i));
		childItems.push_back(tmpItem);
	}
	
	parenItem->appendRows(childItems);
}

void MyTreeView::delelteItem(int subTree, QString itemName)
{
	QStandardItem  *parenItem = model->item(subTree);
	int  i = 0;
	
	//�ҳ�����ΪitemName�����������ɾ��
	for(i = 0; i < parenItem->rowCount(); i++)
	{	
		if(itemName == parenItem->child(i)->text())
		{
			parenItem->removeRow (i);
			break;
		}
	}
}

void MyTreeView::clearTree(int subTree)
{
	QStandardItem  *parenItem = model->item(subTree);
	int   i = 0;
	for(i = parenItem->rowCount() - 1; i >= 0; i--)
	{
		parenItem->removeRow(i);
	}
	
}

void MyTreeView::enableMouse(bool yesOrNo)
{
	mouseStatus = yesOrNo;
}

bool MyTreeView::getMouseStatus()
{
	return mouseStatus;
}

QString MyTreeView::getCurTitle()
{
	return curTitle;
}

QModelIndex MyTreeView::getCurIndex()
{
	return curIndex;
}
QStandardItem* MyTreeView::getCurItem()
{
       return curItem;
}
QString MyTreeView::getCurPath()
{
    // �ݹ��path
    QString path = "";

    QString uuid = qvariant_cast<QString>(curItem->data(UUID));
    if(uuid == ""){
        return "";
    }

    QStandardItem *tmp = curItem;
    path.prepend(QDir::separator());
    do
    {
       path.prepend(tmp->text());
       path.prepend(QDir::separator());
       tmp = tmp->parent();
       if(tmp != NULL){
           uuid = qvariant_cast<QString>(tmp->data(UUID));
       }
    }while(uuid != "");

    curPath = path;
    return path;
}
QString MyTreeView::getCurUuid()
{
        return curUuId;
}
QString MyTreeView::getCurType()
{
        return curType;
}
QPoint MyTreeView::getCurPoint(){
      return curPoint;
}

// ���˫��
void MyTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{

        if(true == mouseStatus && Qt::LeftButton == event->button())
	{
                curPoint = event->pos();
                curIndex = indexAt(curPoint);
                if(curIndex.isValid()){
                       curItem = model->itemFromIndex(curIndex);

                       changeColor();
                       curTitle = curIndex.data().toString();
                       curUuId =  qvariant_cast<QString>(curItem->data(UUID));
                       curType = qvariant_cast<QString>(curItem->data(NODE_TYPE));
                       emit LBtnDbClk();
                }
	}
}
//// �Ҽ�����
//void myTreeList::mousePressEvent(QMouseEvent *event)
//{

//        // ����˳�
//        if( Qt::LeftButton == event->button()){
//           return;
//        }

//        if(true == mouseStatus )
//        {
//                QModelIndex  index = this->currentIndex();
//                curItem = model->itemFromIndex(index);
//                curPath = qvariant_cast<QString>(curItem->data());

//                curIndex = index;
//                curTitle = index.data().toString();
//                curPoint = event->pos();
//                emit RBtnClk();
//        }
//}

//// ����
//void MyTreeView::mouseReleaseEvent(QMouseEvent *event)
//{
//    // ����˳�
//    if( Qt::LeftButton == event->button()){

//        curPoint = event->pos();
//        curIndex= indexAt(curPoint);
//        preRow = curIndex.row();
//        preColumn = curIndex.column();

//        curItem = model->itemFromIndex(curIndex);
//        curTitle = curIndex.data().toString();
//        curUuId =  qvariant_cast<QString>(curItem->data(UUID));
//        curType = qvariant_cast<QString>(curItem->data(NODE_TYPE));
//        emit LBtnClk();
//        return;
//    }

//    if(true == mouseStatus && Qt::RightButton == event->button())
//    {
//            curPoint = event->pos();
//            curIndex= indexAt(curPoint);
//            preRow = curIndex.row();
//            preColumn = curIndex.column();

//            curItem = model->itemFromIndex(curIndex);
//            curTitle = curIndex.data().toString();
//            curUuId =  qvariant_cast<QString>(curItem->data(UUID));
//            curType = qvariant_cast<QString>(curItem->data(NODE_TYPE));
//            emit RBtnClk();
//    }
//}

// ֻ��ı���ɫ
void MyTreeView::changeColor(){

    QBrush white = QBrush(QColor(255, 255, 255));
    QBrush blue = QBrush(QColor(128,128,128));


    if(preindex.isValid()){
        QStandardItem* item = model->itemFromIndex(preindex);
        if(item){
            item->setData(white, Qt::BackgroundRole);
        }
    }

    if(curIndex.isValid()){
        QStandardItem* item = model->itemFromIndex(curIndex);
        if(item){
            item->setData(blue, Qt::BackgroundRole);
        }
    }
    preindex = curIndex;
}

//// �����ļ���Ŀ¼ȡ����Ŀ¼���ṹ
void MyTreeView::loadDirs(QString dirUuId,  QStandardItem *curItem){

    QList<Dir> dirs = DirDao::selectDirsbyParent(dirUuId, "0");

    //�����������ӽڵ�
    for(int i = 0; i < dirs.count(); i++)
    {
            Dir dir = dirs.at(i);
            QString icon = dir.DIR_ICON;
            if(icon.isEmpty()){
                icon = "folder.ico";
            }
            addItemByParentItem(curItem, dir.DIR_NAME, dir.DIR_GUID, "doc", icon);
    }
}

// �����ļ���Ŀ¼ȡ����Ŀ¼���ṹ
void MyTreeView::loadDelDirs(QString dirUuId,  QStandardItem *curItem){

    QList<Dir> dirs = DirDao::selectDirsbyParent(dirUuId, "1");

    //�����������ӽڵ�
    for(int i = 0; i < dirs.count(); i++)
    {
        Dir dir = dirs.at(i);
        QString icon = dir.DIR_ICON;
        if(icon.isEmpty()){
            icon = "folder.ico";
        }
        addItemByParentItem(curItem, dir.DIR_NAME, dir.DIR_GUID, "basket", icon);
    }
}
//
void MyTreeView::showImportDlg(){
     this->importDlg();
}
//
void MyTreeView::showExportDlg(){
     this->exportDlg();
}

// ���ݱ�ǩ���ж�
void MyTreeView::loadTagByParent(QString tagUuId, QStandardItem *curItem){
    // ����Ҫ�ж�tagUuId
    QList<Tag> tags = TagDao::selectTagsbyParent(tagUuId);

    //�����������ӽڵ�
    for(int i = 0; i < tags.count(); i++)
    {
            Tag tag = tags.at(i);
            addItemByParentItem(curItem, tag.TAG_NAME, tag.TAG_GUID, "tag", "tag.ico");
    }
}

// �򿪵�ǰ���ڵ�
void MyTreeView::showChildTree()
{

    QString curUId = getCurUuid();
    QStandardItem* curItem = getCurItem();
    QModelIndex curIndex = getCurIndex();

    QString type = qvariant_cast<QString>(curItem->data(NODE_TYPE));
    // ���ô�״̬
    if(isExpanded(curIndex)){
       collapse(curIndex);
    }else{
       expand(curIndex);
    }

    if(curItem->hasChildren()){
       return;
    }

    if(type == "doc" || type == "alldocs"){
       loadDirs(curUId, curItem);
    }

    if(type == "tag" || type == "alltags"){
       loadTagByParent(curUId, curItem);
    }

    if(type == "basket"){
       loadDelDirs(curUId, curItem);
    }
}

// ɾ�����ļ���
void MyTreeView::delSubTree()
{
    QString curType = getCurType();
    QStandardItem* curItem = getCurItem();
    if(!curItem){
       return;
    }

    if(!curType.isEmpty() && curType != "alltags" && curType != "alldocs") {
        delSubItems(curItem);
        curItem->parent()->removeRow(curItem->row());
    }
}

// ����CurPath���õ�ǰѡ����Ԫ��
void MyTreeView::setCurItemByUuid(QString uuId, QString type){

    QStandardItem *parenItem;
    //Doc��
    if(type == "alldocs" || type == "doc" ){
         parenItem = model->item(0);
    }else if (type == "alltags" || type == "tag"){
         parenItem = model->item(1);
    }else if (type == "basket"){
        parenItem = model->item(2);
   }

    //�����������ӽڵ�
    for(int i = 0; i < parenItem->rowCount(); i++)
    {
            QStandardItem *item = parenItem->child(i);
            QString tcurUuid = qvariant_cast<QString>(item->data(UUID));
            if(tcurUuid == uuId){
                curItem = item;
                curUuId = qvariant_cast<QString>(item->data(UUID));
                curIndex = item->index();
                curTitle = item->data().toString();
                break;
            }
    }
}
// �Ϸ��¼�
void MyTreeView::dropEvent(QDropEvent *event)
{
    qDebug() << "CLocalDirTreeView::dropEvent(QDropEvent *event)";
    qDebug() << "local::dropEvent mouse pos" << event->pos();
    QObject *source = qobject_cast<QObject *>(event->source());
    if (source && source != this) {
        qDebug() << "CLocalDirTreeView::dropEvent << " << event->mimeData()->text() << event->mimeData()->data("drag/remote");

        QModelIndex dropIndex = indexAt(event->pos());
        if(dropIndex != QModelIndex()) {
            //QDirModel *pModel = qobject_cast<QDirModel *>(model());
            //qDebug() << "drop to local path" << pModel->filePath(dropIndex) << "isdir" << pModel->isDir(dropIndex);
        }

        // ����
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
// �ı�ʱ��
void  MyTreeView::currentItemChanged(const QModelIndex &current, const QModelIndex &previous){
    qDebug("currentItemChanged");
     // �ı�Item������ɫ����ʾѡ��״̬
//    QStandardItem* preItem = model->itemFromIndex(previous);
//    if (!current.isValid()|| !previous.isValid()){
//        return;
//    }
//    if(preItem){
//         qDebug("change preItem");
//       preItem->setData(QBrush(QColor(255,255,255)), Qt::BackgroundRole);
//    }
//    QStandardItem* currentItem = model->itemFromIndex(current);
//    if(currentItem){
//           qDebug("change preItem");
//       currentItem->setData(QBrush(QColor(150,150,150)), Qt::BackgroundRole);
//    }
}
// ���¼���Tag��
void MyTreeView::reloadTagTree()
{
       clearTree(1);
       QStandardItem *allTagItem = model->item(1,0);
       loadTagByParent("", allTagItem);
}
//////////////////////////////////////////////////Action ҵ�����//////////////////////////////////////////////

// �����Ƿ���ʾ���ļ����µ��ļ�
void MyTreeView::setShowSubDirDoc()
{
    // ����ѡ��
    isShowDocUnderSub = showSubDirDoc->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSub(isShowDocUnderSub);
}

// �����Ƿ���ʾ�ӱ�ǩ�µ��ļ�
void MyTreeView::setShowSubTagDoc()
{
    // ����ѡ��
    isShowDocUnderTag = showSubDirTag->isChecked();
    Preferences* p = Preferences::instance();
    p->setShowDocUnderSubTag(isShowDocUnderTag);
}

// �򿪵������
void MyTreeView::importDlg()
{
    QString curType = getCurType();
    QString uuId = getCurUuid();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(!curType.isEmpty() && curType != "alldocs" && curType != "alltags") {
        hasSelRight = true;
        ImportDocDialog dlg(this, uuId, getCurPath());
        dlg.exec();
        if(dlg.update){
            // ���¼������ڵ�
            QStandardItem* curItem = getCurItem();
            delSubItems(curItem);
            loadDirs(uuId, curItem);
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub directory."), QMessageBox::Yes);
        return;
    }
}

// �򿪵�������
void MyTreeView::exportDlg()
{
    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(!curType.isEmpty() && curType != "alldocs" && curType != "alltags") {
        hasSelRight = true;
        ExportDocDialog dlg(this, curUuId, getCurPath(), curType);
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
void MyTreeView::properties()
{
    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ���ӽڵ�
    if(curType == "doc") {
        hasSelRight = true;
        PropOfDirDialog dlg(this, curUuId);
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
// �������ļ���
void MyTreeView::createSubDir()
{
    QString curType = getCurType();
    QString curUuid = getCurUuid();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(curType == "doc") {
        hasSelRight = true;
        CreateSubDirDialog dlg(this, curUuid, getCurPath());
        dlg.exec();
        if(dlg.update){

            // ˢ��ѡ�е���
            QStandardItem* curItem = getCurItem();
            addItemByParentItem(curItem, dlg.dirName->text(), dlg.m_newUuid, "doc", "folder.ico");
            expand(getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// �ƶ��ļ���
void MyTreeView::moveDir()
{
    QString curType = getCurType();
    QString curUuid = getCurUuid();
    QString curTitle = getCurTitle();
    QStandardItem* curItem = getCurItem();
    bool hasSelRight = false;

    // ��ѡ�� �ļ� �ӽڵ�
    if(curType == "doc") {
        hasSelRight = true;
        MoveToDirDialog dlg(this, curUuid, getCurPath());
        dlg.exec();
        if(dlg.update){
              // ȡ���ӽ���ѡ�е�path
              QString toDirUuid = dlg.m_toUuid;
              // ɾ��������ѡ�еĽڵ�
              curItem->parent()->removeRow(curItem->row());

              // ����������Ľڵ� (�ӽ����½��ļ�������²��ɹ�)
              setCurItemByUuid(toDirUuid, curType);

              QStandardItem* curItem = getCurItem();
              addItemByParentItem(curItem, curTitle, curUuid, "doc", "folder.ico");
              expand(getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}


// �ı����ļ�������
void MyTreeView::renameSubDir()
{
    QString curType = getCurType();
    QStandardItem* curItem = getCurItem();
    QString curTitle = getCurTitle();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(!curType.isEmpty() && curType != "alltags") {
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
                Dir dir;
                dir.DIR_GUID = getCurUuid();
                dir.DIR_NAME = text;
                dir.MF_VERSION = 0;
                dir.DIR_ORDER = 0;
                DirDao::updateDir(dir);
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// ���ļ�������
void MyTreeView::sortSubDirs()
{
    QString curType = getCurType();
    QString curUuid = getCurUuid();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(curType == "doc") {
        hasSelRight = true;
        SortSubDirsDialog dlg(this, curUuid, getCurPath());
        dlg.exec();
        if(dlg.update){
            // ˢ��ѡ�е���
            setCurItemByUuid(curUuid, curType);
            delSubItems(curItem);
            //showChildTree();
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// �½����ڵ�
void MyTreeView::createRootDir()
{
    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ�
    if(curType == "alldocs") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Roor Directory name:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
                if (text == ""){
                  return;
                }
                // ����Ŀ¼
                Dir dir;
                dir.DIR_GUID = QUuid::createUuid();
                dir.DIR_PARENT_UUID = "";
                dir.DIR_NAME = text;
                dir.DIR_ICON = "folder.ico";
                dir.DIR_ORDER = 0;
                dir.DELETE_FLAG = '0';
                dir.MF_VERSION = 0;
                DirDao::insertDir(dir);

                QStandardItem* curItem = getCurItem();
                addItemByParentItem(curItem, text, dir.DIR_GUID , "doc", "folder.ico");
                expand(getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// �½�����ǩ
void MyTreeView::newTag()
{
    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ�� ��ǩ �ڵ�
    if(curType == "alltags" || curType == "tag") {
        hasSelRight = true;
        // curPath �����жϸ��ڵ���ӽڵ�
        CreateTagDialog dlg(this, getCurUuid());
        dlg.exec();
        if(dlg.update){
              QStandardItem* curItem = getCurItem();
              addItemByParentItem(curItem,  dlg.m_tagname, dlg.m_newTagUuId, "tag", "tag.ico");
              expand(getCurIndex());
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// ɾ����ǩ
void MyTreeView::deleteTag()
{
    QString curType = getCurType();
    bool hasSelRight = false;
    QString title = getCurTitle();
    QString curUuId = getCurUuid();

    // ��ѡ�� ��ǩ �ڵ�
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
          delSubTree();
          // ɾ��Tag
          TagDao::deleteTag(curUuId);
          // ɾ������Ϊ��������Tag
          QList<Tag> tags;
          TagDao::selectAllSubTagbyTag(tags, curUuId);

          for (int var = 0; var < tags.length(); ++var) {
              Tag tag = tags.at(var);
              TagDao::deleteTag(tag.TAG_GUID);
              DocTagDao::deleteDocTagByTag(tag.TAG_GUID);
          }
        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an Sub Tag."), QMessageBox::Yes);
        return;
    }
}

// �ı��ӱ�ǩ����
void MyTreeView::renameSubTag()
{
    QString curType = getCurType();
    QStandardItem* curItem = getCurItem();
    QString curTitle = getCurTitle();
    QString curUuId = getCurUuid();

    bool hasSelRight = false;

    // ��ѡ�� �ܽڵ���ӽڵ�
    if(curType == "tag") {
        hasSelRight = true;
        bool ok;
        QString text = QInputDialog::getText(this, m_appName, tr("New Tag name:"), QLineEdit::Normal, curTitle, &ok);
        if (ok && !text.isEmpty()) {
                if (text == curTitle){
                  return;
                }
                // �ı����ڵ�
                curItem->setData(text, Qt::DisplayRole);
                // �ı��ǩ����
                // ɾ��Tag
                Tag tag;
                tag.TAG_GUID = curUuId;
                tag.TAG_NAME = text;
                TagDao::updateTag(tag);
        }

    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

// ��ʾ��ǩ����
void MyTreeView::showPropOfTag()
{
    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ�� ��ǩ �ڵ�
    if(curType == "tag") {
        QString tagname = getCurTitle();
        QStandardItem*  curItem = getCurItem();
        QString curUuId = getCurUuid();
        Tag tag = TagDao::selectTag(curUuId);
        hasSelRight = true;
        // curPath �����жϸ��ڵ���ӽڵ�
        // getCurPath ���ڴ�UuId
        PropOfTagDialog dlg(this, tag.TAG_GUID, tagname, tag.TAG_DESCRIPTION);
        dlg.exec();
        if(dlg.update){
            if (dlg.m_tagname.isEmpty()){
              // ��ʾ����
              return;
            }
            // �ı����ڵ�
            curItem->setData(dlg.m_tagname, Qt::DisplayRole);

        }
    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an directory."), QMessageBox::Yes);
        return;
    }
}

void MyTreeView:: moveToRoot(){

    QString curType = getCurType();
    bool hasSelRight = false;

    // ��ѡ�� ��ǩ �ڵ�
    if(curType == "tag" ) {
        QString curUuid = getCurUuid();
        Tag tag = TagDao::selectTag(curUuid);
        hasSelRight = true;
        tag.TAG_GROUP_GUID = "";
        TagDao::updateTag(tag);
        // ����load�ı�ǩ��
        // ɾ����ǰ�Ľڵ�
        delSubTree();

        // ����������ڵ�
        addItem(1, tag.TAG_NAME, tag.TAG_GUID, "expander_normal.png");

    }
    // ���û��ѡ����Ŀ¼�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an Tag."), QMessageBox::Yes);
        return;
    }
}

// �ƶ���ǩ
void MyTreeView::movetoTag()
{
    QString curType = getCurType();
    QStandardItem* curItem = getCurItem();
    QString curTitle = getCurTitle();
    QString curPath = getCurPath();
    QString curUuId = getCurUuid();
    bool hasSelRight = false;

    // ��ѡ�� �ļ� �ӽڵ�
    if(curType == "tag") {
        hasSelRight = true;
        MoveToTagDialog dlg(this, curPath);
        dlg.exec();
        if(dlg.update){
              // ȡ���ӽ���ѡ�е�path
              QString mselUuId = dlg.m_selUuId;
              // ɾ��������ѡ�еĽڵ�
              curItem->parent()->removeRow(curItem->row());

              // ����������Ľڵ� (�ӽ����½��ļ�������²��ɹ�)
              setCurItemByUuid(curUuId,  curType);

              QString curPath = getCurPath();
              if(!curPath.isEmpty()){
                  QStandardItem* curItem = getCurItem();
                  addItemByParentItem(curItem, curTitle, mselUuId, "tag", "tag.ico");
                  curItem->setIcon(Utils::getIcon("expander_open.png"));
                  expand(getCurIndex());
              }
        }
    }
    // ���û��ѡ���ӱ�ǩ�ڵ�
    if(!hasSelRight){
        QMessageBox::warning(this, tr("Warning"), tr("Please Select an sub tag."), QMessageBox::Yes);
        return;
    }
}

// ɾ�����ļ���
void MyTreeView::delSubDir()
{
    QString curTitle = getCurTitle();
    QString curUuid = getCurUuid();
    int ret = QMessageBox::question(this, "",
                                    tr("Are you sure that delete the directory \"%1\"?").arg(curTitle),
                                    QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes){
       delSubTree();
       // ɾ�����ڵ�
       DirDao::deleteDir(curUuid);
       DirDao::updateToRootDir(curUuid);
       DocDao::deleteDocByDirUid(curUuid);

       // ɾ���ӽڵ�
       QList<Dir> selDirList;
       DirDao::selectAllSubDirbyDir(selDirList, curUuid, "0");
       // �߼�ɾ��
       for (int var = 0; var < selDirList.size(); ++var) {
           // �ļ���
           Dir dir = selDirList.at(var);
           DirDao::deleteDir(dir.DIR_GUID);
           // �ļ����µ��ļ�
           DocDao::deleteDocByDirUid(dir.DIR_GUID);
       }
    }

    if(ret == QMessageBox::No){
       return;
    }
}


// ɾ�����ļ���
void MyTreeView::dropBasket()
{
    int ret = QMessageBox::question(this, "",
                                    tr("Are you sure that drop the Basket ?"),
                                    QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes){
       QStandardItem *parenItem = model->item(2);
       delSubItems(parenItem);


       QList<Doc> deldocs = DocDao::selectDocsByDelFlg("1");
       Doc doc;

       QString notePath = Utils::getLocateNotesPath();
       foreach (doc, deldocs) {
           // ɾ���ĵ������ı�ע
           QList<Note> notes = NoteDao::selectNotesbyDocUuId(doc.DOCUMENT_GUID);
           Note note;
           foreach (note, notes) {
               QString noteUuId = note.NOTE_GUID;
               // ɾ��note�ļ�
               QString filepath = notePath;
               filepath.append(QDir::separator());
               filepath.append(noteUuId);
               filepath.append(".html");
               QFileInfo file(filepath);
               if(file.exists()){
                   QFile::remove(filepath);
               }
           }
           NoteDao::deleteNoteByDoc(doc.DOCUMENT_GUID);

           // ɾ�������ĵ�
           RelateDocDao::deleteRelateDocByDocUuId(doc.DOCUMENT_GUID);
           // ɾ���ĵ�Tag
           DocTagDao::deleteDocTagByDoc(doc.DOCUMENT_GUID);
       }

       DirDao::physicalDelDir();
       DocDao::physicalDelDoc();
    }

    if(ret == QMessageBox::No){
       return;
    }
}



