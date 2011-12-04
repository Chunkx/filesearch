// XTPReportControlEx.cpp: implementation of the CXTPReportControlEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalAgenter.h"
#include "XTPReportControlEx.h"
#include "LocalAgenterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPReportControlEx::CXTPReportControlEx()
{
	m_ParentDlg = NULL;
}

CXTPReportControlEx::~CXTPReportControlEx()
{

}

BEGIN_MESSAGE_MAP(CXTPReportControlEx, CXTPReportControl)
//{{AFX_MSG_MAP(CTabTreeFile)
	ON_WM_CONTEXTMENU()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CXTPReportControlEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

	ScreenToClient(&point);
	CXTPReportColumn* pColumn = GetColumns()->GetReportHeader()->HitTest(point);
	if (pColumn != NULL)
	{
		//ѡ��ͷ
		return;
	}

//	ѡ�н����
// 	CXTPReportRow* pRow = HitTest(point);	
// 	if (pRow == NULL)
// 	{
// 		return ;
// 	}
	
	//�����˵�
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_TREE));

	//δѡ��
	ClientToScreen(&point);
	
	menu.GetSubMenu(0)->RemoveMenu(1, MF_BYPOSITION);
	menu.GetSubMenu(0)->RemoveMenu(1, MF_BYPOSITION);
	int nReturn = CXTPCommandBars::TrackPopupMenu(menu.GetSubMenu(0), TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY,  point.x, point.y, this);
	
	switch (nReturn)
	{
	case ID_EDIT_ADD:
		{
			//����һ��row��item
			//������Ϊ�ɱ༭״̬
			CLocalAgenterDlg* pDlg = (CLocalAgenterDlg*)m_ParentDlg;
			pDlg->AddNewRecord("����");
		}
		break;		
	}
	return ;	
	
}

void CXTPReportControlEx::SetParentDlg(LPVOID pDlg)
{
	m_ParentDlg = pDlg;
}
