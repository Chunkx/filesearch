// sloFastSearchAgent.cpp: implementation of the sloFastSearchAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FsUi.h"
#include "sloFastSearchAgent.h"
#include "sloRegAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloFastSearchAgent::sloFastSearchAgent()
{
	memset(m_szKeyPath, NULL, MAX_PATH);
	GetKeyFilePath();
}

sloFastSearchAgent::~sloFastSearchAgent()
{

}

std::string sloFastSearchAgent::GetFilePathFromName(char* szName)
{
	std::string strPath= "";

	do 
	{
		//
		if (!m_pMySqlDB && !ConnectDB())
		{
			break;
		}
		
		std::string strQuerySQL = "select * from t_result where filename='%s'";
		HRESULT hr = doSqlExe(TRUE, strQuerySQL.c_str(), sloCommAgent::ConverSqlPath(szName).c_str());
		if (FAILED(hr))
			break;
		
		int nCount = m_pMySqlDB->GetRowCount();
		int nFieldCount = m_pMySqlDB->GetFieldCount();
		if(nCount >= 1 && nFieldCount >= 1)
		{			
			bool bSucc = m_pMySqlDB->GetRow();
			if(bSucc==false)
				break;
			
			int nFilePathLen = 0;
			char* pFilePath = m_pMySqlDB->GetField("filepath",&nFilePathLen);
			if (nFilePathLen != 0 && pFilePath != NULL)
				strPath = pFilePath;				
		}
	} while (0);

	return strPath;
}

DWORD sloFastSearchAgent::GetAllPath()
{
	//�����ݿ��л�ȡ��������id
	if (!m_pMySqlDB && !ConnectDB())
	{
		return -1;
	}

	ClearPathList();
	
	std::string strQuerySQL = "select * from t_searcher order by id asc";
	HRESULT hr = doSqlExe(FALSE, strQuerySQL.c_str() );
	if (FAILED(hr))
		return -1;
	
	int nCount = m_pMySqlDB->GetRowCount();
	int nFieldCount = m_pMySqlDB->GetFieldCount();
	if(nCount >= 1 && nFieldCount >= 1)
	{	
		for (int i = 0; i < nCount; i++)
		{
			bool bSucc = m_pMySqlDB->GetRow();
			if(bSucc==false)
				return -1;
			
			int nPathLen = 0;
			char* pPath = m_pMySqlDB->GetField("path",&nPathLen);
			if(pPath != NULL && nPathLen >= 1)
			{	
				int nIDLen = 0;
				char* pID = m_pMySqlDB->GetField("id",&nIDLen);
				if(pID != NULL && nIDLen >= 1)
				{	
					PathIndex path;
					memset(&path, NULL, sizeof(PathIndex));
					path.nID = atoi(pID);
					strcpy(path.szPath, pPath);

					m_PathList.push_back(path);
				}
			}			
		}
	}
	
	return 0;	
}


DWORD sloFastSearchAgent::SearchRecords(std::string strQuerySQL, BOOL bRecent/* = FALSE*/)
{
	//��������Ƿ�����
	if (!m_pMySqlDB && !ConnectDB())
	{
		return -1;
	}
	
	ClearRecList();	
	
	HRESULT hr = doSqlExe(FALSE, strQuerySQL.c_str() );
	if (FAILED(hr))
		return -2;
	
	int nCount = m_pMySqlDB->GetRowCount();
	int nFieldCount = m_pMySqlDB->GetFieldCount();
	if(nCount >= 1 && nFieldCount >= 1)
	{	
		for (int i = 0; i < nCount; i++)
		{
			bool bSucc = m_pMySqlDB->GetRow();
			if(bSucc==false)
				return -1;

			//�Ȳ����Ƿ��и���
			SearchRectord sr;
			memset(&sr, NULL, sizeof(SearchRectord));
			if (!bRecent)
			{
				//ȡt_result���е�����
				int nFileTypeLen = 0;
				char* pFileType = m_pMySqlDB->GetField("filetype",&nFileTypeLen);
				if (nFileTypeLen == 0)
					continue ;
				
				int nFileNameLen = 0;
				char* pFileName = m_pMySqlDB->GetField("filename",&nFileNameLen);
				
				int nFilePathLen = 0;
				char* pFilePath = m_pMySqlDB->GetField("filepath",&nFilePathLen);
				
				int nDespLen = 0;
				char* pDesp = m_pMySqlDB->GetField("desp",&nDespLen);
				
				int nContentLen = 0;
				char* pContent = m_pMySqlDB->GetField("content",&nContentLen);
							
				if (nFileTypeLen)
					memcpy(sr.szFileType, pFileType, nFileTypeLen);
				if (nFileNameLen)
					memcpy(sr.szFileName, pFileName, nFileNameLen);
				if (nFilePathLen)
					memcpy(sr.szFilePath, pFilePath, nFilePathLen);			
				if (nDespLen)
					sr.DespList.push_back(pDesp);			
				if (nContentLen)
					memcpy(sr.szContent, pContent, nContentLen);
			}else
			{
				m_filterAgent.Init();
				//ȡt_recent_changeinfo
				int nFilePathLen = 0;
				char* pFilePath = m_pMySqlDB->GetField("path",&nFilePathLen);
				if (nFilePathLen == 0)
					continue ;
				
				//����ȫ·����ȡ�ļ���
				char szFileName[MAX_PATH] = {0};
				char szExt[MAX_PATH] = {0};
				_splitpath(pFilePath, NULL, NULL, szFileName, szExt);
				strcat(szFileName, szExt);
				
				//�����ļ���׺��ȡ�ļ�����
				std::string strType = m_filterAgent.GetFileTypeFromExt(szExt);
				
				//���ṹ��
				memcpy(sr.szFileType, strType.c_str(), strType.size());
				memcpy(sr.szFileName, szFileName, strlen(szFileName));
				memcpy(sr.szFilePath, pFilePath, nFilePathLen);			
			}

			AddList(sr);	
		}
	}
	
	return 0;	
}


DWORD sloFastSearchAgent::GetSearchRecords()
{
	std::string strQuerySQL = "select * from t_result";
	
	return SearchRecords(strQuerySQL, FALSE);
}

DWORD sloFastSearchAgent::GetSearchRecords_Recent(int nDays)
{	
	//select distinct path from commoninfo.t_recent_changeinfo where TO_DAYS(NOW())-TO_DAYS(systime) >= #days# and operflg != 3 AND operflg != 4 (û��ɾ����������)
//	std::string strQuerySQL = "select distinct path from t_recent_changeinfo where TO_DAYS(NOW())-TO_DAYS(systime) <= %d and operflg != 3 AND operflg != 4";
	char szSql[1024] = {0};
	sprintf(szSql, "select distinct path from t_recent_changeinfo where TO_DAYS(NOW())-TO_DAYS(systime) <= %d and operflg='2' limit 0,500", nDays-1);
	
	return SearchRecords(szSql, TRUE);	
}

void sloFastSearchAgent::AddList(SearchRectord sr)
{
	int nCount = m_RecList.size();
	for (int i = 0; i < nCount; i++)
	{
		if (strcmp(sr.szFilePath, m_RecList[i].szFilePath) == 0)
		{
			break ;
		}
	}	

	if (i >= nCount)
	{
		m_RecList.push_back(sr);
	}else
		m_RecList[i].DespList.push_back(sr.DespList[0]);
}


BOOL sloFastSearchAgent::IsKeyFileExist()
{
	if(strlen(m_szKeyPath) == 0)
		return FALSE;

	return sloCommAgent::IsFileExists(m_szKeyPath);
}

#define  KEY_PRO_NAME "tomcat\\webapps\\slfile\\WEB-INF\\classes\\com\\web\\searchlocal\\properties\\keyWord.properties"
BOOL sloFastSearchAgent::GetKeyFilePath()
{
	char szFileExePath[MAX_PATH] = {0};
	sloRegAgent reg;
	if(reg.ReadInstallPath(szFileExePath))
	{  
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		
		_splitpath( szFileExePath, drive, dir, fname, ext );
		sprintf(m_szKeyPath,"%s%s%s",drive, dir,KEY_PRO_NAME);
	}
	
	return FALSE;
}

void sloFastSearchAgent::ClearPathList()
{
	m_PathList.clear();
}


void sloFastSearchAgent::ClearRecList()
{
	m_RecList.clear();
}

//
//����ֵ:
//id����:  0-ȫ�� -1-����ĵ�
int sloFastSearchAgent::GetPathIndex(char* szPath)
{
	if (strcmp(szPath, g_lag.LoadString("label.fastserlist")) == 0)
	{
		return 0;
	}

	if (strncmp(szPath, RECENT, sizeof(RECENT)) == 0)
	{
		return -1;
	}

	int nCount = m_PathList.size();
	for (int i = 0; i < nCount; i++)
	{
		PathIndex index = m_PathList[i];
		if (strcmp(szPath, index.szPath) == 0)
		{
			return index.nID;
		}
	}

	return -1;
}