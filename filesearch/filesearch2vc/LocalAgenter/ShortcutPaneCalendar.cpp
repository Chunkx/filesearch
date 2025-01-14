// ShortcutPaneCalendar.cpp: implementation of the CShortcutPaneCalendar class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2009 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ShortcutPaneCalendar.h"
//#include "MainFrm.h"
#include "LocalAgenterDlg.h"
#include "GroupDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShortcutPaneCalendar::CShortcutPaneCalendar()
{
	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 1, 1);

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_CALENDAR);
	ASSERT(hIcon);

	m_ilTreeIcons.Add (hIcon);

	m_bSelect = FALSE;
	
	memset(m_szTypeName, NULL, MAX_PATH);

	m_nType = 2;
}

CShortcutPaneCalendar::~CShortcutPaneCalendar()
{

}

void CShortcutPaneCalendar::SetParentWnd(LPVOID pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

BOOL CShortcutPaneCalendar::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	if (!CXTPShortcutBarPane::Create(lpszCaption, pParent))
		return FALSE;

//	m_wndCalendar.Create(WS_CHILD|WS_VISIBLE|MCS_NOTODAY , CPoint(0, 0), this, 0);

	VERIFY(m_wndTreeCalendar.Create(WS_VISIBLE|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_EDITLABELS |TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, 0));

	m_wndTreeCalendar.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);
	m_wndTreeCalendar.SetParentPane(this);

	//读取数据库，获取树基本信息
	sloMysqlAgent::GetInstance()->GetGroupsFromDB_Website();
	HTREEITEM hItem;
	//获取分组个数
	int nGroupSize = sloMysqlAgent::GetInstance()->m_GroupListWebsite.size();
	for (int i = 0; i < nGroupSize; i++)
	{
		hItem = m_wndTreeCalendar.InsertItem (sloMysqlAgent::GetInstance()->m_GroupListWebsite[i].szGroupName, 0, 0);
	}

	CXTPShortcutBarPaneItem* pitem = AddItem(_T(""), &m_wndTreeCalendar, 115);
    pitem->ShowCaption(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CShortcutPaneCalendar, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneCalendar)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, 0, OnSelchanged)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CShortcutPaneCalendar::OnSize(UINT nType, int cx, int cy)
{
	CXTPShortcutBarPaneItem* pItem = GetItem(0);
	if (pItem)
	{
		pItem->SetHeight(max(0, cy - 150));
	}

	CXTPShortcutBarPane::OnSize(nType, cx, cy);
}



void CShortcutPaneCalendar::OnSelchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CLocalAgenterDlg* pDlg = (CLocalAgenterDlg*)m_pParentWnd;
	// Get the selected tree item and its icon.
	//	int nImage;
	HTREEITEM htItem = m_wndTreeCalendar.GetSelectedItem();
	if (htItem != NULL)
	{
		pDlg->m_btnDelete.EnableWindow(FALSE);
		m_bSelect = TRUE;
		//		m_wndTreeCtrl.GetItemImage( htItem, nImage, nImage );
		
		// 		pFrameWnd->UpdateCaption(m_wndTreeCtrl.GetItemText( htItem ),
		// 			m_ilTreeIcons.ExtractIcon(nImage));
		CString strItemText = m_wndTreeCalendar.GetItemText(htItem);
		m_wndTreeCalendar.SelectItem(htItem);	
		
		pDlg->ShowListContent_Website( strItemText.GetBuffer(0));
		//记录选中类型
		memset(m_szTypeName, NULL, MAX_PATH);
		strcpy(m_szTypeName, strItemText.GetBuffer(0));
	}
	
	*pResult = 0;
	
}

void CShortcutPaneCalendar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	HTREEITEM hItemSelected = m_wndTreeCalendar.GetSelectedItem();
	if (hItemSelected == NULL || !m_bSelect)
	{
		//选中空白位置，不显示菜单
		return ;
	}

	m_bSelect = FALSE;
	CString strItemText = m_wndTreeCalendar.GetItemText(hItemSelected);

	//如果选中父节点
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_TREE));
	ClientToScreen(&pt);
	int nReturn = CXTPCommandBars::TrackPopupMenu(menu.GetSubMenu(0), TPM_LEFTALIGN|TPM_RETURNCMD|TPM_NONOTIFY, pt.x, pt.y, this);
		
	switch (nReturn)
	{
	case ID_EDIT_ADD:
		{
			char szContent[MAX_PATH] = {0};
			for (int i = 1; i <= MAX_PATH; i++)
			{
				memset(szContent, NULL, MAX_PATH);
				sprintf(szContent, "自定义%d", i);
				if(sloMysqlAgent::GetInstance()->ExistWebsiteGroup(szContent) == FALSE)
					break;
			}
			//新增一节点，然后设置为可编辑
			HTREEITEM hNewItem = m_wndTreeCalendar.InsertItem(szContent, 0, 0);
			//并更新数据库表t_keywords_type
			sloMysqlAgent::GetInstance()->AddGroup_Website(szContent);
			CEdit* pEdit = m_wndTreeCalendar.EditLabel(hNewItem);		
		}
		break;	
	case ID_EDIT_DEL:
		{
			int nRet = MessageBox("您确认删除该分组？","删除分组", MB_YESNO | MB_ICONWARNING);
			if(6 == nRet)
			{
				m_wndTreeCalendar.DeleteItem(hItemSelected);
				//并更新数据库表
				sloMysqlAgent::GetInstance()->DelGroup_Website(strItemText.GetBuffer(0));
			}
		}
	case ID_EDIT_RENAME:
		{
			CEdit* pEdit = m_wndTreeCalendar.EditLabel(hItemSelected);		
		}
		break;				
	}
}


void CShortcutPaneCalendar::SetOlditemText(CString strOlditem)
{
	m_strItemOld = strOlditem;
}

BOOL CShortcutPaneCalendar::RenameItem(CString strNewitem)
{
	if (m_strItemOld == strNewitem || m_strItemOld.IsEmpty() || strNewitem.IsEmpty())
	{
		return TRUE;
	}

	//并写入数据库表
	sloMysqlAgent::GetInstance()->UpdateGroup_Website(m_strItemOld.GetBuffer(0), strNewitem.GetBuffer(0));
	
	m_strItemOld.Empty();
	
	return TRUE;
}
