
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
#include "db/dirdao.h"
#include "db/sqlite3.h"

// �쳣���
void DirDao::exception(const QString & message)
{
        QMessageBox::critical(0, tr("SQL Error"), message);
}
// �����ļ���
bool DirDao::insertDir(Dir dir)
{
    QString sql = Database::getSql("mf_insert_dir.sql");
    sql = sql.arg(dir.DIR_GUID, dir.DIR_PARENT_UUID, dir.DIR_NAME, dir.DIR_DESCRIPTION, dir.DIR_ICON,
                   QString::number(dir.DIR_ORDER), QString::number(dir.MF_VERSION), dir.DELETE_FLAG);
    return Database::execSql(sql);
}
// ɾ���ļ���
bool DirDao::deleteDir(QString dirUuId)
{
    QString sql = Database::getSql("mf_delete_dir.sql");
    sql = sql.arg("1", dirUuId);
    return Database::execSql(sql);
}
// �����ļ���Ϊ���ļ���
bool DirDao::updateToRootDir(QString dirUuId)
{
    QString sql;
    sql.append("UPDATE MF_DIR SET DIR_PARENT_UUID=''  WHERE DIR_GUID='");
    sql.append(dirUuId);
    sql.append("'");
    return Database::execSql(sql);
}
// �ָ��ļ���
bool DirDao::restoreDir(QString dirUuId)
{
    QString sql = Database::getSql("mf_delete_dir.sql");
    sql = sql.arg("0", dirUuId);
    return Database::execSql(sql);
}

// ���ݸ�Ŀ¼ȡ����Ŀ¼
QList<Dir> DirDao::selectDirsbyParent(QString groupUuid, QString delFlg)
{
    QString sql = Database::getSql("mf_select_dir_parent.sql");
    sql = sql.arg(groupUuid, delFlg);


    QSqlQuery query = Database::execSelect(sql);

    QList<Dir> returnList;
    while (query.next()){
            Dir field;
            field.DIR_GUID = query.value(0).toString();
            field.DIR_NAME = query.value(1).toString();
            field.DIR_DESCRIPTION = query.value(2).toString();
            field.DIR_ICON = query.value(3).toString();
            field.DIR_ORDER = query.value(4).toInt();
            field.DT_MODIFIED = query.value(5).toString();
            field.MF_VERSION = query.value(6).toInt();
            field.DELETE_FLAG = query.value(7).toString();
            returnList.append(field);
    }
    return returnList;
}

// �ݹ������Ŀ¼ȡ�����в㼶��Ŀ¼
void DirDao::selectAllSubDirbyDir(QList<Dir> & selDirList, QString dirUuid, QString delFlg){

    QList<Dir> tmpList = DirDao::selectDirsbyParent(dirUuid, delFlg);
    selDirList.append(tmpList);
    if(tmpList.size() > 0){
        do
        {
            Dir first = (Dir)tmpList.takeFirst();
            selectAllSubDirbyDir(selDirList, first.DIR_GUID, delFlg);

        }while(tmpList.size() != 0);
    }
}

// �ݹ����Ŀ¼ȡ�����в㼶��Ŀ¼
void DirDao::selectAllParentDirbyDir(QList<Dir> & parentDirList, QString dirUuid){

    Dir tempDir = DirDao::selectDir(dirUuid);

    parentDirList.append(tempDir);
    if(tempDir.DIR_PARENT_UUID != ""){
       selectAllParentDirbyDir(parentDirList, tempDir.DIR_PARENT_UUID);
    }
}

// ����ɾ���ļ���
bool DirDao::physicalDelDir(){
    QString sql;
    sql.append("DELETE FROM MF_DIR WHERE DELETE_FLAG='1'");
    return Database::execSql(sql);
}

// ���ݱ�ǩuuId��ȡ��ǩ
Dir DirDao::selectDir(const QString & uuid)
{
    QString sql = Database::getSql("mf_select_dir_uuid.sql");
    sql = sql.arg(uuid);
    QSqlQuery query = Database::execSelect(sql);

    while (query.next()){
            Dir field;
            field.DIR_GUID = query.value(0).toString();
            field.DIR_PARENT_UUID = query.value(1).toString();
            field.DIR_NAME = query.value(2).toString();
            field.DIR_DESCRIPTION = query.value(3).toString();
            field.DIR_ICON = query.value(4).toString();
            field.DIR_ORDER = query.value(5).toInt();
            field.DT_MODIFIED = query.value(6).toString();
            field.MF_VERSION = query.value(7).toInt();
            field.DELETE_FLAG = query.value(8).toString();
            return field;
    }
}

// ����Ŀ¼
bool DirDao::updateDir(Dir dir){

    // ȡ�����ݿ�ԭ����
    Dir orgDir = selectDir(dir.DIR_GUID);

    // ȡ�����ݿ���ڵ�Ŀ¼
    QString dirparentuuid = orgDir.DIR_PARENT_UUID;
    QString dirname = orgDir.DIR_NAME;
    QString dirdesp = orgDir.DIR_DESCRIPTION;
    QString diricon = orgDir.DIR_ICON;
    int dirorder = orgDir.DIR_ORDER;
    int version = orgDir.MF_VERSION;
    QString delflg = orgDir.DELETE_FLAG;

    if(! dir.DIR_PARENT_UUID.isEmpty()){
       dirparentuuid = dir.DIR_DESCRIPTION;
    }

    if(! dir.DIR_NAME.isEmpty()){
       dirname = dir.DIR_NAME;
    }

    if(! dir.DIR_DESCRIPTION.isEmpty()){
       dirdesp = dir.DIR_DESCRIPTION;
    }

    if(! dir.DIR_ICON.isEmpty()){
       diricon = dir.DIR_ICON;
    }

    if(dir.DIR_ORDER != 0){
       dirorder = dir.DIR_ORDER;
    }

    if(dir.MF_VERSION != 0){
       version = dir.MF_VERSION;
    }

    if(dir.DELETE_FLAG != 0){
       delflg = dir.DELETE_FLAG;
    }

    QString sql = Database::getSql("mf_update_dir.sql");
    sql = sql.arg(dirparentuuid, dirname, dirdesp, diricon, QString::number(dirorder), QString::number(version), delflg, dir.DIR_GUID);
    return Database::execSql(sql);
}

//void DirDao::clearSelDirList(){
//    selDirList.clear();
//}

//QList<Dir> DirDao::getSelDirList(){
//  return selDirList;
//}
