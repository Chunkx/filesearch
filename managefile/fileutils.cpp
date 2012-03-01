/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#include <QIcon>
#include <QPixmapCache>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "fileutils.h"

// Ŀ��Copy
bool FileUtils::copyDirectoryFiles(const QDir &fromDir, const QDir &toDir, bool coverFileIfExist)
{
    QDir sourceDir = fromDir;
    QDir targetDir = toDir;
    if(!targetDir.exists()){    /**< ���Ŀ��Ŀ¼�����ڣ�����д��� */
        if(!targetDir.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< ��ΪĿ¼ʱ���ݹ�Ľ���copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
                return false;
        }
        else{            /**< �������ǲ���ʱ�������ļ�����ɾ������ */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// �����ļ�copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

// Ŀ��CopyĿ¼�ṹ
bool FileUtils::copyDirectory(const QDir &fromDir, const QDir &toDir)
{
    QDir sourceDir = fromDir;
    QDir targetDir = toDir;
    /**< ���Ŀ��Ŀ¼�����ڣ�����д��� */
    if(!targetDir.exists()){
        if(!targetDir.mkdir(toDir.absolutePath())){
          return false;
        }
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == ".."){
             continue;
        }
         /**< ��ΪĿ¼ʱ���ݹ�Ľ���copy */
        if(fileInfo.isDir()){
            if(!copyDirectory(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
                 return false;
            }
        }
    }
    return true;
}

// ɾ����Ŀ¼
bool FileUtils::delDirectory(const QDir &fromDir)
{
    QDir sourceDir = fromDir;
    /** ���Ŀ��Ŀ¼�����ڣ�����д��� */
    if(!fromDir.exists()){
        return true;
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    if(fileInfoList.size() == 0){
        sourceDir.rmpath(fromDir.absolutePath());
    }
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == ".."){
             continue;
        }
         /** ��ΪĿ¼ʱ���ݹ�Ľ���copy */
        if(fileInfo.isDir()){
            if(!delDirectory(fileInfo.filePath())){
                 return false;
            }
        }
    }
    return true;
}

bool FileUtils::writeFile(QString filepath, QStringList lines)
{

    QFile file(filepath);
    if (!file.open( QIODevice::Append | QIODevice::Text )) {
        return false;
    }

     QTextStream stream( &file );
     for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it ){
       stream << *it << "\n";
     }
     file.close();
     return true;
}

QStringList FileUtils::readFile(QString filepath)
{
      QStringList lines;
      QFile file(filepath);
      if (!file.open( QIODevice::ReadOnly | QIODevice::Text )) {
            return lines;
      }

      QTextStream stream( &file );
      int n = 1;
      QString line;
      while ( !stream.atEnd() ) {
          line = stream.readLine(); // ��������\n����һ���ı�
          printf( "%3d: %s\n", n++, line);
          lines << line;
      }
      file.close();
      return lines;

}

bool FileUtils::removeDirectory(QString dirName)
{
    QDir dir(dirName);
    QString tmpdir = "";
    if(!dir.exists()){
        return false;
    }

    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){
            tmpdir = dirName + ("/") + fileInfo.fileName();
            removeDirectory(tmpdir);
            dir.rmdir(fileInfo.fileName()); /**< �Ƴ���Ŀ¼ */
        }
        else if(fileInfo.isFile()){
            QFile tmpFile(fileInfo.fileName());
            dir.remove(tmpFile.fileName()); /**< ɾ����ʱ�ļ� */
        }
        else{
            ;
        }
    }

    /**< �����ϼ�Ŀ¼����Ϊֻ�з����ϼ�Ŀ¼���ſ���ɾ�����Ŀ¼ */
    dir.cdUp();
    if(dir.exists(dirName)){
        if(!dir.rmdir(dirName))
            return false;
    }
    return true;
}

// ɾ���ļ����������ļ�
void FileUtils::deleteDirectory(QFileInfo fileList){

    if(fileList.isDir()){
        int childCount =0;
        QString dir = fileList.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount>2){
            for(int i=0;i<childCount;i++){
                if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==("..")){
                    continue;
                }
                deleteDirectory(newFileList.at(i));
            }
        }
        fileList.absoluteDir().rmpath(fileList.fileName());
    }else if(fileList.isFile()){
        fileList.absoluteDir().remove(fileList.fileName());
    }
}

// ȡ���ļ�����������ļ�
int FileUtils::loadAllFile(QDir dir, QList<QString> fileList){
    //Ŀ¼
    if (!dir.exists()) {
        return -1;
    }

    // ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //�ļ�������
    dir.setSorting(QDir::DirsFirst);

    //ת����һ��list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return -1;
    }

    int i=0;
    //�ݹ��㷨�ĺ��Ĳ���
    do{
        QFileInfo fileInfo = list.at(i);
        //������ļ��У��ݹ�
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            loadAllFile(QDir(fileInfo.filePath()), fileList);
        } else{
            //bool isDat = fileInfo.fileName().endsWith(".dat");
            //if (isDat== true) {
              //  q_myTreeList->addItem(0, fileInfo.fileName());
            //}
            fileList.append(fileInfo.filePath());
        }//end else
        i++;
    } while(i < list.size());
}
