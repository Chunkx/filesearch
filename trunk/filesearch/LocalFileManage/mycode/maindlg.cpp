#include "stdafx.h"
#include "maindlg.h"
#include "resource.h"


CMainDlg::CMainDlg():CBkDialogImpl<CMainDlg>(IDR_MAIN_DIALOG) // ��������ؽ�����
{
	BkString::Load(IDR_STRING_DEF); // �����ַ���
	BkSkin::LoadSkins(IDR_SKIN_DEF); // ����Ƥ��
	BkStyle::LoadStyles(IDR_STYLE_DEF); // ���ط��
}

CMainDlg::~CMainDlg()
{

}

// �رմ���
LRESULT CMainDlg::OnClose()
{
	EndDialog(0);
	return 0;
}

// ��󻯴���
LRESULT CMainDlg::OnMaxWindow()
{
	if (WS_MAXIMIZE == (GetStyle() & WS_MAXIMIZE))
	{		
		SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);
	}
	else
	{		
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION, 0);
	}
	return 0;
}

// ��С������
LRESULT CMainDlg::OnMinWindow()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return 0;
}

// �ļ��˵�
void CMainDlg::OnBnClickedPopupMenu()
{
	
	initFileMenu();
	if( m_file_wndMenu.IsWindow() )
	{
		CRect	rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		m_file_wndMenu.PopUp( rc.left, rc.top + 50);
	}
}

// �鿴�˵�
void CMainDlg::OnViewMenu()
{
	CPoint	pos;
	CRect	rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	pos.x = rc.right - 950;
	pos.y = rc.top + 50;

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_VIEWMENU);

	CMenuHandle Menu = menuPopup.GetSubMenu(0);;
	
	//ATLVERIFY(GetCursorPos(&pos));
	Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN, pos.x, pos.y, m_hWnd);
}
// ���߲˵�
void CMainDlg::OnToolMenu()
{
	CPoint	pos;
	CRect	rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	pos.x = rc.right - 890;
	pos.y = rc.top + 50;

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_TOOLMENU);

	CMenuHandle Menu = menuPopup.GetSubMenu(0);;
	
	//ATLVERIFY(GetCursorPos(&pos));
	Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN, pos.x, pos.y, m_hWnd);
}

// �����˵�
void CMainDlg::OnHelpMenu()
{
	CPoint	pos;
	CRect	rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	pos.x = rc.right - 830;
	pos.y = rc.top + 50;

	CMenu menuPopup;
	menuPopup.LoadMenu(IDR_HELPMENU);

	CMenuHandle Menu = menuPopup.GetSubMenu(0);;
	
	//ATLVERIFY(GetCursorPos(&pos));
	Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN, pos.x, pos.y, m_hWnd);
}



// ϵͳͼ��
void CMainDlg::OnSysCommand(UINT nID, CPoint point)
{
	SetMsgHandled(FALSE);

	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		SetMsgHandled(TRUE);
		OnClose();
		break;
	case SC_RESTORE:
		{
			DWORD dwStyle = GetStyle();
			if (WS_MINIMIZE == (dwStyle & WS_MINIMIZE))
				break;

			if (WS_MAXIMIZE == (dwStyle & WS_MAXIMIZE))
			{
				SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_max");
				break;
			}
		}
	case SC_MAXIMIZE:
		SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_normal");
		break;
	default:
		break;
	}
}