#include <QProgressDialog>
#include "ui_doctodirdialog.h"
#include "mytreeview.h"

#include <QStandardItemModel>

class DocToDirDialog : public QDialog, public Ui::DocToDirDialog
{
	Q_OBJECT

	public:
                DocToDirDialog(QWidget * parent = 0,const QString & docName = 0, const QString & docUuid = 0, const bool isCopy = false);
                bool update;
                bool m_isCopy; // true ���� false �ƶ�

                QString m_docName;
                QString m_selDocUuid;
                QString m_selDirUuid;
	private:
                QObject * m_parent;
                MyTreeView *q_myTreeList;

	private slots:
                void confirmBtn_clicked();
                void cancelBtn_clicked();
                void newDirBtn_clicked();

        signals:
                // ȷ����ť���
                void  comfirmBtnClk();

};
