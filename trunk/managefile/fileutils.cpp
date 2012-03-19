#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <browser/webview.h>
#include <QAxObject>
#include <QAxWidget>

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
        }else{            /**< �������ǲ���ʱ�������ļ�����ɾ������ */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// �����ļ�copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

// ��Ŀ���ļ�copy��Ŀ��Ŀ¼
bool FileUtils::copyFileToDir(QString sfromFile, QString toDir, bool coverFileIfExist)
{
    QDir *targetDir = new QDir (toDir);
    QFileInfo *fileInfo = new QFileInfo(sfromFile);
    /** ���Ŀ��Ŀ¼�����ڣ�����д��� */
    if(!targetDir->exists()){
        if(!targetDir->mkpath(targetDir->absolutePath()))
            return false;
    }

     /**< �������ǲ���ʱ�������ļ�����ɾ������ */
    if(coverFileIfExist && targetDir->exists(fileInfo->fileName())){
        targetDir->remove(fileInfo->fileName());
    }

    /// �����ļ�copy
    if(!QFile::copy(fileInfo->absoluteFilePath(), toDir.append(QDir::separator()).append(fileInfo->fileName()))){
        return false;
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

// д���ļ�
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

// ��ȡ�ļ�
QStringList FileUtils::readFile(QString filepath)
{
      QStringList lines;
      QFile file(filepath);
      if (!file.open( QIODevice::ReadOnly | QIODevice::Text )) {
            return lines;
      }

      QTextStream stream( &file );
      QString line;
      while ( !stream.atEnd() ) {
          line = stream.readLine(); // ��������\n����һ���ı�
          lines << line;
      }
      file.close();
      return lines;

}

// ��ȡĿ¼�����е��ļ�
QStringList FileUtils::readAllDatFile(QString filepath, QStringList lines)
{
      QDir dir(filepath);
      if(!dir.exists()){
          return QStringList();
      }

      QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

      foreach(QFileInfo fileInfo, fileInfoList){
          if(fileInfo.isDir()){
              QString dir = fileInfo.filePath();
              lines.append(readAllDatFile(dir, lines));
          } else if(fileInfo.isFile()){
              QString dir = fileInfo.filePath();
              lines.append(readFile(dir));
          }
      }
      return lines;
}

// ɾ���ļ���
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
        } else if(fileInfo.isFile()){
            QFile tmpFile(fileInfo.fileName());
            dir.remove(tmpFile.fileName()); /**< ɾ����ʱ�ļ� */
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
        if(childCount >2 ){
            for(int i=0; i < childCount; i++){
                if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==("..")){
                    continue;
                }
                deleteDirectory(newFileList.at(i));
            }
        }
        fileList.absoluteDir().rmdir(fileList.fileName());
    } else if(fileList.isFile()){
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
            fileList.append(fileInfo.filePath());
        }//end else
        i++;
    } while(i < list.size());
}

// ���ļ�
int FileUtils::openFile(const QString &filepath){
    // ���ļ�
    QDir file(filepath);
    if (filepath.isEmpty() || !file.exists()) {
        return -1;
    }

    // QDesktopServices::openUrl(QUrl("file:////Users/Biao/Desktop/1.mp4"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));

}

// webView���ı��ļ�
int FileUtils::openTxtFile(const QString &filepath, WebView &webview){
    // ���ļ�
    QFile file(filepath);
    if(!file.exists()){
      return -1;
    }
    QTextStream te(&file);
    webview.setHtml(te.readAll());
}

// webView�򿪲���ȡExcel�ļ�
int FileUtils::readExcelFile(const QString &filepath){

    // �ļ�������
    QFile file(filepath);
    if(!file.exists()){
        return -1;
    }

    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", filepath);
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );

    // excel.querySubObject("ActiveWorkBook");

    QList<QVariantList> data;	//Data list from excel, each QVariantList is worksheet row

    //worksheets count
    int count = sheets->dynamicCall("Count()").toInt();

    for (int i=1; i<=count; i++) //cycle through sheets
    {
            //sheet pointer
            QAxObject* sheet = sheets->querySubObject( "Item( int )", i );

            QAxObject* rows = sheet->querySubObject( "Rows" );
            int rowCount = rows->dynamicCall( "Count()" ).toInt(); //unfortunately, always returns 255, so you have to check somehow validity of cell values
            QAxObject* columns = sheet->querySubObject( "Columns" );
            int columnCount = columns->dynamicCall( "Count()" ).toInt(); //similarly, always returns 65535

            //One of possible ways to get column count
            int currentColumnCount = 0;
            for (int col=1; col<columnCount; col++)
            {
                    QAxObject* cell = sheet->querySubObject( "Cells( int, int )", currentColumnCount, col );
                    QVariant value = cell->dynamicCall( "Value()" );
                    if (value.toString().isEmpty()){
                      break;
                    } else{
                       currentColumnCount = col;
                    }

            }
            columnCount = currentColumnCount;

            //sheet->dynamicCall( "Calculate()" ); //maybe somewhen it's necessary, but i've found out that cell values are calculated without calling this function. maybe it must be called just to recalculate

            for (int row=1; row <= rowCount; row++)
            {
                    QVariantList dataRow;
                    bool isEmpty = true;	//when all the cells of row are empty, it means that file is at end (of course, it maybe not right for different excel files. it's just criteria to calculate somehow row count for my file)
                    for (int column=1; column <= columnCount; column++)
                    {
                            QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row, column );
                            QVariant value = cell->dynamicCall( "Value()" );
                            if (!value.toString().isEmpty() && isEmpty)
                                    isEmpty = false;
                            dataRow.append(value);
                    }
                    if (isEmpty) //criteria to get out of cycle
                            break;
                    data.append(dataRow);
            }
    }

    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    return 0;
}

// webView��Excel�ļ�
int FileUtils::openWordFile(const QString &filepath){

       // �ļ�������
       QFile file(filepath);
       if(!file.exists()){
            return -1;
       }


//    QAxWidget *wordActive = new QAxWidget;
//    wordActive->setControl("Word.Application");
//    QAxObject* newFile = wordActive->querySubObject("Open(const QString&)", filepath);

       QAxWidget word("Word.Application");
       word.setProperty("Visible", true);

       QAxObject * documents = word.querySubObject("Documents");
       documents->dynamicCall("Add (void)");
       QAxObject * document = word.querySubObject("ActiveDocument");
       document->dynamicCall("SaveAs (const QString&)", QString("e:/test/docbyqt.doc"));
       document->dynamicCall("Close (boolean)", false);
       word.dynamicCall("Quit (void)");


      // QAxWidget AxApplication = new QAxWidget(QString::fromUtf8("c:\\temp\\xxx.doc"),0);

}







