//QTreeView����չ��

#ifndef _MY_TREEVIEW_H_
#define _MY_TREEVIEW_H_

#include <QApplication>
#include <QTreeView>
#include <QList>
#include <QWidget>
#include <QStandardItem>
#include <QMouseEvent>

#include "mytreedelegate.h"
#include "mytreeitemmodel.h"

class MyTreeView : public QTreeView
{
Q_OBJECT
public:
        MyTreeView(QString title = NULL, QWidget *parent = 0);
        ~MyTreeView();
	void         setTreeTitle(QString title); //����tree������
	void         setSubTreeTitle(int subTree,QString title);
        void         addItem(int subTree, QString itemName, QString path, QString icon); //����item
        void         addItemByParentItem(QStandardItem *parenItem, QString itemName, QString uid, QString type, QString icon); //����item
	void         addItems(int subTree, QList<QString> nameList);
	void         delelteItem(int subTree, QString itemName);
        bool         delSubItems(QStandardItem *parenItem);
	void         clearTree(int subTree, QString itemName);//������е�item
	void         enableMouse(bool yesOrNo);  //���������Ӧ
	bool         getMouseStatus();   //�鿴�ǿ���ʹ�����
	QString      getCurTitle();  //��ȡ��ǰ���˫����title
        QString      getCurPath();   //��ȡ��ǰ���˫����·��
	QModelIndex  getCurIndex();
        QStandardItem* getCurItem(); // ��ȡ��ǰ˫���ڵ�
        QPoint       getCurPoint();  // ��ȡ��ǰ���λ��
        QString      getCurUuid();   // ��ȡ��ǰUuId
        QString      getCurType();   // ��ȡ��ǰ�ڵ�����
        void         loadDirs(QString diruId, QStandardItem *curItem);
        void         delSubTree();   // ɾ����ǰ��
        void         setCurItemByUuid(QString uuId, QString type); // ����CurUuuid���õ�ǰѡ����Ԫ��
        void         loadTagByParent(QString tagUuId, QStandardItem *curItem);
private slots:
        void         showChildTree();
signals:
	//���˫��
	void         LBtnDbClk();
        //��굥��
        void         LBtnClk();
        //ѡ��ĳ��Item
        void         RBtnClk();
protected:
	void         mouseDoubleClickEvent(QMouseEvent *event);
        // void         mousePressEvent(QMouseEvent *event);
        void         mouseReleaseEvent(QMouseEvent *event);
        void         dropEvent(QDropEvent *event);
        //void         mouseDoubleClickEvent(QMouseEvent *event);
        //void         mouseMoveEvent(QMouseEvent *event);
        //void         keyPressEvent(QKeyEvent *event);
    //#ifndef QT_NO_DRAGANDDROP
        //void dragMoveEvent(QDragMoveEvent *event);
    //#endif
private:
	int                   numSubTree;
	bool                  mouseStatus;
	QString               treeTitle;
	QString               curTitle; //��ǰ���˫��ѡ������
        QString               curPath;  //��ǰ���˫����·��
        QString               curUuId;  //��ǰUuId
        QString               curType;  //��ǰ�ڵ�����
	QModelIndex           curIndex;
        QStandardItem*        curItem;
        QStandardItemModel    *model;
        QPoint                curPoint;
        MyTreeDelegate * delegate;
        MyTreeItemModel * themodel;

};

#endif
