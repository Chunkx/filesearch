
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
#include "db/tagdao.h"
#include "db/sqlite3.h"

// �쳣���
void TagDao::exception(const QString & message)
{
        QMessageBox::critical(0, tr("SQL Error"), message);
}
// �����ǩ
bool TagDao::insertTag(Tag tag)
{
    QString sql = Database::getSql("mf_insert_tag.sql");
    sql = sql.arg(tag.TAG_GUID,tag.TAG_GROUP_GUID,tag.TAG_NAME,tag.TAG_DESCRIPTION, QString::number(tag.MF_VERSION));
    return Database::execSql(sql);
}
// ɾ����ǩ
bool TagDao::deleteTag(QString tagUuId)
{
    QString sql = Database::getSql("mf_delete_tag.sql");
    sql = sql.arg(tagUuId);
    return Database::execSql(sql);
}

// ���ݸ���ǩȡ���ӱ�ǩ
QList<Tag> TagDao::selectTagsbyParent(const QString & groupUuid)
{
    QString sql = Database::getSql("mf_select_tag_group.sql");
    sql = sql.arg(groupUuid);
    QSqlQuery query = Database::execSelect(sql);

    QList<Tag> returnList;
    while (query.next()){
            Tag field;
            field.TAG_GUID = query.value(0).toString();
            field.TAG_NAME = query.value(1).toString();
            field.TAG_DESCRIPTION = query.value(2).toString();
            field.DT_MODIFIED = query.value(3).toChar();
            field.MF_VERSION = query.value(4).toInt();
            returnList.append(field);
    }
    return returnList;
}

// ���ݱ�ǩuuId��ȡ��ǩ
Tag TagDao::selectTag(const QString & uuid)
{
    QString sql = Database::getSql("mf_select_tag_uuid.sql");
    sql = sql.arg(uuid);
    QSqlQuery query = Database::execSelect(sql);

    while (query.next()){
            Tag field;
            field.TAG_GUID = query.value(0).toString();
            field.TAG_NAME = query.value(1).toString();
            field.TAG_GROUP_GUID = query.value(2).toString();
            field.TAG_DESCRIPTION = query.value(3).toString();
            field.DT_MODIFIED = query.value(4).toChar();
            field.MF_VERSION = query.value(5).toInt();
            return field;
    }
}

// �����ӱ�ǩ
bool TagDao::updateTag(Tag tag){

    // ȡ�����ݿ�ԭ����
    Tag orgTag = selectTag(tag.TAG_GUID);

    QString tagname = orgTag.TAG_NAME;
    QString taggroupuuid = orgTag.TAG_GROUP_GUID;
    QString tagdesp = orgTag.TAG_DESCRIPTION;
    int tagversion = orgTag.MF_VERSION;

    if(! tag.TAG_NAME.isEmpty()){
       tagname = tag.TAG_NAME;
    }
    // �ƶ���������ǩ
    taggroupuuid = tag.TAG_GROUP_GUID;
    if(! tag.TAG_DESCRIPTION.isEmpty()){
       tagdesp = tag.TAG_DESCRIPTION;
    }

    if(tag.MF_VERSION != 0){
       tagversion = tag.MF_VERSION;
    }
    QString sql = Database::getSql("mf_update_tag.sql");
    sql = sql.arg(tagname, taggroupuuid, tagdesp, QString::number(tagversion), tag.TAG_GUID);
    return Database::execSql(sql);
}
