#pragma once

#include "ui_notesdialog.h"

#include <QStandardItemModel>
#include <QModelIndex>


/*! \brief Import data into table using various importer types.
\note XML import requires Qt library at least in the 4.3.0 version.
\author Petr Vanek <petr@scribus.info>
*/
class NotesDialog : public QDialog, public Ui::NotesDialog
{
	Q_OBJECT

	public:
                NotesDialog(QWidget * parent = 0,const QString & m_docUuid = 0);
                bool update;
                QStandardItemModel *model;
                QString m_docUuid;
                QModelIndex selIndex;
	private:
                void intNotes();
	private slots:
                void addNote();
                void deleteNote(QModelIndex selIndex);
                void deleteAllNote();
                void contextMenuRequested(const QPoint &position);
        signals:
                void showAddNoteWidget();
};