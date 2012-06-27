#include "windows.h"
#include "jni.h"
#include <string>
#include <iostream>
#include <QDebug>
#include <excuteJavaUtil.h>

using namespace std;

//jstring NewJString(JNIEnv *env, char *str);
//string  JStringToCString (JNIEnv *env, jstring str);

bool ExcuteJavaUtil::executeJava()
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
    options[1].optionString = "-Djava.class.path=.;c:\\";

    //������ʾ��Ϣ�����ͣ�ȡֵ��gc��class��jni�����һ��ȡ����Ļ�ֵ֮���ö��Ÿ񿪣���-verbose:gc,class
    //�ò������������۲�C++����JAVA�Ĺ��̣����øò����󣬳�����ڱ�׼����豸�ϴ�ӡ���õ������Ϣ
    options[2].optionString = "-verbose:NONE";

    //���ð汾�ţ��汾����JNI_VERSION_1_1��JNI_VERSION_1_2��JNI_VERSION_1_4
    //ѡ��һ�����㰲װ��JRE�汾����İ汾�ż��ɣ��������JRE�汾һ��Ҫ���ڻ��߸���ָ���İ汾��
    vm_args.version = JNI_VERSION_1_4;
    vm_args.nOptions = 3;
    vm_args.options = options;
    //�ò���ָ���Ƿ���ԷǱ�׼�Ĳ����������JNI_FLASE���������Ǳ�׼����ʱ��JNI_CreateJavaVM�᷵��JNI_ERR
    vm_args.ignoreUnrecognized = JNI_TRUE;
    //����JVM.DLL��̬��
    HINSTANCE hInstance = ::LoadLibraryA("C:\\QtWorksapce\\managefile\\jre\\bin\\client\\jvm.dll");
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
    //����test.Demo�࣬����JAVA���CLASS����
    jclass cls = env->FindClass("test/Demo");
    //�������CLASS�����ȡ�����ʵ��
    jobject obj = env->AllocObject(cls);

    //��ȡ���еķ��������һ�������Ƿ�����ǩ����ͨ��javap -s -p �ļ������Ի��
    jmethodID mid = env->GetMethodID(cls, "append", "(Ljava/lang/String;I)Ljava/lang/String;");
    //������������ö���ķ���  ��������
    char *szTest = "aaaaa";
    jstring arg = NewJString(env, szTest);
    jstring msg = (jstring) env->CallObjectMethod(obj, mid, arg, 12);
    string temp = JStringToCString(env, msg);
    // QDebug << JStringToCString(env, msg);

    QString qstr = QString::fromStdString(temp);

//    str = qstr.toStdString();
//    qstr = QString::fromStdString(str);

    //������������ͷŶ�̬��
    jvm->DestroyJavaVM();
    ::FreeLibrary(hInstance);
    return 0;
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

