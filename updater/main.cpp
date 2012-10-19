#include <QtGui/QApplication>
#include <QProcess>
#include <QDir>
#include <QTranslator>
#include <QMessageBox>
#include "mainwindow.h"
#include "utils.h"
#include "fileutils.h"
#include "single_application.h"

#define _TIME_ qPrintable (QTime::currentTime ().toString ("hh:mm:ss:zzz"))

void customMessageHandler(QtMsgType type, const char *msg)
 {
        QString txt;
        switch (type) {
         //������Ϣ��ʾ
         case QtDebugMsg:
                 txt = QString("%1: Debug: %2").arg(_TIME_).arg(msg);
                 break;

        //һ���warning��ʾ
         case QtWarningMsg:
                 txt = QString("%1: Warning: %2").arg(_TIME_).arg(msg);
         break;
         //���ش�����ʾ
         case QtCriticalMsg:
                txt = QString("%1: Critical: %2").arg(_TIME_).arg(msg);
         break;
         //����������ʾ
         case QtFatalMsg:
                txt = QString("%1: Fatal: %2").arg(_TIME_).arg(msg);
                abort();
         }

         QString logfile = Utils::getSaveLogPath().append(QDir::separator()).append("mflog.log");
         QFileInfo file(logfile);
         if(file.exists()){
             if(file.size() > 1048576){
                 FileUtils::deleteDirectory(file);
             }
         }

         QFile outFile(logfile);
         outFile.open(QIODevice::WriteOnly | QIODevice::Append);
         QTextStream ts(&outFile);
         ts << txt << endl;
}

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, "7yd2_2d");
    // ����ʵ������
    if (app.isRunning())
    {
        // app.sendMessage("There is an other Solo instance runing."));
        return 0;
    }

    qInstallMsgHandler(customMessageHandler);

    MainWindow w;
    w.hide();

    // �趨�����Լ�����
    QApplication::addLibraryPath("./plugins");
    // ���ô���ͼ��
    app.setWindowIcon(Utils::getIcon("updater.ico"));
    // ����������
    QString lang = QLocale::system().name();
    QTranslator translator;
    QString localTranslator = Utils::getTranslator(lang.left(2));
    translator.load(localTranslator);
    app.installTranslator(&translator);

    // ��������
    int ret = app.exec();
    // ����������
    if (ret == 773) {
        QProcess::startDetached(QDir::currentPath().append(QDir::separator()).append("Solo.exe"), QStringList());
        return 0;
    }
    return ret;
}
