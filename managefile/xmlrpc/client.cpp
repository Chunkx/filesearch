#include "client.h"
#include "xmlrpcclient.h"
#include "utils.h"
#include "preferences.h"

#include <QVariantList>

//#define ApiMethodName_ClientLogin "accounts.clientLogin";
//#define ApiMethodName_ClientLogout  "accounts.clientLogout";
//#define ApiMethodName_CreateAccount  "accounts.createAccount";
//#define ApiMethodName_GetAllCategories  "category.getAll";
//#define ApiMethodName_GetAllTags "tag.getAll";
//#define ApiMethodName_DownloadDocumentList  "document.getSimpleList";
//#define ApiMethodName_DocumentsByCategory  "document.getSimpleListByCategory";
//#define ApiMethodName_DocumentsByTag  "document.getSimpleListByTag";


//#define ApiMethodName_DownloadData "data.download";
//#define ApiMethodName_UploadData "data.upload";


//#define ApiMethodName_DownloadDeletedList "deleted.getList";
//#define ApiMethodName_DocumentsByKey "document.getSimpleListByKey";
//#define ApiMethodName_UploadDeletedList "deleted.postList";

//#define ApiMethodName_UploadDocumentInfo "document.postSimpleData";
//#define ApiMethodName_UploadTags "tag.postList";
//#define ApiMethodName_GetVersion "wiz.getVersion";

//#define ApiMethodName_UploadTags "note.postList";
//#define ApiMethodName_UploadTags "tag.postList";

Client::Client( const QString &address, quint16 port, QObject *parent )
            : QObject( parent ), address( address ), port( port )
{
}

Client::~Client() {}

//user_guid, user_id, user_email, upassword

// �����˺�
void  Client::createAccount(const QString &user_guid, const QString &user_id, const QString &user_email, const QString &password){
    QVariantMap m;
    addCommonParam(m);
    m["user_guid"] = user_guid;
    m["user_id"] = user_id;
    m["user_email"] = user_email;
    m["password"] = password;

    m["product_name"] = "win";
    m["display_name"] = "";
    m["invite_code"] = "";

    QString os = Utils::getSysOs();
    QString lang = Utils::getSysLang();
    m["os"] = os;
    m["lang"] = lang;

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onCreateAccountDataReady(QVariant) ) );
    client->execute("accounts.createAccount", QVariantList() << QVariant( m )  );
}
// ��½����ȡToken
void  Client::clientLogin(const QString &accountUserId,const QString &password){
    QVariantMap m;
    addCommonParam(m);
    m["user_id"] = accountUserId;
    m["password"] = password;

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onClientLoginDataReady(QVariant) ) );
    client->execute("accounts.clientLogin", QVariantList() << QVariant( m ) );
}
// �˳�ϵͳ
void  Client::clientLogout(const QString &accountUserId,const QString &token){
    QVariantMap m;
    addCommonParam(m);
    m["user_id"] = accountUserId;
    m["token"] = token;

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onClientLogoutDataReady(QVariant) ) );
    client->execute("accounts.clientLogout", QVariantList() << QVariant( m ) );
}
// �ı�����
void Client::changePwd(const QString &oldPwd,const QString &newPwd){
    QVariantMap m;
    addCommonParam(m);
    // ���ʼ���ΪUer_id
    Preferences* p = Preferences::instance();
    m["user_id"] = p->getUserEmail();

    m["old_password"] = oldPwd;
    m["new_password"] = newPwd;

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onChangePwdDataReady(QVariant) ) );
    client->execute("accounts.changePassword", QVariantList() << QVariant( m ) );
}

// ȡ�������ļ���
void  Client::getAllDir(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetAllDirDataReady(QVariant) ) );
    client->execute("dir.getAll", QVariantList() << QVariant( m ) );
}

// ȡ�����б�ǩ
void  Client::getAllTag(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetAllTagDataReady(QVariant) ) );
    client->execute("tag.getAll", QVariantList() << QVariant( m ) );
}

// ȡ�����м򵥵��ĵ�
void  Client::getSimpleList(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetSimpleListDataReady(QVariant) ) );
    client->execute("document.getSimpleList", QVariantList() << QVariant( m ) );
}
// ȡ��Ŀ¼�����е��ĵ�
void  Client::getSimpleListByDir(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetSimpleListByDirDataReady(QVariant) ) );
    client->execute("document.getSimpleListByDir", QVariantList() << QVariant( m ) );
}
// ȡ����ǩ�����е��ĵ�
void  Client::getSimpleListByTag(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetSimpleListByTagDataReady(QVariant) ) );
    client->execute("document.getSimpleListByTag", QVariantList() << QVariant( m ) );
}
// ����
void  Client::download(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onDownloadDataReady(QVariant) ) );
    client->execute("data.download", QVariantList() << QVariant( m ) );
}
// �ϴ�
void  Client::upload(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onUploadDataReady(QVariant) ) );
    client->execute("data.upload", QVariantList() << QVariant( m ) );
}

//#define ApiMethodName_UploadDocumentInfo "document.postSimpleData";
//#define ApiMethodName_UploadTags "tag.postList";
//#define ApiMethodName_GetVersion "wiz.getVersion";

//#define ApiMethodName_UploadTags "note.postList";
//#define ApiMethodName_UploadTags "tag.postList";
// �ύ�����ĵ������Ϣ
void  Client::postDocSimpleData(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onPostDocSimpleDataReady(QVariant) ) );
    client->execute("document.postSimpleData", QVariantList() << QVariant( m ) );
}
// �ύ�����ĵ��ʼ������Ϣ
void  Client::postNoteList(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onPostNoteListDataReady(QVariant) ) );
    client->execute("note.postList", QVariantList() << QVariant( m ) );
}
// �ύ���±�ǩ�����Ϣ
void  Client::postTagList(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onPostTagListDataReady(QVariant) ) );
    client->execute("tag.postList", QVariantList() << QVariant( m ) );
}

// �ύ���±�ǩ�����Ϣ
void  Client::getVersion(){
    QVariantMap m;
    addCommonParam(m);

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady(QVariant) ), this, SLOT( onGetVersionDataReady(QVariant) ) );
    client->execute("mf.getVersion", QVariantList() << QVariant( m ) );
}

// ------------------------��ͬ����-----------------------------------//
// ���빲ͬ����
void Client::addCommonParam(QVariantMap &param){
    param["client_type"] = "win";
    param["program_type"] = "normal";
    param["api_version"] = "1";

    if (!autoken.token.isEmpty()) {
        param["token"] = autoken.token;
    }
    if (!autoken.dbGuid.isEmpty()) {
        param["db_guid"] = autoken.dbGuid;
    }
    if (!autoken.userGuid.isEmpty()) {
        param["user_guid"] = autoken.userGuid;
    }
}
// �Ƿ񷵻�����
bool Client::isSuccess()
{
    QVariantMap map = revalue.value<QVariantMap>();
    QString returnCode = map["return_code"].value<QString>();
    return returnCode == "200";
}

// ------------------------���غ���-----------------------------------//
// ��½ ����ɹ�������Token
void Client::onClientLoginDataReady( const QVariant &response )
{
    qDebug() << "onClientLoginDataReady";
    qDebug() << response;
    revalue = response;

    QVariantMap map = revalue.value<QVariantMap>();
    qDebug() << map["api_version"];
    qDebug() << map["db_guid"];
    qDebug() << map["token"];
    autoken.token = map["token"].value<QString>();
    autoken.dbGuid = map["db_guid"].value<QString>();

    // ȡ���û���Ϣ
    userMap = map["user"].value<QVariantMap>();
    QString userGuid = userMap["user_guid"].value<QString>();
    autoken.userGuid = userGuid;

    // ���������Ѿ�׼������Ϣ
    emit clientLoginDataReady();
}
// �����˺�
void Client::onCreateAccountDataReady( const QVariant &response )
{
    qDebug() << "onCreateAccountDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit createAccountDataReady();
}
// �ı�����
void Client::onChangePwdDataReady( const QVariant &response )
{
    qDebug() << "onChangePwdDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit changePwdDataReady();
}
// ȡ�ð汾
void Client::onGetVersionDataReady( const QVariant &response ){
    qDebug() << "onGetVersionDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getVersionDataReady();
}
// �ύ��ǩ��Ϣ
void Client::onPostTagListDataReady( const QVariant &response ){
    qDebug() << "onPostTagListDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit postTagListDataReady();
}
// �ύ�ʼ���Ϣ
void Client::onPostNoteListDataReady( const QVariant &response ){
    qDebug() << "onPostNoteListDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit postNoteListDataReady();
}
// �ύ �����ĵ�����Ϣ
void Client::onPostDocSimpleDataReady( const QVariant &response ){
    qDebug() << "onPostDocSimpleDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit postDocSimpleDataReady();
}
// �ϴ�
void Client::onUploadDataReady( const QVariant &response ){
    qDebug() << "onUploadDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit uploadDataReady();
}
// ����
void Client::onDownloadDataReady( const QVariant &response ){
    qDebug() << "onDownloadDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit downloadDataReady();
}
// ȡ�ñ�ǩ���ĵ��б�
void Client::onGetSimpleListByTagDataReady( const QVariant &response ){
    qDebug() << "onGetSimpleListByTagDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getSimpleListByTagDataReady();
}
// ȡ���ļ������ĵ��б�
void Client::onGetSimpleListByDirDataReady( const QVariant &response ){
    qDebug() << "onGetSimpleListByDirDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getSimpleListByDirDataReady();
}
// ȡ���ĵ��б�
void Client::onGetSimpleListDataReady( const QVariant &response ){
    qDebug() << "onGetSimpleListDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getSimpleListDataReady();
}
// ȡ�����б�ǩ
void Client::onGetAllTagDataReady( const QVariant &response ){
    qDebug() << "onGetAllTagDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getAllTagDataReady();
}
// ȡ�������ļ���
void Client::onGetAllDirDataReady( const QVariant &response ){
    qDebug() << "onGetAllDirDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit getAllDirDataReady();
}
// �˳�ϵͳ
void Client::onClientLogoutDataReady( const QVariant &response ){
    qDebug() << "onClientLogoutDataReady";
    qDebug() << response;
    revalue = response;

    // ���������Ѿ�׼������Ϣ
    emit clientLogoutDataReady();
}
