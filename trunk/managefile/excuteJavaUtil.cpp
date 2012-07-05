#include "windows.h"
#include "jni.h"
#include <string>
#include <iostream>
#include <QDir>
#include <excuteJavaUtil.h>
#include <utils.h>
#include <QDebug>

using namespace std;

//jstring NewJString(JNIEnv *env, char *str);
//string  JStringToCString (JNIEnv *env, jstring str);

bool ExcuteJavaUtil::indexFiles(QList<Doc> files)
{

    // �ȵ���JNI_GetCreatedJavaVMs()���������0�͵���JNI_CreateJavaVM�����򲻵���

    //����һ������ָ�룬��������ָ��JVM�е�JNI_CreateJavaVM����
    typedef jint (WINAPI *PFunCreateJavaVM)(JavaVM **, void **, void *);
    typedef jint (WINAPI *PFunGetCreatedJavaVMs)(JavaVM **, void **, void *);
    int res;
    JavaVMInitArgs vm_args;
    JavaVMOption options[3];
    JavaVM *jvm;
    JNIEnv *env;
    qDebug() << "indexFiles 3";

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

    options[1].optionString = "-Djava.class.path=.;.\\jre\\jar\\bcmail-jdk14-132.jar;.\\jre\\jar\\bcprov-jdk14-132.jar;.\\jre\\jar\\commons-collections-3.2.1.jar;.\\jre\\jar\\commons-logging-1.1.jar;.\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;.\\jre\\jar\\htmllexer.jar;.\\jre\\jar\\htmlparser.jar;.\\jre\\jar\\indexFile.jar;.\\jre\\jar\\log4j-1.2.13.jar;.\\jre\\jar\\lucene-core-3.3.0.jar;.\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;.\\jre\\jar\\lucene-highlighter-3.3.0.jar;.\\jre\\jar\\lucene-memory-3.3.0.jar;.\\jre\\jar\\nekohtml.jar;.\\jre\\jar\\paoding-analysis.jar;.\\jre\\jar\\poi-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-excelant-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-schemas-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-scratchpad-3.8-beta3-20110606.jar;.\\jre\\jar\\stax-api-1.0.1.jar;.\\jre\\jar\\xercesImpl.jar;.\\jre\\jar\\xmlbeans-2.3.0.jar";
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
    //����JVM.DLL��̬��
    // HINSTANCE hInstance = ::LoadLibraryA("C:\\QtWorksapce\\managefile\\jre\\bin\\client\\jvm.dll");
    HINSTANCE hInstance = ::LoadLibraryA(path.toLocal8Bit().constData());
    if (hInstance == NULL)
    {
        qDebug() << "indexFiles hInstance == NULL";
        return false;
    }
    //ȡ�������JNI_CreateJavaVM����ָ��
    PFunGetCreatedJavaVMs funGetCreatedJavaVMs = (PFunGetCreatedJavaVMs)::GetProcAddress(hInstance, "JNI_GetCreatedJavaVMs");
    // �ȵ���JNI_GetCreatedJavaVMs()���������0�͵���JNI_CreateJavaVM�����򲻵���

    //����JNI_CreateJavaVM���������
    res = (*funGetCreatedJavaVMs)(&jvm, (void**)&env, &vm_args);
    if( res == 0){
        //ȡ�������JNI_CreateJavaVM����ָ��
        PFunCreateJavaVM funCreateJavaVM = (PFunCreateJavaVM)::GetProcAddress(hInstance, "JNI_CreateJavaVM");
        //����JNI_CreateJavaVM���������
        res = (*funCreateJavaVM)(&jvm, (void**)&env, &vm_args);
    }
    if (res < 0)
    {
       return false;
    }
    //����test.Demo�࣬����JAVA���CLASS����
    jclass cls = env->FindClass("com/searchlocal/lucene/IndexMaker");
    //�������CLASS�����ȡ�����ʵ��
    jobject obj = env->AllocObject(cls);

    //��ȡ���еķ��������һ�������Ƿ�����ǩ����ͨ��javap -s -p �ļ������Ի��
    jmethodID mid = env->GetMethodID(cls, "makeindex", "(Ljava/lang/String;Ljava/lang/String;)Z");


//        javap -s -p -classpath C:\QtWorksapce\managefile-build-de
//        sktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK____\jre\jar\indexFile\ com.searchl
//        ocal.lucene.IndexMaker

    //������������ö���ķ���  ��������
    // char *indexpath = "C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\slfile\\index";
    QString indexpath = "C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\slfile\\index";
    jstring arg1 = NewJString(env, indexpath.toLatin1().data());

//    char *filepath = "E:\\MyDocuments\\Qt\\Document+For+QT.doc";
//    QString filepath = "F:\\Document\\����\\Bug���Ľ��б�.xls";
//    jstring arg2 = NewJString(env, filepath.toLatin1().data());

//   // jstring msg = (jstring) env->CallObjectMethod(obj, mid, arg1, arg2);
//    env->CallObjectMethod(obj, mid, arg1, arg2);

    Doc file;
    foreach(file, files){
        // jstring msg = (jstring) env->CallObjectMethod(obj, mid, arg1, arg2);
        qDebug() << "index file ---" << file.DOCUMENT_LOCATION;
        jstring arg2 = NewJString(env, file.DOCUMENT_LOCATION.toLatin1().data());
        env->CallObjectMethod(obj, mid, arg1, arg2);
    }

    // string temp = JStringToCString(env, msg);

    // QDebug << JStringToCString(env, msg);

    //QString qstr = QString::fromStdString(temp);

//    str = qstr.toStdString();
//    qstr = QString::fromStdString(str);

    //������������ͷŶ�̬��
    jvm->DestroyJavaVM();
    ::FreeLibrary(hInstance);
    return true;
}


bool ExcuteJavaUtil::queryIndex(const QString &searchType, const QString &keyword)
{

    //����һ������ָ�룬��������ָ��JVM�е�JNI_CreateJavaVM����
    typedef jint (WINAPI *PFunCreateJavaVM)(JavaVM **, void **, void *);

    int res;
    JavaVMInitArgs vm_args;
    JavaVMOption options[3];
    JavaVM *jvm;
    JNIEnv *env;

    /*���ó�ʼ������*/
    //disable JIT������JNI�ĵ��еĽ��ͣ��������岻�Ǻ���� ����ȡ��ЩֵҲ�������
    //��JNI�ĵ������ʾ�������а������
    options[0].optionString = "-Djava.compiler=NONE";

    //����classpath����������õ��˵�������JAR����Ҳ�������������������
        // char *optionString;
    QString option = "-Djava.class.path=.;";
    option.append(".\\jre\\jar\\commons-collections-3.2.1.jar;");
    option.append(".\\jre\\jar\\commons-logging-1.1.jar;");
    option.append(".\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;");
    option.append(".\\jre\\jar\\indexFile.jar;");
    option.append(".\\jre\\jar\\log4j-1.2.13.jar;");
    option.append(".\\jre\\jar\\lucene-core-3.3.0.jar;");
    option.append(".\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;");
    option.append(".\\jre\\jar\\lucene-highlighter-3.3.0.jar;");
    option.append(".\\jre\\jar\\lucene-memory-3.3.0.jar;");
    option.append(".\\jre\\jar\\nekohtml.jar;");
    option.append(".\\jre\\jar\\paoding-analysis.jar;");
    option.append(".\\jre\\jar\\stax-api-1.0.1.jar;");
    option.append(".\\jre\\jar\\xercesImpl.jar;");
    option.append(".\\jre\\jar\\xmlbeans-2.3.0.jar;");


    // options[1].optionString = "-Djava.class.path=.;.\\jre\\jar\\bcmail-jdk14-132.jar;.\\jre\\jar\\bcprov-jdk14-132.jar;.\\jre\\jar\\commons-collections-3.2.1.jar;.\\jre\\jar\\commons-logging-1.1.jar;.\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;.\\jre\\jar\\htmllexer.jar;.\\jre\\jar\\htmlparser.jar;.\\jre\\jar\\indexFile.jar;.\\jre\\jar\\log4j-1.2.13.jar;.\\jre\\jar\\lucene-core-3.3.0.jar;.\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;.\\jre\\jar\\lucene-highlighter-3.3.0.jar;.\\jre\\jar\\lucene-memory-3.3.0.jar;.\\jre\\jar\\nekohtml.jar;.\\jre\\jar\\paoding-analysis.jar;.\\jre\\jar\\poi-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-excelant-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-ooxml-schemas-3.8-beta3-20110606.jar;.\\jre\\jar\\poi-scratchpad-3.8-beta3-20110606.jar;.\\jre\\jar\\stax-api-1.0.1.jar;.\\jre\\jar\\xercesImpl.jar;.\\jre\\jar\\xmlbeans-2.3.0.jar;.\\jre\\jar\\json-lib-2.3-jdk13.jar;.\\jre\\jar\\commons-lang.jar;.\\jre\\jar\\ezmorph-1.0.4.jar;.\\jre\\jar\\commons-collections.jar;.\\jre\\jar\\commons-beanutils-1.8.0.jar";
     options[1].optionString = "-Djava.class.path=.;.\\jre\\jar\\commons-collections-3.2.1.jar;.\\jre\\jar\\commons-logging-1.1.jar;.\\jre\\jar\\geronimo-stax-api_1.0_spec-1.0.jar;.\\jre\\jar\\indexFile.jar;.\\jre\\jar\\log4j-1.2.13.jar;.\\jre\\jar\\lucene-core-3.3.0.jar;.\\jre\\jar\\lucene-gosen-1.2-dev-ipadic.jar;.\\jre\\jar\\lucene-highlighter-3.3.0.jar;.\\jre\\jar\\lucene-memory-3.3.0.jar;.\\jre\\jar\\nekohtml.jar;.\\jre\\jar\\paoding-analysis.jar;.\\jre\\jar\\stax-api-1.0.1.jar;.\\jre\\jar\\xercesImpl.jar;.\\jre\\jar\\xmlbeans-2.3.0.jar";

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
    //����JVM.DLL��̬��
    // HINSTANCE hInstance = ::LoadLibraryA("C:\\QtWorksapce\\managefile\\jre\\bin\\client\\jvm.dll");
    HINSTANCE hInstance = ::LoadLibraryA(path.toLocal8Bit().constData());
    if (hInstance == NULL)
    {
        return false;
    }
    //ȡ�������JNI_CreateJavaVM����ָ��
    PFunCreateJavaVM funCreateJavaVM = (PFunCreateJavaVM)::GetProcAddress(hInstance, "JNI_CreateJavaVM");
    //����JNI_CreateJavaVM���������
    res = (*funCreateJavaVM)(&jvm, (void**)&env, &vm_args);
    if (res < 0)
    {
        return false;
    }
    // ����test.Demo�࣬����JAVA���CLASS����
    jclass cls = env->FindClass("com/searchlocal/lucene/ContentSearcher");
    // �������CLASS�����ȡ�����ʵ��
    jobject obj = env->AllocObject(cls);

    //��ȡ���еķ��������һ�������Ƿ�����ǩ����ͨ��javap -s -p �ļ������Ի��
    jmethodID mid = env->GetMethodID(cls, "queryAll", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");


//        javap -s -p -classpath C:\QtWorksapce\managefile-build-de
//        sktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK____\jre\jar\indexFile\ com.searchl
//        ocal.lucene.ContentSearcher

    //������������ö���ķ���  ��������
    // char *indexpath = "C:\\Documents and Settings\\Administrator\\Local Settings\\Application Data\\slfile\\index";

    jstring arg1 = NewJString(env, searchType.toLatin1().data());
    jstring arg2 = NewJString(env, keyword.toLatin1().data());

    QString indexpath = Utils::getLocateIndexPath();
    jstring arg3 = NewJString(env, indexpath.toLatin1().data());

    QString dbpath = Utils::getLocateDbPath();
    dbpath.append(QDir::separator()).append("MF");
    jstring arg4 = NewJString(env, dbpath.toLatin1().data());

    qDebug() << "searchType>> " << searchType;
    qDebug() << "keyword>> " << keyword;
    qDebug() << "indexpath>> " << indexpath;
    qDebug() << "dbpath>> " << dbpath;
    env->CallObjectMethod(obj, mid, arg1, arg2, arg3, arg4);
//    qDebug() << "temp>> " << msg;
//    string temp = JStringToCString(env, msg);

//    QString jsonresult = QString::fromStdString(temp);
//    qDebug() << "jsonresult>> " << jsonresult;

    //������������ͷŶ�̬��
//    jvm->DestroyJavaVM();
//    ::FreeLibrary(hInstance);
    return true;
}

string ExcuteJavaUtil::JStringToCString (JNIEnv *env, jstring str)// (jstring str, LPTSTR desc, int desc_len)
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

jstring ExcuteJavaUtil::NewJString(JNIEnv *env, char *str)
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
    jstring js = env->NewString(buffer,len);
    delete [] buffer;
    return js;
}

