#include <utils.h>


#include "kpsdk.h"
#include "kpan.h"



//��ȡ��ʱtoken��secret����֤url
//���룺consumerKey    consumerSecret
//�����tmpOauthToken  tmpOauthTokenSecret     authoriseUrl
//����: http ״̬
int Kpan::kpReqTmpToken(QString &tmpOauthToken,QString &tmpOauthTokenSecret,\
                                                     QString &authoriseUrl,\
                                                     const QString &consumerKey,const QString &consumerSecret)
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->reqReqTmpToken();

    tmpOauthToken=kp->mTmpToken;
    tmpOauthTokenSecret=kp->mTmpTokenSecret;
    authoriseUrl = QString(KP_AUTHORISE_SRC_URL).append(tmpOauthToken);

    int ret = kp->mRet;
    delete kp;
    return ret;
}

//��ȡaccessToken
//����:consumerKey     consumerSecret         tmpOauthToken  tmpOauthTokenSecret
//�����oauthToken     oauthTokenSecret        usrID       chargedDir
//����: http ״̬
int Kpan::kpAccessToken(QString &oauth_token, QString &oauth_token_secret,\
                                                      QString &user_id,      QString &charged_dir,\
                                                      const QString &consumerKey,const QString &consumerSecret,\
                                                      const QString &tmpOauthToken,const QString &tmpOauthTokenSecret
                                                      )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey = consumerKey;
    kp->mConsumerSecret = consumerSecret;
    kp->mTmpToken=tmpOauthToken;
    kp->mTmpTokenSecret=tmpOauthTokenSecret;
    kp->reqAcessToken();
    oauth_token = kp->mAuTokenInfo.auToken;
    oauth_token_secret = kp->mAuTokenInfo.auTokenSecret;
    user_id=kp->mAuTokenInfo.usrID;
    charged_dir=kp->mAuTokenInfo.chargedDir;

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//��ȡ�û���Ϣ
//����: consumerKey consumerSecret            oauthToken    oauthTokenSecret
//�����max_file_size   user_name       quota_used       quota_total
//����: http ״̬
int Kpan::kpUsrInfo(QString &max_file_size, QString &user_name,\
                                                  QString &quota_used,    QString &quota_total,\
                                                  const QString &consumerKey,const QString &consumerSecret,\
                                                  const QString &oauthToken,const QString &oauthTokenSecret
                                                  )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->reqUsrInfo();
    max_file_size=kp->mUsrInfo.maxFileSize;
    user_name=kp->mUsrInfo.usrName;
    quota_used=kp->mUsrInfo.quotaUsed;
    quota_total=kp->mUsrInfo.quotaTotal;

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//��ȡ�ļ�(��)��Ϣ
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   dataPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//dataPath  ��ָҪ��ȡ���ļ�(��)·��+��
//�����jsonMetaData
//����: http ״̬
int Kpan::kpMetaData(QString &jsonMetaData,\
                                                  const QString &consumerKey,const QString &consumerSecret,\
                                                  const QString &oauthToken,const QString &oauthTokenSecret,
                                                  const bool   &isAppPath, const QString &dataPath
                                                  )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mDataPath=dataPath;
    kp->reqMetaData();
    jsonMetaData=QString(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//�½��ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ�½����ļ���·��+��
//�����jsonCreateFolder
//����: http ״̬
int Kpan::kpCreateFolder(QString &jsonCreateFolder,\
                                                    const QString &consumerKey,const QString &consumerSecret,\
                                                    const QString &oauthToken,const QString &oauthTokenSecret,
                                                    const bool   &isAppPath, const QString &floderPath
                                                    )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mCreateFolderPath=floderPath;
    kp->reqCreateFloder();
    jsonCreateFolder=QString(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//ɾ���ļ���
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   floderPath  to_recycle
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//floderPath  ��ָҪ��ȡ���ļ�(��)·��+��
//to_recycle  �Ƿ�ɾ��������վ��
//�����jsonDelFile
int Kpan::kpDelFile(QString &jsonDelFile,\
                                               const QString &consumerKey,const QString &consumerSecret,\
                                               const QString &oauthToken,const QString &oauthTokenSecret,
                                               const bool   &isAppPath, const QString &floderPath,
                                               const bool   &to_recycle
                                               )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mIsToRecyle=to_recycle;
    kp->mDelFilePath=floderPath;
    kp->reqDelFile();
    jsonDelFile = QString(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//�ƶ��ļ�(��)
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
//����: http ״̬
int Kpan::kpMvFile(QString &jsonMvFile,\
                                              const QString &consumerKey,const QString &consumerSecret,\
                                              const QString &oauthToken,const QString &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const QString &fromPath, const QString &toPath
                                              )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=fromPath;
    kp->mToPath=toPath;
    kp->reqMoveFile();
    jsonMvFile=QString(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//�����ļ�(��)
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    isAppPath   fromPath    toPath
//isAppPath ��Ӧ�÷���Ȩ��·��Ϊkuaipan��Ϊfalse,��Ϊtrue
//�����jsonMvFile
int Kpan::kpCpFile(QString &jsonCpFile,\
                                              const QString &consumerKey,const QString &consumerSecret,\
                                              const QString &oauthToken,const QString &oauthTokenSecret,
                                              const bool   &isAppPath, \
                                              const QString &fromPath, const QString &toPath
                                              )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=fromPath;
    kp->mToPath=toPath;
    kp->reqCopyFile();
    jsonCpFile=QString(kp->mJsonBuf);

    int ret=kp->mRet;
    delete kp;
    return ret;
}

//��ȡ�ϴ��ڵ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret
//�����uploadNode
//����: http ״̬
int Kpan::kpUploadLocate(QString &uploadNode,\
                                                       const QString &consumerKey,const QString &consumerSecret,\
                                                       const QString &oauthToken,const QString &oauthTokenSecret
                                                       )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->reqUploadLocate();
    uploadNode=kp->mUploadNode;
    int ret=kp->mRet;
    delete kp;
    return ret;
}

//�ϴ��ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isOverWrite isAppPath   uploadNode  fromPath toPath
//�����jsonUploadFile
//����: http ״̬
int Kpan::kpUploadFile(QString &jsonUploadFile,
                                                  const QString &consumerKey,const QString &consumerSecret,\
                                                  const QString &oauthToken,const QString &oauthTokenSecret,\
                                                  const bool &isOverWrite, const bool &isAppPath,\
                                                  const QString &uploadNode, const QString &fromPath,
                                                  const QString &toPath
                                                  )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsOverWrite=isOverWrite;
    kp->mIsAppPath=isAppPath;
    kp->mUploadNode=uploadNode;
    kp->mFromPath=fromPath;
    kp->mToPath=toPath;
    kp->reqUploadFile();
    jsonUploadFile=kp->mJsonBuf;
    int ret=kp->mRet;
    delete kp;
    return ret;
}

//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath toPath
//����: http ״̬
int Kpan::kpDownloadFile(const QString &consumerKey,const QString &consumerSecret,\
                                                  const QString &oauthToken,const QString &oauthTokenSecret,\
                                                  const bool &isAppPath,const QString &fromPath,
                                                  const QString &toPath
                                                  )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=fromPath;
    kp->mToPath=toPath;
    kp->reqDownLoadFile();

    int ret=kp->mRet;
    delete kp;
    return ret;
}


//�����ļ�
//����: consumerKey consumerSecret  oauthToken    oauthTokenSecret    ��
//     isAppPath   fromPath
//�����jsonShareFile
//����: http ״̬
int Kpan::kpShareFile(QString &jsonShareFile, const QString &consumerKey,const QString &consumerSecret,\
                                                 const QString &oauthToken,const QString &oauthTokenSecret,\
                                                 const bool &isAppPath,const QString &fromPath
                                                 )
{
    KpSDK *kp=new KpSDK();
    kp->mConsumerKey=consumerKey;
    kp->mConsumerSecret=consumerSecret;
    kp->mAuTokenInfo.auToken=oauthToken;
    kp->mAuTokenInfo.auTokenSecret=oauthTokenSecret;
    kp->mIsAppPath=isAppPath;
    kp->mFromPath=fromPath;
    kp->reqShareFile();
    jsonShareFile=kp->mJsonBuf;

    int ret=kp->mRet;
    delete kp;
    return ret;
}
