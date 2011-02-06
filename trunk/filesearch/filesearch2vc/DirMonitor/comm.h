#if !defined(AFX_COMM__INCLUDED_)
#define AFX_COMM__INCLUDED_

enum File_Action{
	FILE_UNKNOW = 0,
	FILE_ADD,
	FILE_MODIFYED,
	FILE_REMOVED,
	FILE_NAMECHANGE
};


#pragma pack(push, 1)

typedef struct _File_Action_Log
{
	File_Action FileActon;
	char szDbName[MAX_PATH];
	char szSrcName[MAX_PATH*2];
	char szDesName[MAX_PATH];
	char szLogTime[30];
	_File_Action_Log()
	{
		FileActon = FILE_UNKNOW;
		memset(szDbName, NULL, MAX_PATH);
		memset(szSrcName,NULL, MAX_PATH);
		memset(szDesName,NULL, MAX_PATH);
		memset(szLogTime, NULL, 30);
	}

}File_Action_Log,*pFile_Action_Log;

#pragma pack(pop)

#endif