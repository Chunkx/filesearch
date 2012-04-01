//Ĭ�ϵ����ṹֻ��һ����������
#include <QStandardItem>
#include <QMessageBox>
#include <QModelIndex>
#include <QDir>


#include "myTreeList.h"
#include "utils.h"
#include "fileutils.h"
#include "db/tagdao.h"


myTreeList::myTreeList(QString title, QWidget *parent) : treeTitle("tree"), QTreeView(parent), numSubTree(2),
    mouseStatus(true), curTitle("title")
{
	//Ĭ���������������νṹ
        model = new QStandardItemModel(1,1);
        model->setHeaderData(0, Qt::Horizontal, title);

        QStandardItem  *allDocItem = new QStandardItem(tr("AllDocs"));
        allDocItem->setIcon(Utils::getIcon("expander_normal.png"));
        allDocItem->setData("alldocs", Qt::UserRole + 1);

        QStandardItem  *allTagItem = new QStandardItem(tr("AllTags"));
        allTagItem->setIcon(Utils::getIcon("expander_normal.png"));
        allTagItem->setData("alltags", Qt::UserRole + 1);

        // QCSS
        this->setStyleSheet(
                    "QTreeView::branch {image:none;}"
                    "QTreeView::item{height: 25px;}"
                    "QTreeView::item:hover{background-color:rgb(100,100,100)}"
                    "QTreeView::item:selected{background-color:rgb(128,128,128)}"
        );
        if(title == "all"){
            model->setItem(0, allDocItem);
            model->setItem(1, allTagItem);
        }

        if(title == "tag"){
            model->setItem(0, allTagItem);
        }

        if(title == "doc"){
            model->setItem(0, allDocItem);
        }


        // ת�Ƶ�Tree�ڲ�ʵ��
        connect(this, SIGNAL(LBtnDbClk()), this, SLOT(showChildTree()));

	this->setModel(model);
	this->setAnimated(true);
}

myTreeList::~myTreeList()
{
        delete model;
}

void myTreeList::setTreeTitle(QString title)
{
	model->setHeaderData(0, Qt::Horizontal, title);
}

void myTreeList::setSubTreeTitle(int subTree, QString title)
{
	model->item(subTree)->setText(title);
}

void myTreeList::addItem(int subTree, QString itemName, QString path, QString icon)
{
        QStandardItem  *parenItem = model->item(subTree);
        QStandardItem  *childItem =  new QStandardItem(itemName);
        childItem->setData(path, Qt::UserRole + 1);

        childItem->setIcon(Utils::getIcon(icon));
        parenItem->appendRow(childItem);
}

void myTreeList::addItemByParentItem(QStandardItem *parenItem, QString itemName, QString path, QString icon)
{
        QStandardItem  *childItem =  new QStandardItem(itemName);
        childItem->setData(path, Qt::UserRole + 1);
        childItem->setIcon(Utils::getIcon(icon));
        parenItem->appendRow(childItem);
}

bool myTreeList::delSubItems(QStandardItem *parenItem)
{
    // ���ֱ�ӵ�
    for(int i = 0; i < parenItem->rowCount(); i++)
    {
       parenItem->removeRow (i);
    }

    return true;
}


void myTreeList::addItems(int subTree, QList<QString> nameList)
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

void myTreeList::delelteItem(int subTree, QString itemName)
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

void myTreeList::clearTree(int subTree, QString itemName)
{
	QStandardItem  *parenItem = model->item(subTree);
	int   i = 0;
	for(i = parenItem->rowCount() - 1; i >= 0; i--)
	{
		parenItem->removeRow(i);
	}
	
}

void myTreeList::enableMouse(bool yesOrNo)
{
	mouseStatus = yesOrNo;
}

bool myTreeList::getMouseStatus()
{
	return mouseStatus;
}

QString myTreeList::getCurTitle()
{
	return curTitle;
}

QModelIndex myTreeList::getCurIndex()
{
	return curIndex;
}
QStandardItem* myTreeList::getCurItem()
{
       return curItem;
}
QString myTreeList::getCurPath()
{
        return curPath;
}
QPoint myTreeList::getCurPoint(){
      return curPoint;
}
// ���˫��
void myTreeList::mouseDoubleClickEvent(QMouseEvent *event)
{

        if(true == mouseStatus && Qt::LeftButton == event->button())
	{
                QModelIndex  index = this->currentIndex();
                curItem = model->itemFromIndex(index);

                curPath = qvariant_cast<QString>(curItem->data());
		curIndex = index;
		curTitle = index.data().toString();

		emit LBtnDbClk();
	}
}
//// �Ҽ�����
//void myTreeList::mousePressEvent(QMouseEvent *event)
//{

//        // ����˳�
//        if( Qt::LeftButton == event->button()){
//          return;
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

// ����
void myTreeList::mouseReleaseEvent(QMouseEvent *event)
{
    // ���õ�ǰĿ¼
    QModelIndex  index = this->currentIndex();
    curItem = model->itemFromIndex(index);
    curPath = qvariant_cast<QString>(curItem->data());

    // ����˳�
    if( Qt::LeftButton == event->button()){
        emit LBtnClk();
        return;
    }

    if(true == mouseStatus && Qt::RightButton == event->button())
    {
            curIndex = index;
            curTitle = index.data().toString();
            curPoint = event->pos();
            emit RBtnClk();
    }
}

//// �����ļ���Ŀ¼ȡ����Ŀ¼���ṹ
void myTreeList::loadDirByLay(QString parentPath, int lay, QStandardItem *curItem){
    //Ŀ¼
    QDir dir(parentPath);
    if (!dir.exists()) {
        return;
    }
    // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //�ļ�������
    dir.setSorting(QDir::DirsFirst);

    //ת����һ��list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return;
    }

    int i=0;
    // ����Ŀ¼
    do{
        QFileInfo fileInfo = list.at(i);
        QString filename = fileInfo.fileName();
        QString filepath = fileInfo.filePath();
        filepath = QDir::toNativeSeparators(filepath);
        //������ļ���
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            if(lay == 0){
              addItem(lay, filename, filepath, "expander_normal.png");
            }else{
              addItemByParentItem(curItem, filename, filepath, "expander_normal.png");
            }
        }
        i++;
    } while(i < list.size());
}

// ���ݱ�ǩ���ж�
void myTreeList::loadTagByParent(QString tagUuId, QStandardItem *curItem){
    // ����Ҫ�ж�tagUuId
    // QString tagUuIds = new QString(tagUuId);
    QList<Tag> tags = TagDao::selectTagsbyParent(tagUuId);

    //�����������ӽڵ�
    for(int i = 0; i < tags.count(); i++)
    {
            Tag tag = tags.at(i);
            addItemByParentItem(curItem, tag.TAG_NAME, tag.TAG_GUID, "expander_normal.png");
    }
}


// �򿪵�ǰ���ڵ�
void myTreeList::showChildTree()
{
    QString path = getCurPath();
    if(path.isEmpty()){
      return;
    }

    QStandardItem* curItem = getCurItem();
    QModelIndex curIndex = getCurIndex();

    // ���ô�״̬
    if(isExpanded(curIndex)){
       curItem->setIcon(Utils::getIcon("expander_normal.png"));
       collapse(getCurIndex());
    }else{
       curItem->setIcon(Utils::getIcon("expander_open.png"));
       expand(getCurIndex());
    }

    // �Ȳ���Ҫ������
    if(curItem->hasChildren()){
       return;
    }

    if(path != "alldocs" && path != "alltags"){
       loadDirByLay(path, 1, curItem);
    }

    // �ж��Ƿ�ѡ�� ��ǩ �ڵ�
    if(path == "alltags" || (path != "alldoc" && path.indexOf(QDir::separator()) == -1 )){
        if(path == "alltags"){
           path = "";
        }
       loadTagByParent(path, curItem);
    }
}

// ɾ�����ļ���
void myTreeList::delSubTree()
{
    QString curPath = getCurPath();
    QStandardItem* curItem = getCurItem();
    if(!curItem){
       return;
    }

    if(!curPath.isEmpty() && curPath != "alltags" && curPath != "alldocs") { 
        delSubItems(curItem);
        curItem->parent()->removeRow(curItem->row());
    }
}

// ����CurPath���õ�ǰѡ����Ԫ��
void myTreeList::setCurItemByPath(QString path)
{
    curPath = "";
    QStandardItem *parenItem;
    //Doc��
    if(path == "alldocs" || path.indexOf(QDir::separator()) != -1 ){
         parenItem = model->item(0);
    }else{
         parenItem = model->item(1);
    }

    //�����������ӽڵ�
    for(int i = 0; i < parenItem->rowCount(); i++)
    {
            QStandardItem *item = parenItem->child(i);
            QString tcurPath = qvariant_cast<QString>(item->data());
            if(tcurPath == path){
                curItem = item;
                curPath = qvariant_cast<QString>(curItem->data());
                curIndex = curItem->index();
                curTitle = curIndex.data().toString();
            }
    }
}

