
#include <QApplication>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QVariant>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QObject>

#include "db/database.h"
#include "db/notedao.h"
#include "db/sqlite3.h"

// �쳣���
void NoteDao::exception(const QString & message)
{
        QMessageBox::critical(0, tr("SQL Error"), message);
}
// ���뱸ע
bool NoteDao::insertNote(Note note)
{

    QString sql = Database::getSql("mf_insert_note.sql");
    sql = sql.arg(note.NOTE_GUID, note.DOCUMENT_GUID, note.NOTE_CONTENT, note.NOTE_OWNER
                  , QString::number(note.PAGE), note.SHEETPAGE,  QString::number(note.ROW),  QString::number(note.COLUMN));
    return Database::execSql(sql);
}
// ɾ����ע
bool NoteDao::deleteNote(QString noteUuId)
{
    QString sql = Database::getSql("mf_delete_note.sql");
    sql = sql.arg(noteUuId);
    return Database::execSql(sql);
}
// ɾ�����б�ע
bool NoteDao::deleteNoteByDoc(QString docUuId)
{
    QString sql = Database::getSql("mf_delete_note_docuuid.sql");
    sql = sql.arg(docUuId);
    return Database::execSql(sql);
}

// ����Docuuidȡ�ñ�ע
QList<Note> NoteDao::selectNotesbyDocUuId(const QString & docUuid)
{
    QString sql = Database::getSql("mf_select_note_doc.sql");
    sql = sql.arg(docUuid);
    QSqlQuery query = Database::execSelect(sql);

    QList<Note> returnList;
    while (query.next()){
            Note field;
            field.NOTE_GUID = query.value(0).toString();
            field.NOTE_CONTENT = query.value(1).toString();
            field.NOTE_OWNER = query.value(2).toString();
            field.PAGE = query.value(3).toInt();
            field.SHEETPAGE = query.value(4).toString();
            field.ROW = query.value(5).toInt();
            field.COLUMN = query.value(6).toInt();
            field.DT_MODIFIED = query.value(7).toChar();
            returnList.append(field);
    }
    return returnList;
}

// ���ݱ�עuuId��ȡ��ע
Note NoteDao::selectNote(const QString & uuid)
{
    QString sql = Database::getSql("mf_select_note_uuid.sql");
    sql = sql.arg(uuid);
    QSqlQuery query = Database::execSelect(sql);

    while (query.next()){
            Note field;
            field.NOTE_GUID = query.value(0).toString();
            field.NOTE_CONTENT = query.value(1).toString();
            field.NOTE_OWNER = query.value(2).toString();
            field.PAGE = query.value(3).toInt();
            field.SHEETPAGE = query.value(4).toString();
            field.ROW = query.value(5).toInt();
            field.COLUMN = query.value(6).toInt();
            field.DT_MODIFIED = query.value(7).toChar();
            return field;
    }
}

// ���±�ע
bool NoteDao::updateNote(Note note){

    // ȡ�����ݿ�ԭ����
    Note orgNote = selectNote(note.NOTE_GUID);

    QString docUuId = orgNote.DOCUMENT_GUID;
    QString noteContent = orgNote.NOTE_CONTENT;
    QString noteOwner = orgNote.NOTE_OWNER;
    int page = orgNote.PAGE;
    QString sheetpage = orgNote.SHEETPAGE;
    int row = orgNote.ROW;
    int column = orgNote.COLUMN;

    if(! note.DOCUMENT_GUID.isEmpty()){
       docUuId = note.DOCUMENT_GUID;
    }

    if(! note.NOTE_CONTENT.isEmpty()){
       noteContent = note.NOTE_CONTENT;
    }

    if(! note.DOCUMENT_GUID.isEmpty()){
       noteOwner = note.DOCUMENT_GUID;
    }

    if(note.PAGE != 0){
       page = note.PAGE;
    }

    if(! note.SHEETPAGE.isEmpty()){
       sheetpage = note.SHEETPAGE;
    }

    if(note.ROW != 0){
       row = note.ROW;
    }

    if(note.COLUMN != 0){
       column = note.COLUMN;
    }

    QString sql = Database::getSql("mf_update_note.sql");
    sql = sql.arg(docUuId, noteContent, noteOwner, QString::number(page),
                  sheetpage, QString::number(row), QString::number(column), note.NOTE_GUID);
    return Database::execSql(sql);

      return true;
}
