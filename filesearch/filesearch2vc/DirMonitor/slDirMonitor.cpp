#include "StdAfx.h"
#include "slDirMonitor.h"
#include "DirectoryChanges.h"
#include "DirectoryChangeHandler_Dispatch.h"
#include "sltDirMonitorThread.h"


//default filters
#define INCLUDE_FILTERS_STRING	""
#define EXCLUDE_FILTERS_STRING	"*\\SYSTEM VOLUME INFORMATION\\*;*\\WINDOWS\\*;*\\WINNT\\*;*\\RECYCLER\\*;*\\LOCAL SETTINGS\\*;*\\APPLICATION DATA\\*;*\\TEMPLATES\\*;*\\COOKIES\\*;*\\RECYCLED\\*;*\\PROGRAM FILES\\360\\*;*\\PROGRAM FILES\\FOXMAIL\\*;"


CDirectoryChangeWatcher			m_DirWatcher;
slLogSendThread		g_LogSendThread;
slXmlAgent			g_xmlFilterAgent;

sltDirMonitorThread g_DirMonThread;

CString	m_strExcludeFilter1;
CString	m_strIncludeFilter1;

BOOL g_iniInit = FALSE;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	BOOL    bret = FALSE;
	HRESULT hr = E_FAIL;

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH: 
			{
				char szLogPath[MAX_PATH] = {0};
				char szProcName[MAX_PATH] = {0};
				GetSystemDirectory(szProcName, MAX_PATH);
				sprintf(szLogPath,"%s\\DirMonitor.log", szProcName);
						
				log.SetFile(szLogPath,true);
				log.SetLevel(100);
				log.SetMode(Log::ToFile );	
			}
			break;
		case DLL_PROCESS_DETACH:
			{				

			}
			break;
		case DLL_THREAD_ATTACH:

			break;
		case DLL_THREAD_DETACH:

			break;
    }
    return TRUE;
}

//�������
DWORD __stdcall Monitor_Start()
{
	log.Print(LL_DEBUG_INFO, "[Info]Enter Monitor_Start\r\n");
	g_DirMonThread.startup();

	log.Print(LL_DEBUG_INFO, "[Info]Leave Monitor_Start\r\n");
	return 0;
}

//���ù���Ŀ¼
//0    �ɹ�
//���� ʧ��
DWORD __stdcall Monitor_Reset()
{
	//
	//����Ŀ¼�ı䣬���¶�ȡ�����˵�Ŀ¼
	return g_xmlFilterAgent.LoadDB();
}

//ֹͣ���
DWORD __stdcall Monitor_Stop()
{
	log.Print(LL_DEBUG_INFO, "[Info]Enter Monitor_Stop\r\n");

	m_DirWatcher.UnwatchAllDirectories();

	//ֹͣ�����߳�
	g_LogSendThread.shutdown();
	g_xmlFilterAgent.Clear();
	g_DirMonThread.shutdown();

	log.Print(LL_DEBUG_INFO, "[Info]Leave Monitor_Stop\r\n");
	return 0;
}

//�����������
DWORD __stdcall Monitor_Start_AllDisk(BOOL bRemovableDisk)
{
	DWORD dwRet = 0;
	//ö�ٵ�ǰ���е��̷����������еĿ�д�̷���
	TCHAR szDiskStr[MAX_PATH] = {0};
	GetLogicalDriveStrings(MAX_PATH, szDiskStr);

	TCHAR *ptmp  = szDiskStr;
	while (ptmp[0])
	{
		//��������� ����
		if(('a' == (*ptmp)) || ('A' == (*ptmp)))
		{
			ptmp += 4;
			continue;
		}
		
		if(((DRIVE_REMOVABLE == GetDriveType(ptmp)) && bRemovableDisk) || 
			(DRIVE_FIXED == GetDriveType(ptmp)) || 
			(DRIVE_REMOTE == GetDriveType(ptmp)))
		{
			CString sDiskStr(ptmp);
			dwRet = Monitor_Start_Dir(ptmp, 4);
		}
		
		ptmp += 4;
	}

	return dwRet;
}

//��ع̶�����
DWORD __stdcall Monitor_Start_Dir(char* pszDirPath, DWORD dwLen)
{
	log.Print(LL_DEBUG_INFO,"Begin To Monitor %s\r\n",pszDirPath);
	DWORD dwRet = 0;
	DWORD dwWatch = 0;
	CString	m_strDirectoryToMonitor(pszDirPath);

	DWORD dwChangeFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;;
	BOOL bWatchSubDir = TRUE;
	
	if( m_DirWatcher.IsWatchingDirectory( m_strDirectoryToMonitor) )
		m_DirWatcher.UnwatchDirectory( m_strDirectoryToMonitor );

	CDirectoryChangeHandler_Dispatch * pHandler = new CDirectoryChangeHandler_Dispatch();
		
	if( ERROR_SUCCESS != (dwWatch = m_DirWatcher.WatchDirectory(m_strDirectoryToMonitor, 
		dwChangeFilter,
		pHandler,
		bWatchSubDir,
		m_strIncludeFilter1,
		EXCLUDE_FILTERS_STRING)) )
	{
		dwRet = 1;
	}

	if( pHandler )
		pHandler->Release();

	return dwRet;
}
