#pragma once
/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#ifndef IMPORTTABLEDIALOG_H
#define IMPORTTABLEDIALOG_H
#endif

#include <QStandardItemModel>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QSslError>
#include <QEventLoop>

/*! \brief Import data into table using various importer types.
\note XML import requires Qt library at least in the 4.3.0 version.
\author Petr Vanek <petr@scribus.info>
*/
class ReqUtil : public QWidget
{
	Q_OBJECT

	public:
                ReqUtil(QWidget * parent = 0);
                void startRequest(QUrl url);
                QVariantMap getReply();
                bool getError();
        private:
                QNetworkAccessManager *manager;
                QNetworkReply *reply;
                QUrl url;     //�洢�����ַ
                QString m_action;
                bool hasError;
                QEventLoop *loop;
                QByteArray rtnStr;

	private slots:
                void httpFinished();  //������غ�Ĵ���
                void httpReadyRead();  //���յ�����ʱ�Ĵ���
                void slotError(QNetworkReply::NetworkError error);
                //�п�������
                void slotSslErrors(QList<QSslError> temp);
        signals:
               void reqfinished();


};
