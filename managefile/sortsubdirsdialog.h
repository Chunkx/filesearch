#include <QProgressDialog>
#include "ui_sortsubdirsdialog.h"
#include "mytreeview.h"

#include <QStandardItemModel>

class SortSubDirsDialog : public QDialog, public Ui::SortSubDirsDialog
{
	Q_OBJECT

	public:
                SortSubDirsDialog(QWidget * parent = 0,const QString & curUuid = 0, const QString & dir = 0);
                bool update;
                QString m_dir;
                QString m_curUuid;
	private:
                QObject * m_parent;

	private slots:
                void confirmBtn_clicked();
                void cancelBtn_clicked();
                // �����ƶ��ļ���
                void moveDown();
                // �����ƶ��ļ���
                void moveUp();
        signals:

};
