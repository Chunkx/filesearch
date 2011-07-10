// sloRecentIndexAgent.cpp: implementation of the sloRecentIndexAgent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sl4j.h"
#include "sloRecentIndexAgent.h"

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib ")

#include "sloDiskManager.h"
#include "sloRegAgent.h"
#include "sloCommAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloRecentIndexAgent::sloRecentIndexAgent()
{
	memset(m_szpropertiesPath, NULL, MAX_PATH);
}

sloRecentIndexAgent::~sloRecentIndexAgent()
{

}

#define PATH_NAME	"indexpath"
DWORD sloRecentIndexAgent::CreateRecIndexPath()
{
	sloDiskManager disk;
	
	//������������ļ�������Ŀ¼ 
	char szRecentIndexPath[MAX_PATH] = {0};
	sprintf(szRecentIndexPath,"%c:\\recent_files_index",disk.GetMaxFreeSpaceDisk());
	
	//������Ŀ¼��������Ϊ��������
	if (!PathFileExists(szRecentIndexPath))
	{
		if (CreateDirectory(szRecentIndexPath, NULL))
		{
			//�����ļ�������
			SetFileAttributes(szRecentIndexPath,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	
	//д�������ļ�
	if(GetProFilePath() == FALSE)
		return -2;
	
	BOOL bRet = sloCommAgent::WritePropertyfileString(PATH_NAME, szRecentIndexPath, m_szpropertiesPath);
	
	return bRet ? 0 : -1;
}


#define  RECENT_PRO_NAME "tomcat\\webapps\\slfile\\WEB-INF\\classes\\com\\web\\searchlocal\\properties\\recentIndexPath.properties"
BOOL sloRecentIndexAgent::GetProFilePath()
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
		sprintf(m_szpropertiesPath,"%s%s%s",drive, dir,RECENT_PRO_NAME);
		return TRUE;
	}
	
	return FALSE;
}