// sloLicenseAgent.cpp: implementation of the sloLicenseAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FsUi.h"
#include "sloLicenseAgent.h"
#include "sloRegAgent.h"
#include "myftpcomm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloLicenseAgent::sloLicenseAgent()
{
	memset(&m_LicInfo, NULL, sizeof(LicenseInfo));

	ClearPath();
		
		m_bReg = FALSE;
	m_bDown = FALSE;
}

void sloLicenseAgent::ClearPath()
{
	memset(m_szOldLicBatPath, NULL, MAX_PATH);
	memset(m_szLicBatPath, NULL, MAX_PATH);
	memset(m_szLicTxtPath, NULL, MAX_PATH);
	
	memset(m_szServerLicBatPath, NULL, MAX_PATH);
	memset(m_szServerLicTxtPath, NULL, MAX_PATH);
}

sloLicenseAgent::~sloLicenseAgent()
{

}

int sloLicenseAgent::Init()
{
	//
	if (!m_pMySqlDB && !ConnectDB())
	{
		return -1;
	}
	
	std::string strQuerySQL = "select * from t_licence";
	HRESULT hr = doSqlExe(FALSE, strQuerySQL.c_str());
	if (FAILED(hr))
		return -1;
	
	int nCount = m_pMySqlDB->GetRowCount();
	int nFieldCount = m_pMySqlDB->GetFieldCount();
	if(nCount >= 1 && nFieldCount >= 1)
	{	
		bool bSucc = m_pMySqlDB->GetRow();
		if(bSucc==false)
			return -1;
		
		int nOrderNoLen = 0;
		char* pOrder = m_pMySqlDB->GetField("orderno",&nOrderNoLen);
		if(pOrder != NULL && nOrderNoLen >= 1)	
			strcpy(m_LicInfo.szOrderNo, pOrder);

		int nEmailLen = 0;
		char* pEmail = m_pMySqlDB->GetField("email",&nEmailLen);
		if(pEmail != NULL && nEmailLen >= 1)	
			strcpy(m_LicInfo.szEmail, pEmail);

		int nstartdateLen = 0;
		char* pstartdate = m_pMySqlDB->GetField("startdate",&nstartdateLen);
		if(pstartdate != NULL && nstartdateLen >= 1)	
			strcpy(m_LicInfo.szStartDate, pstartdate);

		int nEnddateLen = 0;
		char* pEnddate = m_pMySqlDB->GetField("enddate",&nEnddateLen);
		if(pEnddate != NULL && nEnddateLen >= 1)
			strcpy(m_LicInfo.szEndDate, pEnddate);

		int nDownLen = 0;
		char* pDown = m_pMySqlDB->GetField("hasdownload",&nDownLen);
		if(pDown != NULL && nDownLen >= 1)
		{	
			int nDown = atoi(pDown);
			if (nDown == 1)
				m_bDown = TRUE;
		}

		m_bReg = TRUE;
	}
	
	return 0;	
}

BOOL sloLicenseAgent::EventLicense(char* szOrderNo, char* szEmail)
{
	BOOL bRet = TRUE;
	do 
	{
		SetLicenseInfo(szOrderNo, szEmail);

		//����license�ļ�
		if(!GetLicensePath())
		{
			bRet = FALSE;
			break;
		}

		//����licence_������.dat,licence_������.txt�ļ�
		int nRet = BuildLicenseFile();
		if (nRet != 0)
		{
			bRet = FALSE;
			break;
		}

		//���ϴ�ftp
		if (UpLoadLicFiles() != 0)
		{
			bRet = FALSE;
			break;
		}

		//ע����ɣ��������ݿ�
		nRet = AddLicenseRec();
		if (nRet != 0)
		{
			bRet = FALSE;
			break;
		}

		m_bReg = TRUE;

	} while (0);

	return bRet;
}

void sloLicenseAgent::SetLicenseInfo(char* szOrderNo, char* szEmail)
{
	strcpy(m_LicInfo.szOrderNo,szOrderNo);
	strcpy(m_LicInfo.szEmail,szEmail);
	strcpy(m_LicInfo.szStartDate,sloCommAgent::GetCurTime());
	strcpy(m_LicInfo.szEndDate,sloCommAgent::GetCurTime(1));
}

int sloLicenseAgent::AddLicenseRec()
{
	//
	if (!m_pMySqlDB && !ConnectDB())
	{
		return -1;
	}

	//���ȼ���Ƿ����һ����¼��������ɾ��
	if (m_bReg)
	{
		//ɾ�����м�¼
		std::string strQuerySQL = "delete from t_licence";
		HRESULT hr = doSqlExe(FALSE, strQuerySQL.c_str());
		if (FAILED(hr))
		{	
			OutputDebugString("delete t_licence failed!");
			//return -1;
		}
	}

	std::string strQuerySQL = "insert into t_licence(orderno,email,hasdownload,startdate,enddate) values('%s','%s','0','%s','%s')";
	HRESULT hr = doSqlExe(TRUE, strQuerySQL.c_str(),m_LicInfo.szOrderNo, m_LicInfo.szEmail, m_LicInfo.szStartDate, m_LicInfo.szEndDate);
	if (FAILED(hr))
		return -1;
	
	return 0;
}

int sloLicenseAgent::BuildLicenseFile()
{
	int nRet = 0;
	do 
	{
		//����licence_������.txt�ļ�
		char szContent[512];
		sprintf(szContent,"%s,0,%s,%s",m_LicInfo.szEmail, m_LicInfo.szStartDate,m_LicInfo.szEndDate);
		FILE *fp = fopen(m_szLicTxtPath, "w+");
		if(fwrite(szContent, 1, strlen(szContent), fp) == -1)
		{
			nRet = -2;
			break;
		}

		fclose(fp);

		//����licence_������.dat
		//copy�ļ�
		if (!CopyFile(m_szOldLicBatPath,m_szLicBatPath,FALSE))
		{
			int nErr = GetLastError();
			nRet = -3;
			break;
		}

// 		if(rename(m_szOldLicBatPath, m_szLicBatPath) != 0)
// 		{
// 			int nErr = GetLastError();
// 			nRet = -3;
// 			break;
// 		}

	} while (0);

	return nRet;
}

// FTP:  124.42.19.247
// �û���:  hd9002427
// ����:  ssssss
#define FTP_SERVER	"124.42.19.247"
#define FTP_USER	"hd9002427"
#define FTP_PWD		"ssssss"
#define CUR_FOLDER	"//licences"
int sloLicenseAgent::UpLoadLicFiles()
{
	int nRet = 0;

	myftpcomm ftp;	
	do 
	{
		//����ftp
		nRet = ftp.GetFtpConnection(FTP_SERVER, FTP_USER, FTP_PWD);
		if (nRet != 0)
			break;
	
		//���õ�ǰĿ¼
		if(!ftp.SetCurrentDirectory(CUR_FOLDER))
		{
			nRet = -1;
			break;
		}

		//����Ŀ¼,Ŀ¼�����򴴽�ʧ��
		if(!ftp.CreateDirectory(m_LicInfo.szStartDate))
		{
			nRet = 0/*-2*/;
//			break;
		}

		//�ϴ�bat�ļ�
		if(!ftp.PutFile(m_szLicBatPath, m_szServerLicBatPath))
		{
			nRet = -3;
			break;
		}

		//�ϴ�txt�ļ�
		if(!ftp.PutFile(m_szLicTxtPath, m_szServerLicTxtPath))
		{
			nRet = -3;
			break;
		}

	} while (0);

	//�ر�����
	ftp.Disconnect();

	return nRet;
}

#define  LICENSE_NAME "licence"

BOOL sloLicenseAgent::GetLicensePath()
{
	char szFileExePath[MAX_PATH] = {0};
	sloRegAgent reg;
	if(!reg.ReadInstallPath(szFileExePath))
		return FALSE;
	
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	
	_splitpath( szFileExePath, drive, dir, fname, ext );
	
	ClearPath();

	sprintf(m_szOldLicBatPath,"%s%s%s.dat",drive, dir,LICENSE_NAME);
	sprintf(m_szLicBatPath,"%s%s%s_%s.dat",drive, dir,LICENSE_NAME,m_LicInfo.szOrderNo);
	sprintf(m_szLicTxtPath,"%s%s%s_%s.txt",drive, dir,LICENSE_NAME,m_LicInfo.szOrderNo);
	
	sprintf(m_szServerLicBatPath,"%s//%s//%s_%s.dat",CUR_FOLDER,m_LicInfo.szStartDate, LICENSE_NAME,m_LicInfo.szOrderNo);
	sprintf(m_szServerLicTxtPath,"%s//%s//%s_%s.txt",CUR_FOLDER,m_LicInfo.szStartDate, LICENSE_NAME,m_LicInfo.szOrderNo);
	
	return TRUE;
	
}

//�Ϸ���e-mail��ַΪ����@ǰ������Ҫ����һ����ĸ����@�;��֮������һ����ĸ���ھ�������һ����ĸ
//�����ʼ���ַֻ�ܰ�����ĸ�����֡���� (.)�����ַ� (-) ���»��� (_)
bool sloLicenseAgent::IsEmailAddr(char* str,char* error)
{
	int strlen2=0;
	char* strempt=NULL;
	char* strempt2=NULL;
	char* P=NULL;
	char* error2=NULL;
	strempt=str;
	error2=error;
	strlen2=strlen(strempt);
	if(strlen2==0)
	{
		//MessageBox(NULL,"�����E-mail��ַ����Ϊ�գ�����������","��ʾ",MB_ICONINFORMATION);
		strcpy(error2,g_lag.LoadString("errors.nocharts"));
		return false;
	}
	for(P=strempt; *P!='\0';P++)
	{
		if(*P==' ')
		{
			//MessageBox(NULL,"�����E-mail��ַ�в��ܰ����ո��������������","��ʾ",MB_ICONINFORMATION);
			strcpy(error2,g_lag.LoadString("errors.hasnullchart"));
			return false;
		}
		else if((((*P>=48)&&(*P<=57))||((*P>=64)&&(*P<=90))||((*P>=97)&&(*P<=122))||*P==45||*P=='.'||*P==95)==0)
		{
			strcpy(error2,g_lag.LoadString("errors.hasinvalidchart"));
			return false;
		}
	}
	if(strlen2>50)
	{
		//MessageBox(NULL,"email��ַ���Ȳ��ܳ���50λ","��ʾ",MB_ICONINFORMATION);
		strcpy(error2,g_lag.LoadString("errors.overlen"));
		return false;
	}
	strempt2=strstr(strempt,"@");
	if(!strempt2)
	{
		strcpy(error2,g_lag.LoadString("errors.invalidchart1"));
		return false;
	}
	if(!(strempt2-strempt))
	{
		strcpy(error2,g_lag.LoadString("errors.invalidchart2"));
		return false;
	}
	if (strempt2-strempt==1)
		if(((*strempt>=65)&&(*strempt<=90))||((*strempt>=97)&&(*strempt<=122))==0)
		{
			strcpy(error2,g_lag.LoadString("errors.invalidchart3"));
			return false;
		}
		if (!(P=strstr(strempt2,".")))
		{
			strcpy(error2,g_lag.LoadString("errors.invalidchart4"));
			return false;
		}
		else
		{  
			if(!(P-strempt2-1))
			{
				strcpy(error2,g_lag.LoadString("errors.invalidchart5"));
				return false;
			}
			if(P-strempt2-1==1)
				if(((*(strempt2+1)>=65)&&(*(strempt2+1)<=90))||((*(strempt2+1)>=97)&&(*(strempt2+1)<=122))==0)
				{
					strcpy(error2,g_lag.LoadString("errors.invalidchart6"));
					return false;
				}
		}
		strempt2=strstr(strempt,".");
		if(*(strempt2+1)=='\0')
		{
			strcpy(error2,g_lag.LoadString("errors.invalidchart7"));
			return false;
		}
		if(((*(strempt2+1)>=65)&&(*(strempt2+1)<=90))||((*(strempt2+1)>=97)&&(*(strempt2+1)<=122))==0)
		{
			strcpy(error2,g_lag.LoadString("errors.invalidchart8"));
			return false;
		}
//		strcpy(error2,"����һ����ȷ�������ַ");
		return true;
}

BOOL sloLicenseAgent::BackLicense(char* szBackPath)
{
	//����license�ļ�������Ŀ¼
	//��ȡ
	//copyfile
	return TRUE;
}

BOOL sloLicenseAgent::RestoreLicense(char* szResotrePath)
{
	//��������Ŀ¼license�ļ�����װĿ¼
	//copyfile
	return TRUE;	
}