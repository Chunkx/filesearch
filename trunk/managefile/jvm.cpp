#include "jvm.h"
#include <QDebug>
#include <string>
#include <iostream>
#include <QDir>
#include <utils.h>
#include <db/docdao.h>
#include <db/resultdao.h>
#include <QTextCodec>
#include <QThread>
#include <QMessageBox>

JNIEnv* Jvm::env = NULL;
JavaVM* Jvm::jvm = NULL;

Jvm::~Jvm()
{
}

bool Jvm::BeginJVM()
{

     if(jvm == NULL){
        //����һ������ָ�룬��������ָ��JVM�е�JNI_CreateJavaVM����
        typedef jint (WINAPI *PFunCreateJavaVM)(JavaVM **, void **, void *);
        int ret;
        JavaVMInitArgs vm_args;
        JavaVMOption options[3];

         qDebug() << "ininsize JVM";

        /*���ó�ʼ������*/
        //disable JIT������JNI�ĵ��еĽ��ͣ��������岻�Ǻ���� ����ȡ��ЩֵҲ�������
        //��JNI�ĵ������ʾ�������а������
        options[0].optionString = "-Djava.compiler=NONE";

        //����classpath����������õ��˵�������JAR����Ҳ�������������������
            // char *optionString;
        QString option = "-Djava.class.path=.;";
        option.append(".\\jre\\jar\\bcmail-jdk14-132.jar;");
        option.append(".\\jre\\jar\\bcprov-jdk14-132.jar;");
        option.append(".\\jre\\jar\\commons-collections-3.2.1.jar;");
        option.append(".\\jre\\jar\\commons-logging-1.1.jar;");
        option.append(".\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;");
        option.append(".\\jre\\jar\\htmllexer.jar;");
        option.append(".\\jre\\jar\\htmlparser.jar;");
        option.append(".\\jre\\jar\\indexFile.jar;");
        option.append(".\\jre\\jar\\log4j-1.2.13.jar;");
        option.append(".\\jre\\jar\\lucene-core-3.3.0.jar;");
        option.append(".\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;");
        option.append(".\\jre\\jar\\lucene-highlighter-3.3.0.jar;");
        option.append(".\\jre\\jar\\lucene-memory-3.3.0.jar;");
        option.append(".\\jre\\jar\\nekohtml.jar;");
        option.append(".\\jre\\jar\\paoding-analysis.jar;");
        option.append(".\\jre\\jar\\poi-3.8-beta3-20110606.jar;");
        option.append(".\\jre\\jar\\poi-excelant-3.8-beta3-20110606.jar;");
        option.append(".\\jre\\jar\\poi-ooxml-3.8-beta3-20110606.jar;");
        option.append(".\\jre\\jar\\poi-ooxml-schemas-3.8-beta3-20110606.jar;");
        option.append(".\\jre\\jar\\poi-scratchpad-3.8-beta3-20110606.jar;");
        option.append(".\\jre\\jar\\stax-api-1.0.1.jar;");
        option.append(".\\jre\\jar\\xercesImpl.jar;");
        option.append(".\\jre\\jar\\xmlbeans-2.3.0.jar;");
        option.append(".\\jre\\jar\\dom4j-1.6.1.jar;");
        option.append(".\\jre\\jar\\sendmail.jar;");
        option.append(".\\jre\\jar\\mail-1.4.1.jar;");


        options[1].optionString = "-Djava.class.path=.;.\\jre\\jar\\bcmail-jdk14-132.jar;.\\jre\\jar\\bcprov-jdk14-132.jar;.\\jre\\jar\\commons-collections-3.2.1.jar;.\\jre\\jar\\commons-logging-1.1.jar;.\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;.\\jre\\jar\\htmllexer.jar;.\\jre\\jar\\htmlparser.jar;.\\jre\\jar\\indexFile.jar;.\\jre\\jar\\log4j-1.2.13.jar;.\\jre\\jar\\lucene-core-3.3.0.jar;.\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;.\\jre\\jar\\lucene-highlighter-3.3.0.jar;.\\jre\\jar\\lucene-memory-3.3.0.jar;.\\jre\\jar\\nekohtml.jar;.\\jre\\jar\\paoding-analysis.jar;.\\jre\\jar\\poi-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-excelant-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-schemas-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-scratchpad-3.8-beta3-20110606.jar;.\\jre\\jar\\stax-api-1.0.1.jar;.\\jre\\jar\\xercesImpl.jar;.\\jre\\jar\\xmlbeans-2.3.0.jar;.\\jre\\jar\\dom4j-1.6.1.jar;.\\jre\\jar\\sendmail.jar;.\\jre\\jar\\mail-1.4.1.jar";
        // options[1].optionString = option.toLatin1().data();

        //������ʾ��Ϣ�����ͣ�ȡֵ��gc��class��jni�����һ��ȡ����Ļ�ֵ֮���ö��Ÿ񿪣���-verbose:gc,class
        //�ò������������۲�C++����JAVA�Ĺ��̣����øò����󣬳�����ڱ�׼����豸�ϴ�ӡ���õ������Ϣ
        options[2].optionString = "-verbose:NONE";

        //���ð汾�ţ��汾����JNI_VERSION_1_1��JNI_VERSION_1_2��JNI_VERSION_1_4
        //ѡ��һ�����㰲װ��JRE�汾����İ汾�ż��ɣ��������JRE�汾һ��Ҫ���ڻ��߸���ָ���İ汾��
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 3;
        vm_args.options = options;
        //�ò���ָ���Ƿ���ԷǱ�׼�Ĳ����������JNI_FLASE���������Ǳ�׼����ʱ��JNI_CreateJavaVM�᷵��JNI_ERR
        vm_args.ignoreUnrecognized = JNI_TRUE;
        QString path = QDir::currentPath();
        path.append("\\jre\\bin\\client\\jvm.dll");

        HINSTANCE hInstance = ::LoadLibraryA(path.toLocal8Bit().constData());
        // HINSTANCE hInstance = ::LoadLibraryW(path.toStdWString().c_str());
        if (hInstance == NULL)
        {
            qDebug() << "LoadLibraryW hInstance == NULL";
            hInstance = LoadLibraryEx(path.toStdWString().c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
        }
        if (hInstance == NULL)
        {
            qDebug() << "LoadLibraryEx hInstance == NULL";
            return false;
        }
        //ȡ�������JNI_CreateJavaVM����ָ��
        PFunCreateJavaVM funCreateJavaVM = (PFunCreateJavaVM)::GetProcAddress(hInstance, "JNI_CreateJavaVM");
        //����JNI_CreateJavaVM���������
        ret = (*funCreateJavaVM)(&jvm, (void**)&env, &vm_args);
        if(ret < 0){
            qDebug() << "CreateJVM Created Failed !";
        }else{
            qDebug() << "CreateJVM Created!, jvm Not NULL";
            jvm->AttachCurrentThread((void **) &env, NULL);
        }
     }else {
         qDebug() << "CreateJVM jvm Not NULL";
         jvm->AttachCurrentThread((void **) &env, NULL);
     }
}

// ���Է���
bool Jvm::invokeMethod(QString clz, QString method, QString methodSign, QMap<QString, QString> parms){
    BeginJVM();
    //����test.Demo�࣬����JAVA���CLASS����
    jclass cls = env->FindClass(clz.toLatin1().data());
    //�������CLASS�����ȡ�����ʵ��
    jobject obj = env->AllocObject(cls);

    //��ȡ���еķ��������һ�������Ƿ�����ǩ����ͨ��javap -s -p �ļ������Ի��
    //javap -s -p -classpath C:\QtWorksapce\managefile-build-de
//    sktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK____\jre\jar\sendmail\ com.util.mai
//    l.SendMail
    jmethodID mid = env->GetMethodID(cls, method.toLatin1().data(), methodSign.toLatin1().data());


    if(method == "makeindex"){
        DocDao dao;
        QString idexpath = parms.value("indexpath");
        QString dbpath = parms.value("dbpath");

        QString filepath = parms.value("filepath");
        // ȫ��ִ��
        if(filepath.isEmpty()){
            QList<Doc> docList = dao.selectDocsByIndexFlag("0");
            QByteArray tempindexpath = idexpath.toLocal8Bit();
            char* cindexpath = tempindexpath.data();

            QByteArray tempdbpath = dbpath.toLocal8Bit();
            char* cdbpath = tempdbpath.data();

            jstring arg1 = NewJString(env, cindexpath);
            jstring arg4 = NewJString(env, cdbpath);
            Doc file;
            foreach(file, docList){
                qDebug() << "index file ---" << file.DOCUMENT_LOCATION;

                QByteArray tempfilepath = file.DOCUMENT_LOCATION.toLocal8Bit();
                char* filepath = tempfilepath.data();

                jstring arg2 = NewJString(env, filepath);
                jstring arg3 = NewJString(env, file.DOCUMENT_GUID.toLatin1().data());
                env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4);

                file.DOCUMENT_INDEXFLG = "1";
                DocDao::updateDoc(file);
            }
        }else{
            QString docuuid = parms.value("docuuid");

            QByteArray tempindexpath = idexpath.toLocal8Bit();
            char* cindexpath = tempindexpath.data();

            QByteArray tempdbpath = dbpath.toLocal8Bit();
            char* cdbpath = tempdbpath.data();

            jstring arg1 = NewJString(env, cindexpath);
            jstring arg4 = NewJString(env, cdbpath);

            qDebug() << "index file ---" << filepath;

            QByteArray tempfilepath = filepath.toLocal8Bit();
            char* filepath = tempfilepath.data();

            jstring arg2 = NewJString(env, filepath);
            jstring arg3 = NewJString(env, docuuid.toLatin1().data());
            env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4);

            Doc doc;
            doc.DOCUMENT_GUID = docuuid;
            doc.DOCUMENT_INDEXFLG = "1";
            DocDao::updateDoc(doc);
        }
    }

    if(method == "deleteIndex"){
        QString idexpath = parms.value("indexpath");
        QString dbpath = parms.value("dbpath");
        QString docuuid = parms.value("docuuid");
        QString filepath = parms.value("filepath");


        QByteArray tempindexpath = idexpath.toLocal8Bit();
        char* cindexpath = tempindexpath.data();

        QByteArray tempdbpath = dbpath.toLocal8Bit();
        char* cdbpath = tempdbpath.data();

        jstring arg1 = NewJString(env, cindexpath);
        jstring arg4 = NewJString(env, cdbpath);

        qDebug() << "delete index file ---" << filepath;

        QByteArray tempfilepath = filepath.toLocal8Bit();
        char* cfilepath = tempfilepath.data();

        jstring arg2 = NewJString(env, cfilepath);
        jstring arg3 = NewJString(env, docuuid.toLatin1().data());
        env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4);

        ResultDao dao;
        dao.deleteResultByDocUid(docuuid);
    }

    if(method == "queryAll"){

        QString keyword = parms.value("keyword");
        QString searchType = parms.value("searchtype");
        QString indexpath = parms.value("indexpath");
        QString dbpath = parms.value("dbpath");

        QByteArray tempkeyword = keyword.toLocal8Bit();
        char* ckeyword = tempkeyword.data();

        QByteArray tempindexpath = indexpath.toLocal8Bit();
        char* cindexpath = tempindexpath.data();

        QByteArray tempdbpath = dbpath.toLocal8Bit();
        char* cdbpath = tempdbpath.data();


        jstring arg1 = NewJString(env, searchType.toLatin1().data());
        jstring arg2 = NewJString(env, ckeyword);
        jstring arg3 = NewJString(env, cindexpath);
        jstring arg4 = NewJString(env, cdbpath);


        qDebug() << "searchType>> " << searchType;
        qDebug() << "keyword--> " << ckeyword;
        qDebug() << "indexpath>> " << cindexpath;
        qDebug() << "dbpath>> " << cdbpath;
        env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4);

    }

    if(method == "sendMail"){

        // (String host, String username, String pwd, String fromaddr, String toaddr, String title, String content)

        QString host = "mx1.ourhost.cn";
        QString username = "support@slfile.net";
        QString pwd = "changsong2008";
        QString fromaddr = "support@slfile.net";

        QString toaddr = parms.value("toaddr");
        QString title = parms.value("title");
        QString content = parms.value("content");

        QByteArray temptitle = title.toLocal8Bit();
        char* ctitle = temptitle.data();

        QByteArray tempcontent = content.toLocal8Bit();
        char* ccontent = tempcontent.data();

        jstring arg1 = NewJString(env, host.toLatin1().data());
        jstring arg2 = NewJString(env, username.toLatin1().data());
        jstring arg3 = NewJString(env, pwd.toLatin1().data());
        jstring arg4 = NewJString(env, fromaddr.toLatin1().data());
        jstring arg5 = NewJString(env, toaddr.toLatin1().data());
        jstring arg6 = NewJString(env, ctitle);
        jstring arg7 = NewJString(env, ccontent);

        env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4, arg5, arg6, arg7);

    }
    //EndJVM();
    return true;
}
// ����JVM
bool Jvm::EndJVM()
{
    //�ر�JVM
    jvm->DestroyJavaVM();
    return true;
}
string Jvm::JStringToCString (JNIEnv *env, jstring str)// (jstring str, LPTSTR desc, int desc_len)
{
    if(str == NULL)
    {
        return "";
    }
    //��VC��wchar_t�������洢���ֽ��ַ�(UNICODE)����������
    int len = env->GetStringLength(str);
    wchar_t *w_buffer = new wchar_t[len+1];
    char *c_buffer = new char[2*len+1];
    ZeroMemory(w_buffer,(len+1)*sizeof(wchar_t));
    //ʹ��GetStringChars������GetStringUTFChars
    const jchar * jcharString = env->GetStringChars(str, 0);
    wcscpy(w_buffer,jcharString);
    env->ReleaseStringChars(str,jcharString);
    ZeroMemory(c_buffer,(2*len+1)*sizeof(char));
    //�����ַ�����ת������(Win32 API)��UNICODEתΪASCII�����ʽ�ַ���
    len = WideCharToMultiByte(CP_ACP,0,w_buffer,len,c_buffer,2*len,NULL,NULL);
    string cstr = c_buffer;
    delete[] w_buffer;
    delete[] c_buffer;

    return cstr;
}

jstring Jvm::NewJString(JNIEnv *env, char *str)
{
    if(!env || !str)
    {
        return 0;
    }
    int slen = strlen(str);
    jchar* buffer = new jchar[slen];
    int len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str),buffer,slen);
    if(len>0 && len < slen)
    {
        buffer[len] = 0;
    }
    jstring js = env->NewString(buffer,slen);
    delete [] buffer;
    return js;
}


