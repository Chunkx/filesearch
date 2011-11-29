// ReportRecord.cpp: implementation of the CReportRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalAgenter.h"
#include "ReportRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReportRecord::CReportRecord(CString strName, CString strDate)
{
	//��ǰ����ʾ��ͼ��
	AddItem(new CXTPReportRecordItem());
	AddItem(new CXTPReportRecordItem());
	
	AddItem(new CXTPReportRecordItem());
	AddItem(new CXTPReportRecordItem());

	AddItem(new CXTPReportRecordItemText(strName));				//ѧԱ���
	AddItem(new CXTPReportRecordItemText(strDate));			//ѧԱ����	
}

CReportRecord::~CReportRecord()
{

}
