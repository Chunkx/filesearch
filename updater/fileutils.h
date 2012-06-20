/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFileInfo>
#include <QAxObject>
#include <QAxWidget>

//! Various helper functions
namespace FileUtils {

bool copyDirectoryFiles(const QDir &fromDir, const QDir &toDir, bool coverFileIfExist);
// ��Ŀ���ļ�copy��Ŀ��Ŀ¼
bool copyFileToDir(QString fromFile, QString toDir, bool coverFileIfExist);
bool removeDirectory(QString dirName);
void deleteDirectory(QFileInfo fileList);
bool delDirectory(const QDir &fromDir);
int  loadAllFile(QDir dir, QList<QString> fileList);

// Ŀ��CopyĿ¼�ṹ
bool copyDirectory(const QDir &fromDir, const QDir &toDir);
// dat�ļ�����
bool writeFile(QString filepath, QStringList lines);
QStringList readFile(QString filepath);
QStringList readAllDatFile(QString dirpath,  QStringList lines);
int openFile(const QString &filepath);
QString suffix(const QString &filepath);

};

#endif
