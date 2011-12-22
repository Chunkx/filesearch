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

	initViewMenu();
	if( m_view_wndMenu.IsWindow() )
	{
		CRect	rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		m_view_wndMenu.PopUp( rc.left + 75, rc.top + 50);
	}
}
// ���߲˵�
void CMainDlg::OnToolMenu()
{
	initToolMenu();
	if( m_tool_wndMenu.IsWindow() )
	{
		CRect	rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		m_tool_wndMenu.PopUp( rc.left + 135, rc.top + 50);
	}
}

// �����˵�
void CMainDlg::OnHelpMenu()
{
	initHelpMenu();
	if( m_help_wndMenu.IsWindow() )
	{
		CRect	rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		m_help_wndMenu.PopUp( rc.left + 195, rc.top + 50);
	}
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