#ifndef RelateDocDAO_H
#define RelateDocDAO_H

#include <QCoreApplication>
#include <QList>

typedef struct
{
        QString DOCUMENT_GUID;
        QString RELATED_DOCUMENT_GUID;
}RelateDoc;


class RelateDocDao
{
            Q_DECLARE_TR_FUNCTIONS(RelateDocDao)
        public:
                // ��������ĵ���ϵ
                static bool insertRelateDoc(RelateDoc reldoc);
                static bool deleteRelateDoc(RelateDoc reldoc);
                static QList<RelateDoc> selectRelateDocs(const QString & docUuid);
                static QList<RelateDoc> selectRelatedDocsByRel(const QString & relDocUuid);
                // ɾ��ĳ���ĵ������й���
                static bool deleteRelateDocByDocUuId(const QString & docUuid);

        private:
                static void exception(const QString & message);
};

#endif
