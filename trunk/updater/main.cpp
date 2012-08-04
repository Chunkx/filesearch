#include <QtGui/QApplication>
#include <QProcess>
#include <QDir>
#include <QTranslator>
#include "mainwindow.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.hide();

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
        qDebug() << "start main programme";
        QProcess::startDetached(QDir::currentPath().append("managefile.exe"), QStringList());
        return 0;
    }
    return ret;
}
