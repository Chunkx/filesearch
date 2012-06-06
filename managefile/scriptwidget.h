#ifndef SCRIPTWIDGET_H
#define SCRIPTWIDGET_H

#include <QWidget>
#include "ui_scriptwidget.h"
//#include <QScriptEngine>
//#include <qscriptedit_p.h>
//#include <QScriptEngineDebugger.h>

//#include "HsfScriptingInterface.h"


//#include <qscintilla2/Qt4/Qsci/qscintilla2/Qt4/Qsci/qsciscintilla.h>


//using namespace TextEditor;
#include <QCompleter>

class QsciScintilla;
class QAbstractItemModel;

class scriptwidget : public QWidget
{
	Q_OBJECT

public:
	scriptwidget(QWidget *parent = 0);
	~scriptwidget();
	//static QScriptValue visshape(QScriptContext *context, QScriptEngine *engine);
	
	
	Ui::scriptwidget ui;
	QsciScintilla *textEdit;
	QString gettext();
	QString gettextbyline(int linenumber);
	void settext(QString textval);
	QAbstractItemModel* modelFromFile(const QString& fileName,QCompleter* completer);

//        HsfScriptingInterface* hsfapi;

        QString resultstream;

private:
	
//        QScriptEngine myengine;
//        QScriptEdit *myeditor;
	
	
//        QScriptEngineDebugger *debugger;
	
	
    

	

public slots:

	void makeinteractive_text(bool value);
	void makeinteractive_mouse(bool value);
	void evaluatetext();
	void evaluatetext(QString text);
	void savecode();
	void readcodefile();
	void seteditor();
	void on3dSelectionChanged();
	void toggleinteractivity();

//	void moveEvent( occviewport* widget, QMouseEvent* e  );
//	void clickEvent( occviewport* widget, QMouseEvent* e );
	

	

	

};

#endif // SCRIPTWIDGET_H
