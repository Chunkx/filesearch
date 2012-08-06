#include "UeSave.h"
#include <QMessageBox>
#include <QWebView>
#include <QWebFrame>
#include <QDebug>
#include <QApplication>
#include <preferences.h>
#include <utils.h>
#include <db/notedao.h>
#include <QUuid>
#include <QApplication>
#include <QTextCodec>

extern QWebView* view;
class QWebFrame;

UeSave::UeSave(QWidget *parent) :
    QLabel(parent)
{
    qDebug()<< "UeSave::UeSave () "<<  endl;
}

UeSave::~UeSave()
{
        qDebug()<<"UeSave::~UeSave ()"<<endl;
}

// ��ҳ��js����
void UeSave::save(QString content, QString plainText)
{
     Preferences* p = Preferences::instance();
     QString selNoteUuid = p->getSelNoteUid();

     // �ж��Ƿ��޸ıʼǻ���׷�ӱʼ�
     QString notesPath = Utils::getLocateNotesPath();
     if(selNoteUuid.isEmpty()){
         selNoteUuid = QUuid::createUuid().toString();
         // ����note��
         Note note;
         note.NOTE_GUID = selNoteUuid;
         //����docUuid ��Ҫ�ж�NotesDialogɾ��ĳһ��ע���,�ٱ༭����ñ�ע��
         //���������ֱ�ӹرոô���
         p->setSelNoteUid(selNoteUuid);

         QString m_docUuid = p->getSelDocUid();
         note.DOCUMENT_GUID = m_docUuid;
         note.NOTE_CONTENT = plainText;
         NoteDao::insertNote(note);
     }else{
         Note note;
         note.NOTE_GUID = selNoteUuid;
         QString m_docUuid = p->getSelDocUid();
         note.DOCUMENT_GUID = m_docUuid;
         note.NOTE_CONTENT = plainText;
         NoteDao::updateNote(note);
     }

     QString filename = notesPath.append(QDir::separator());
     filename.append(selNoteUuid);
     filename.append(".html");

     QFile f( filename );
     if (!f.exists())
     {
         f.open(QIODevice::WriteOnly);
     }else{
         // ɾ���½�
         f.deleteLater();
         f.open(QIODevice::WriteOnly);
     }

     QTextCodec *code= QTextCodec::codecForName("utf8");
     QTextStream t( &f );
     t.setCodec(code); // ����������ñ���

     t << content;
     f.close();

     // ҳ����Ϣ TODO �����Բ�����
     view->page()->mainFrame()->evaluateJavaScript("savesuccess();");
}

