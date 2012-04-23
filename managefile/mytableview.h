#ifndef  _MYTABLEVIEW_H_
#define  _MYTABLEVIEW_H_


#include<QTableView>
#include<QItemDelegate>
#include<QPixmap>
#include<Qpoint>

#include <QApplication>
#include <QWidget>
#include <QStandardItem>
#include <QMouseEvent>


#include "myitemdelegate.h"
#include "mystandarditemmodel.h"


class MyTableView : public QTableView
{
Q_OBJECT
public:
        MyTableView(QWidget * parent=0);
        ~MyTableView(){}
        void buildDocList(QStringList files);
        QPoint getCurPoint();               // ȡ�õ�ǰ�����λ��
        void    enableMouse(bool yesOrNo);  //���������Ӧ
        bool    getMouseStatus();   //�鿴�ǿ���ʹ�����
        QString getCurFilePath();   // ȡ��ѡ���ļ�·��

protected:
        void  leaveEvent (QEvent * event );
        void  updateRow(int row);
        void  wheelEvent(QWheelEvent * event );
        bool  eventFilter(QObject* object,QEvent* event);
        void  mouseMoveEvent(QMouseEvent * event);
        void  resizeEvent(QResizeEvent * event);

        void  mouseDoubleClickEvent(QMouseEvent *event);
        void  mousePressEvent(QMouseEvent *event);
signals:
        //���˫��
        void         LBtnDbClk();
        void         shownotes();
private slots:
       void openInSys();
       void openInTab();
       void openInSysExplore();
       void exportConvert();
       void sendMail();
       void notes();

private:
        MyItemDelegate * delegate;
        MyStandardItemModel * themodel;
        bool    mouseStatus;
        QPoint  curPoint;
        QString  curPath;

        // doc table menu start
        QMenu *cmenu;
        QAction *openInSysAction;
        QAction *openInTabAction;
        QAction *openInSysExploreAction;
        QAction *exportDocAction;
        QAction *emailAsAttachAction;
        QAction *noteOfDocAction;
        QAction *relatedDocAction;
        QAction *tagAction;
        QAction *deleteAction;
        QAction *encodeAction;
        QAction *findAndReplaceAction;
        QAction *selectAllAction;
        QAction *printAction;

        QAction *advancedAction;
        QAction *convertDocAction;
        QAction *combineAction;
        QAction *restoreAction;
        QAction *pprintAction;
        QAction *pformatChangeAction;

        QAction *moveToDirAction;
        QAction *copyToDirAction;
        QAction *optionOfDocTableAction;
        QAction *propAction;

        QAction *office2pdfAction;
        QAction *pdf2officeAction;
        QAction *jpg2pdfAction;
        QAction *ppt2jpgAction;
        // doc table menu end

        void tableContextMenuOpened();
        void initActions();
};
#endif // MYTABLEVIEW_H
