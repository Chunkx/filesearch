// XTPShortcutBarEx.cpp: implementation of the CXTPShortcutBarEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalAgenter.h"
#include "XTPShortcutBarEx.h"
#include "LocalAgenterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPShortcutBarEx::CXTPShortcutBarEx()
{

}

CXTPShortcutBarEx::~CXTPShortcutBarEx()
{

}
BEGIN_MESSAGE_MAP(CXTPShortcutBarEx, CXTPShortcutBar)
//{{AFX_MSG_MAP(CTabTreeFile)
	ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPShortcutBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPShortcutBarItem* pSelectedItem = GetHotItem();
	CLocalAgenterDlg* dlg = (CLocalAgenterDlg*)m_pParentWnd;
	if(pSelectedItem == dlg->m_pItemFolder)
	{
		//ѡ�дʻ��б���
		dlg->ShowListContent_Keywords(1, "ȫ��");
	}else if (pSelectedItem == dlg->m_pItemCalendar)
	{
		//ѡ����ַ�б�
		dlg->ShowListContent_Website("ȫ��");
	}else if (pSelectedItem == dlg->m_pItemTasks)
	{
		//�첶�б���

	}

	CXTPShortcutBar::OnLButtonDown(nFlags, point);
}


void CXTPShortcutBarEx::SetParentWnd(LPVOID pParentWnd)
{
	m_pParentWnd = pParentWnd;
}