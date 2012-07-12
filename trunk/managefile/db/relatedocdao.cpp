
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
#include "db/RelateDocDao.h"
#include "db/sqlite3.h"
#include "sqlloader.h"

// �쳣���
void RelateDocDao::exception(const QString & message)
{
        QMessageBox::critical(0, tr("SQL Error"), message);
}
// ��������ĵ�
bool RelateDocDao::insertRelateDoc(RelateDoc reldoc)
{
    SqlLoader* sqlLoader = SqlLoader::instance();
    QString sql = sqlLoader->getSql("mf_insert_relatedoc.sql");
    sql = sql.arg(reldoc.DOCUMENT_GUID, reldoc.RELATED_DOCUMENT_GUID);

    return Database::execSql(sql);
}
// ɾ�������ĵ�
bool RelateDocDao::deleteRelateDoc(RelateDoc reldoc)
{
    SqlLoader* sqlLoader = SqlLoader::instance();
    QString sql = sqlLoader->getSql("mf_delete_relatedoc.sql");
    sql = sql.arg(reldoc.DOCUMENT_GUID, reldoc.RELATED_DOCUMENT_GUID);
    return Database::execSql(sql);
}

// ɾ��ĳ���ĵ������й���
bool RelateDocDao::deleteRelateDocByDocUuId(const QString & docUuid)
{
    QString sql;
    sql.append("DELETE FROM MF_DOCUMENT_RELATED WHERE DOCUMENT_GUID='");
    sql.append(docUuid);
    sql.append("'");

    return Database::execSql(sql);
}

// ����Docuuidȡ�ù����ĵ�
QList<RelateDoc> RelateDocDao::selectRelateDocs(const QString & docUuid)
{
    SqlLoader* sqlLoader = SqlLoader::instance();
    QString sql = sqlLoader->getSql("mf_select_relatedoc.sql");
    sql = sql.arg(docUuid);
    QSqlQuery query = Database::execSelect(sql);

    QList<RelateDoc> returnList;
    while (query.next()){
            RelateDoc field;
            field.DOCUMENT_GUID = query.value(0).toString();
            field.RELATED_DOCUMENT_GUID = query.value(1).toString();
            returnList.append(field);
    }
    return returnList;
}
// ����RelatedDocuuidȡ�ù����ĵ�
QList<RelateDoc> RelateDocDao::selectRelatedDocsByRel(const QString & relDocUuid)
{
    SqlLoader* sqlLoader = SqlLoader::instance();
    QString sql = sqlLoader->getSql("mf_select_relatedoc_reldoc.sql");
    sql = sql.arg(relDocUuid);
    QSqlQuery query = Database::execSelect(sql);

    QList<RelateDoc> returnList;
    while (query.next()){
            RelateDoc field;
            field.DOCUMENT_GUID = query.value(0).toString();
            field.RELATED_DOCUMENT_GUID = query.value(1).toString();
            returnList.append(field);
    }
    return returnList;
}

