//QTreeView的扩展类

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

#include "mytreedelegate.h"
#include "mytreeitemmodel.h"
#include "db/docdao.h"
#include "jvm.h"
#include "excuteJavaUtil.h"

class IndexFilesObj:public QObject {
    Q_OBJECT
    public:
        IndexFilesObj(){}
    public slots:
       void indexfiles()
       {
//          QList<Doc> docs = DocDao::selectDocsByIndexFlag("0");
//          qDebug()<<"indexFileList size:" << docs.size();
//          ExcuteJavaUtil::indexFiles(docs);
            Jvm jvm;

//           //查找test.Demo类，返回JAVA类的CLASS对象
//           jclass cls = env->FindClass("com/searchlocal/lucene/IndexMaker");
//           //根据类的CLASS对象获取该类的实例
//           jobject obj = env->AllocObject(cls);

//           //获取类中的方法，最后一个参数是方法的签名，通过javap -s -p 文件名可以获得
//           jmethodID mid = env->GetMethodID(cls, "makeindex", "(Ljava/lang/String;Ljava/lang/String;)Z");


//       //        javap -s -p -classpath C:\QtWorksapce\managefile-build-de
//       //        sktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK____\jre\jar\indexFile\ com.searchl
//       //        ocal.lucene.IndexMaker

//           //构造参数并调用对象的方法  中文乱码
//           // char *indexpath = "C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\slfile\\index";


           QString indexpath = "C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\slfile\\index";

           QMap<QString, QString> map;
           map.insert("indexpath",indexpath);
           jvm.invokeMethod("com/searchlocal/lucene/IndexMaker", "makeindex", "(Ljava/lang/String;Ljava/lang/String;)Z", map);
       }
};
class IndexFilesSign:public QObject {
    Q_OBJECT
    public:
        IndexFilesSign(QObject* parent=0):QObject(parent){
        }
   public slots:
        void emitsig()   {
           emit sig();
        }
   signals:
       void sig();
};
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
	void         setTreeTitle(QString title); //设置tree的名称
	void         setSubTreeTitle(int subTree,QString title);
        void         addItem(int subTree, QString itemName, QString path, QString icon); //增加item
        void         addItemByParentItem(QStandardItem *parenItem, QString itemName, QString uid, QString type, QString icon); //增加item
	void         addItems(int subTree, QList<QString> nameList);
	void         delelteItem(int subTree, QString itemName);
        bool         delSubItems(QStandardItem *parenItem);
        void         clearTree(int subTree);//清空所有的item
	void         enableMouse(bool yesOrNo);  //设置鼠标相应
	bool         getMouseStatus();   //查看是可以使用鼠标
	QString      getCurTitle();  //获取当前鼠标双击的title
        QString      getCurPath();   //获取当前鼠标双击的路径
	QModelIndex  getCurIndex();
        QStandardItem* getCurItem(); // 获取当前双击节点
        QPoint       getCurPoint();  // 获取当前点击位置
        QString      getCurUuid();   // 获取当前UuId
        QString      getCurType();   // 获取当前节点类型
        void         loadDirs(QString diruId, QStandardItem *curItem);
        void         delSubTree();   // 删除当前树
        void         setCurItemByUuid(QString uuId, QString type); // 根据CurUuuid设置当前选定的元素
        void         loadTagByParent(QString tagUuId, QStandardItem *curItem);
        //// 根据文件父目录取得子目录树结构
        void         loadDelDirs(QString dirUuId,  QStandardItem *curItem);
        void         showImportDlg();
        void         showExportDlg();

private slots:
        void         showChildTree();
        void         reloadTagTree();
        void         currentItemChanged(const QModelIndex &current, const QModelIndex &previous);
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
	//鼠标双击
	void         LBtnDbClk();
        //鼠标单击
        void         LBtnClk();
//        //选中某个Item
//        void         RBtnClk();
protected:
	void         mouseDoubleClickEvent(QMouseEvent *event);
     //    void         mousePressEvent(QMouseEvent *event);
   //    void         mouseReleaseEvent(QMouseEvent *event);
        void         dropEvent(QDropEvent *event);
        void         mousePressEvent(QMouseEvent *event);
        // 只需改变颜色
        void         changeColor();

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
	QString               curTitle; //当前鼠标双击选的名称
        QString               curPath;  //当前鼠标双击的路径
        QString               curUuId;  //当前UuId
        QString               curType;  //当前节点类型
        QModelIndex           curIndex;
        QStandardItem*        curItem;
        QStandardItemModel    *model;
        QPoint                curPoint;
        MyTreeDelegate * delegate;
        MyTreeItemModel * themodel;

        // 是否显示文件夹下文件
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
