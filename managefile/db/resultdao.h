#ifndef ResultDao_H
#define ResultDao_H

#include <QCoreApplication>
#include <QList>

typedef struct
{
        QString KEY_WORD;
        QString FILE_TYPE;
        QString FILE_PATH;
        QString FILE_NAME;
        QString DESP;
        QString CONTENT;
        QString SHEET_NAME;
        int ROW_NB;
        int PAGE;
        QString DT_CREATED;

}Result;

class ResultDao
{
            Q_DECLARE_TR_FUNCTIONS(ResultDao)
        public:
                // ǰ׺
                static QList<Result> selectByPrefix(const QString & keyword);
                // ��׺
                static QList<Result> selectBySuffix(const QString & keyword);
                // �м�
                static QList<Result> selectByMiddle(const QString & keyword);
                // ��ȫ���
                static QList<Result> selectByFullEqual(const QString & keyword);

        private:
                static void exception(const QString & message);
};

#endif
