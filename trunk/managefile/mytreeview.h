//QTreeView����չ��

#ifndef _MY_TREEVIEW_H_
#define _MY_TREEVIEW_H_

#include <QApplication>
#include <QTreeView>
#include <QList>
#include <QWidget>
#include <QStandardItem>
#include <QMouseEvent>
#include <QDebug>
#include <QThread>
#include <QMap>
#include <QLabel>

#include "mytreedelegate.h"
#include "mytreeitemmodel.h"
#include "db/docdao.h"
#include "jvm.h"
#include "utils.h"
#include "preferences.h"


class MyTreeView : public QTreeView
{
Q_OBJECT
public:
        MyTreeView(QString title = NULL, QWidget *parent = 0);
        ~MyTreeView();
        enum TREE_FIELD {
            UUID = Qt::UserRole + 1,
            NODE_TYPE = Qt::UserRole + 2

        };
	void         setTreeTitle(QString title); //����tree������
	void         setSubTreeTitle(int subTree,QString title);
        void         addItem(int subTree, QString itemName, QString path, QString icon); //����item
        void         addItemByParentItem(QStandardItem *parenItem, QString itemName, QString uid, QString type, QString icon); //����item
	void         addItems(int subTree, QList<QString> nameList);
	void         delelteItem(int subTree, QString itemName);
        bool         delSubItems(QStandardItem *parenItem);
        void         clearTree(int subTree);//������е�item
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
        //// �����ļ���Ŀ¼ȡ����Ŀ¼���ṹ
        void         loadDelDirs(QString dirUuId,  QStandardItem *curItem);
        void         showImportDlg();
        void         showExportDlg();

private slots:
        void         showChildTree();
        void         reloadTagTree();
        void         tableTree_currentItemChanged();

        void importDlg();
        void exportDlg();

        void createSubDir();
        void delSubDir();
        void renameSubDir();
        void createRootDir();
        void moveDir();
        void sortSubDirs();
        void setShowSubDirDoc();
        void setShowSubTagDoc();
        void properties();

        void newTag();
        void deleteTag();
        void showPropOfTag();
        void renameSubTag();
        void moveToRoot();
        void movetoTag();

        void dropBasket();
signals:
        //��굥��
        void         LBtnClk();
        void         changeSearchState();
protected:
        void         mouseDoubleClickEvent(QMouseEvent *event);
         void         mouseReleaseEvent(QMouseEvent *event);
//        void         dropEvent(QDropEvent *event);
        // ֻ��ı���ɫ
        // void         changeColor();

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
       // QStandardItemModel    *model;
        QPoint                curPoint;
        MyTreeDelegate * delegate;
        MyTreeItemModel * model;

        // �Ƿ���ʾ�ļ������ļ�
        bool isShowDocUnderSub;
        bool isShowDocUnderTag;
        QString m_appName;

        //Doc ContextMenu
        QAction *makeSubDir;
        QAction *moveToDir;

        QAction *delDir;
        QAction *renameDir;

        QAction *importDir;
        QAction *exportDir;

        QAction *subDirSort;
        QAction *showSubDirDoc;
        QAction *protectDir;
        QAction *propOfDir;

        //Root ContextMenu
        QAction *makeRootDir;
        QAction *dirSort;
        QAction *protectRootDir;
        QAction *optionOfDir;

        //Tag ContextMenu
        QAction *makeSubTag;
        QAction *moveToTag;
        QAction *moveToRootTag;
        QAction *delTag;
        QAction *renameTag;
        QAction *showSubDirTag;
        QAction *propOfTag;

        //Root Tag ContextMenu
        QAction *makeTag;

        //Root Basket ContextMenu
        QAction *clearBasket;

        QMenu *contextMenu;


        void treeContextMenuOpened();
        void initActions();

};

#endif
