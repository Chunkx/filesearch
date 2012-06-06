#include "scriptwidget.h"

//#include "shapefactory.h"
//#include "ui.h"
//#include "User_AIS.hxx"

//#include "SGMGUI_COMMON.h"
# include <QHBoxLayout>
//#include "AIS_Gauss.hxx"

//#include "parametricsfordummies.h"
//#include <QScriptable>

#include <QFile>
#include <QTextStream>

#include "qscintilla2/Qt4/Qsci/qsciscintilla.h"
#include "qscintilla2/Qt4/Qsci/qscilexerjavascript.h"



#include <qscintilla2/Qt4/Qsci/qsciapis.h>
#include <QFont>
#include <QFontMetrics>
#include <QTextBlock>
#include <QAbstractItemModel>
#include <QStringListModel>
 


scriptwidget::scriptwidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	
	connect(ui.evalbutton, SIGNAL(pressed()), this, SLOT(evaluatetext()));

	// connect widget events to the context wrapper
	/*connect( ui::getInstance()->getWindowController(),       SIGNAL(mouseMoved(occviewport*, QMouseEvent*)), 
		     this,   SLOT  (moveEvent(occviewport*,  QMouseEvent*)) );*/
	// connect widget events to the context wrapper
//	connect( ui::getInstance()->getWindowController(),       SIGNAL(clickEvent(occviewport*, QMouseEvent*)),
//		     this,   SLOT  (clickEvent(occviewport*,   QMouseEvent*)) );
//	// connect widget events to the context wrapper
//	connect( ui::getInstance()->getWindowController(),       SIGNAL(selectionChanged()) ,
//		     this,   SLOT  (onSelectionChanged()) );




//connect(ui.text, SIGNAL(pressed()), this, SLOT(seteditor()));
	
//	myeditor = new QScriptEdit(0); // addtexteditor found it in QT debugger
//	myeditor->show();

	connect( this->ui.interactivecheck,   SIGNAL(stateChanged ( int ) ) , 
		     this,   SLOT  (makeinteractive_text( bool )) );



//    ui.vertlayout->addWidget(myeditor);


	 QString folder = QCoreApplication::applicationDirPath();

	

//	 QCompleter* completer = new QCompleter(myeditor);
//     completer->setModel(modelFromFile(codefilename,completer));
//	 completer->setModelSorting(QCompleter::UnsortedModel);
//     completer->setCaseSensitivity(Qt::CaseInsensitive);
//     completer->setWrapAround(false);
//	 myeditor->setCompleter(completer);


	

        seteditor();

        //readcodefile();

//	hsfapi = new HsfScriptingInterface() ;
////	hsfapi->setparentwidget(this);
//	//myengine.globalObject().setProperty("hsf",myengine.newQObject(hsfapi));
//	QScriptValue myglobal =  myengine.newQObject(hsfapi);
//	myglobal.setPrototype(myengine.globalObject());
//	myengine.setGlobalObject(myglobal);


	//myengine.globalObject().setProperty("hsf", myhsfval); // this another way of adding functions using the function signature

//	QScriptValue frontend = myengine.newQObject(ui.tab);
//	myengine.globalObject().setProperty("gui", frontend);
	
	
	
//	int childcount = ui.sliderset->count();
//	for(int i=0;i<childcount;i++){
//		QSlider* widgetitem = qobject_cast<QSlider*>(ui.sliderset->itemAt(i)->widget());
//		//QLabel* widgetitem2 = qobject_cast<QLabel*>(ui.labelset->itemAt(i)->widget());
		
//		if(widgetitem){
//			QString objname = widgetitem->objectName();
//			QString obj2name = objname + QString("txt");

//			connect(widgetitem,SIGNAL(sliderMoved(int)),this,SLOT(evaluatetext()));
////			frontend.setProperty(objname,myengine.newQObject(widgetitem));
//			//frontend.setProperty(obj2name,myengine.newQObject(widgetitem2));
			
//		}
//	}

	

	

	

}




void scriptwidget::toggleinteractivity()
{


}

void scriptwidget::makeinteractive_text(bool value)
{
//if (value)
//{
//	connect(myeditor, SIGNAL(textChanged()), this, SLOT(evaluatetext())); //make code interactive
//} else {

//	disconnect(myeditor, SIGNAL(textChanged()), this, SLOT(evaluatetext()));

//}


}

void scriptwidget::makeinteractive_mouse(bool value)
{

}


void scriptwidget::on3dSelectionChanged()
{
evaluatetext();
}

//void scriptwidget::moveEvent( occviewport* widget, QMouseEvent* e  )
//{
//	hsfapi->setmousepos(widget->getPoint());
//	evaluatetext();
//}

//void scriptwidget::clickEvent( occviewport* widget, QMouseEvent* e )
//{
//	hsfapi->setmousepos(widget->getPoint());
//	evaluatetext();
//}


QAbstractItemModel* scriptwidget::modelFromFile(const QString& fileName,QCompleter* completer)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly))
         return new QStringListModel(completer);


     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

     QStringList words;

     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         if (!line.isEmpty())
             words << line.trimmed();
     }


     QApplication::restoreOverrideCursor();
 
     return new QStringListModel(words, completer);
 }

void scriptwidget::seteditor()
{
	textEdit = new QsciScintilla();
	

	//textEdit->autoCompleteFromAll();

	QsciLexerJavaScript*  jscript = new QsciLexerJavaScript(textEdit);

 /*   QsciAPIs* api = new QsciAPIs(jscript);
    api->add("hsf.makeline(p1,p2)");
    api->add("hsf.makepoint(x,y,z)");
    api->add("hsf.addshapelist(list,object)");
    api->prepare();*/
 
	//textEdit->setAutoCompletionThreshold(1);


	QFont font;
    font.setFamily("arial");
    font.setFixedPitch(true);
    font.setPointSize(12);
	//font.setStretch(20);
	//font.setBold(true);
	
//	font.setWeight(300);
	font.setStyleStrategy(QFont::StyleStrategy::PreferQuality);

	
	QFontMetrics fm = QFontMetrics(font);

	jscript->setFont(font);
	
	textEdit->setMarginWidth(0, fm.width( "0000" ));
    textEdit->setMarginLineNumbers(0, true);

	textEdit->setEdgeMode(QsciScintilla::EdgeLine);
    textEdit->setEdgeColumn(0);
	textEdit->setEdgeColor(QColor("green"));
	

	
	
	
	textEdit->setLexer(jscript);
	textEdit->setFolding(QsciScintilla::FoldStyle::BoxedTreeFoldStyle);

	//textEdit->setAutoCompletionSource(QsciScintilla::AutoCompletionSource::AcsAll);
	textEdit->setAutoIndent(true);
	textEdit->setBraceMatching(QsciScintilla::BraceMatch::SloppyBraceMatch);
	
	textEdit->setCallTipsStyle(QsciScintilla::CallTipsStyle::CallTipsContext);
	
	textEdit->show();
	textEdit->zoomIn(2);
	ui.vertlayout->addWidget(textEdit);

        // make code interactive
        connect(textEdit, SIGNAL(textChanged()), this, SLOT(evaluatetext()));

	
	
}
scriptwidget::~scriptwidget()
{

}


QString scriptwidget::gettextbyline (int linenumber)
{
//	if (myeditor)
//	{
//	QString lineat = myeditor->document()->findBlockByLineNumber(linenumber).text();
//	return lineat;
//	} else if (textEdit)
//	{
	QString lineat  = textEdit->text(linenumber);
	return lineat;
//	}

}
QString scriptwidget::gettext()
{
//if (myeditor)
//{
//	return myeditor->toPlainText();
//} else if (textEdit)
//{
	return textEdit->text();
//}

}

void scriptwidget::evaluatetext()
{


savecode();

QString linenumbers;
QString text = gettext();

QString newtext = QString("initpart()\n") + text + QString("\n endpart()");
text = newtext;

//debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
//QScriptValue result;

 try
 {
  //result = myengine.evaluate(text);
 }
 catch(...)
 {
	 ui.listener->addItem(QString("Oops!"));
 }


 //if (myengine.hasUncaughtException()) {
     //int line = myengine.uncaughtExceptionLineNumber();
//	 ui.listener->addItem( QString("Oops!:")+ QString::number(line) + QString(" ") + result.toString());
//	 myengine.clearExceptions();
//	 myengine.collectGarbage();
	 
// } else
// {
//	 ui.listener->addItem(result.toString());
 //}


 ui.listener->scrollToBottom();


}





void scriptwidget::evaluatetext(QString text)
{


savecode();

QString linenumbers;


QString newtext = QString("initpart()\n") + text + QString("\n endpart()");
text = newtext;

//debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
//QScriptValue result;

 try
 {
//  result = myengine.evaluate(text);
 }
 catch(...)
 {
	 ui.listener->addItem(QString("Oops!"));
	 resultstream = QString("Oops!");
 }


// if (myengine.hasUncaughtException()) {
     //int line = myengine.uncaughtExceptionLineNumber();
//	 resultstream = QString("Oops!:")+ QString::number(line) + QString(" ") + result.toString();

	 ui.listener->addItem(resultstream);
//	 myengine.clearExceptions();
//	 myengine.collectGarbage();
	 
// } else
// {
//	resultstream = result.toString();
	 ui.listener->addItem(resultstream);
// }


 ui.listener->scrollToBottom();


}




void scriptwidget::savecode()
{

	QString folder = QCoreApplication::applicationDirPath();
	
	QString codefilename = folder + QString("/code.osf");

	QFile data(codefilename);
	if (data.open(QFile::ReadWrite)) {
     QTextStream out(&data);
     out << gettext();
	}
	data.close();
	
}

void scriptwidget::settext(QString textval)
{
//	if (myeditor)
//{
////	myeditor->setPlainText(textval);
//} else if (textEdit)
//{
	return textEdit->setText(textval);
//}
}

void scriptwidget::readcodefile()
{

	QString folder = QCoreApplication::applicationDirPath();
	QString codefilename = folder + QString("/code.osf");

	QFile data(codefilename);
	if (data.open(QFile::ReadOnly)) {
     QTextStream out(&data);
	 settext(out.readAll());
	}
	data.close();
	
}
