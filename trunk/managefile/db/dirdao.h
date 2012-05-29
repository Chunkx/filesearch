#ifndef DIRDAO_H
#define DIRDAO_H

#include <QCoreApplication>
#include <QList>

typedef struct
{
        QString DIR_GUID;
        QString DIR_PARENT_UUID;
        QString DIR_NAME;
        QString DIR_DESCRIPTION;
        QString DIR_ICON;
        int DIR_ORDER;
        QString  DT_MODIFIED;
        int MF_VERSION;
        QString DELETE_FLAG;
}Dir;



class DirDao
{
            Q_DECLARE_TR_FUNCTIONS(DirDao)
            //static QList<Dir> selDirList;
        public:

                // ����Ŀ¼
                static bool insertDir(Dir Dir);
                static bool updateDir(Dir Dir);
                static bool deleteDir(QString dirUuId);
                // �����ļ���Ϊ���ļ���
                static bool updateToRootDir(QString dirUuId);
                // �ָ��ļ���
                bool restoreDir(QString dirUuId);
                static Dir selectDir(const QString & uuid);
                static QList<Dir> selectDirsbyParent(QString groupUuid,  QString delFlg);
                // �ݹ������Ŀ¼ȡ�����в㼶��Ŀ¼
                static void selectAllSubDirbyDir(QList<Dir> & subDirList, QString dirUuid, QString delFlg);
                // �ݹ����Ŀ¼ȡ�����в㼶��Ŀ¼
                static void selectAllParentDirbyDir(QList<Dir> & parentDirList,  QString dirUuid);
                // ����ɾ���ļ���
                static bool  physicalDelDir();

                //static void clearSelDirList();
                //static QList<Dir> getSelDirList();

        private:
                static void exception(const QString & message);
};

#endif
