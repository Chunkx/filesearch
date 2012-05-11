#ifndef DOCDAO_H
#define DOCDAO_H

#include <QCoreApplication>
#include <QList>

typedef struct
{
        QString DOCUMENT_GUID;
        QString DOCUMENT_TITLE;
        QString DIR_GUID;
        QString DOCUMENT_LOCATION;
        QString DOCUMENT_NAME;
        QString DOCUMENT_SEO;
        QString DOCUMENT_URL;
        QString DOCUMENT_AUTHOR;
        QString DOCUMENT_KEYWORDS;
        QString DOCUMENT_TYPE;
        QString DOCUMENT_OWNER;
        QString DT_CREATED;
        QString DT_MODIFIED;
        QString DT_ACCESSED;
        int DOCUMENT_ICON_INDEX;
        int DOCUMENT_SYNC;
        QString DOCUMENT_PROTECT;
        QString DOCUMENT_ENCODE;
        int DOCUMENT_READ_COUNT;
        int DOCUMENT_RELATE_COUNT;
        QString DOCUMENT_INDEXFLG;
        QString DOCUMENT_OPERFLG;
        QString DELETE_FLAG;
        int MF_VERSION;
}Doc;

class DocDao
{
            Q_DECLARE_TR_FUNCTIONS(DocDao)
        public:
                // �����ǩ
                static bool insertDoc(Doc doc);
                static bool updateDoc(Doc doc);
                static bool deleteDoc(QString docUuId);
                static Doc selectDoc(const QString & docUuId);
                static QList<Doc> selectDocsbyDir(const QString & dirUuid);

        private:
                static void exception(const QString & message);
};

#endif
