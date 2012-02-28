/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFileInfo>

//! Various helper functions
namespace FileUtils {

bool copyDirectoryFiles(const QDir &fromDir, const QDir &toDir, bool coverFileIfExist);
bool removeDirectory(QString dirName);
void deleteDirectory(QFileInfo fileList);
int  loadAllFile(QDir dir, QList<QString> fileList);
// Ŀ��CopyĿ¼�ṹ
bool copyDirectory(const QDir &fromDir, const QDir &toDir);
// dat�ļ�����
bool writeFile(QString filepath, QStringList lines);
QStringList readFile(QString filepath);

};

#endif
