#include <QFileDialog>
#include <QMessageBox>

#include <QtDebug>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QDateTime>

#include "logindialog.h"
#include "preferences.h"
#include "utils.h"
#include "db/docdao.h"
#include "db/dirdao.h"

// ��¼����
LoginDialog::LoginDialog(QWidget * parent)
        : QDialog(parent),
          update(false)
{
	setupUi(this);

        manager=new QNetworkAccessManager(this);

        connect(loginBtn, SIGNAL(clicked()), this, SLOT(loginBtn_clicked()));
        connect(registerBtn, SIGNAL(clicked()), this, SLOT(registerBtn_clicked()));
        connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetBtn_clicked()));

        this->setWindowIcon(Utils::getIcon("file_manager.png"));
        this->setWindowTitle(tr("Slfile"));
        tabWidget->setCurrentIndex(0);

        QDir pathDir = Utils::directoryOf("html");
        QString path = pathDir.absolutePath();
        path.append(QDir::separator());
        path.append("welcome");
        path.append(QDir::separator());
        path.append("html");
        path.append(QDir::separator());
        path.append("welcome.html");

        welcome->load(QUrl::fromUserInput(path));
}

// ��¼
void LoginDialog::loginBtn_clicked()
{
    QString surl;
    surl.append("http://www.slfile.net/mf-login.php?username=");
    surl.append(nameemail->text().trimmed());
    surl.append("&password=");
    QString pwd = Utils::getMD5Str(nameemail->text().trimmed());
    surl.append(pwd);
    url= QUrl::fromEncoded(surl.toUtf8());

    QFileInfo info(url.path());

    fileName=info.fileName(); //��ȡ�ļ���
    if (fileName.isEmpty()) {
      fileName = "db.json";  //����ļ���Ϊ�գ���ʹ��db.json
    }else{
       fileName = "dbfm.json";
    }
    file = new QFile(fileName);
    if(file->open(QIODevice::WriteOnly))
    {
        startRequest(url);  //������������
    } else{
          //������ļ�ʧ�ܣ���ɾ��file����ʹfileָ��Ϊ0��Ȼ�󷵻�
          qDebug() << "file open error";
          delete file;
          file = 0;
          return;
    }

    update = true;
    this->close();
}

// ע��
void LoginDialog::registerBtn_clicked()
{

//    $usrname = $_GET['username'];
//	$email = $_GET['email'];
//	$pwd = $_GET['pwd'];
//	$guid = $_GET['guid'];


    QString surl;
    surl.append("http://www.slfile.net/mf-signup.php?username=");
    surl.append(username->text().trimmed());
    surl.append("&pwd=");
    QString spwd = Utils::getMD5Str(pwd->text().trimmed());
    surl.append(spwd);
    surl.append("&email=");
    QString semail = email->text().trimmed();
    surl.append(semail);
    surl.append("&guid=");
    QString guid = QUuid::createUuid().toString();
    surl.append(guid);

    url= QUrl::fromEncoded(surl.toUtf8());

    QFileInfo info(url.path());

    fileName=info.fileName(); //��ȡ�ļ���
    if (fileName.isEmpty()) {
      fileName = "db.json";  //����ļ���Ϊ�գ���ʹ��db.json
    }else{
       fileName = "dbfm.json";
    }
    file = new QFile(fileName);
    if(file->open(QIODevice::WriteOnly))
    {
        startRequest(url);  //������������
    } else{
          //������ļ�ʧ�ܣ���ɾ��file����ʹfileָ��Ϊ0��Ȼ�󷵻�
          qDebug() << "file open error";
          delete file;
          file = 0;
          return;
    }
}

// ����
void LoginDialog::resetBtn_clicked(){


}

//��������
void LoginDialog::startRequest(QUrl url)
{
    reply = manager->get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));  //������ɺ�
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead())); //�п�������ʱ
}

//�п�������
void LoginDialog::httpReadyRead()
{
  if (file){
      file->write((reply->readAll()));
  }
}
// �������
void LoginDialog::httpFinished()
 {
     file->flush();
     file->close();
     reply->deleteLater();
     reply = 0;
     // TODO ����Setting���ʼ��ȡfile��ķ���ֵ �Ƿ���֤ͨ��

     //delete file;
     file = 0;
}




