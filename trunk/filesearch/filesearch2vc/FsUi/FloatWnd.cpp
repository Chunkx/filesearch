// FloatWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "FloatWnd.h"
#include "FsUi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatWnd dialog


CFloatWnd::CFloatWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CFloatWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFloatWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatWnd)
	DDX_Control(pDX, IDC_LOGO, m_Logo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloatWnd, CDialog)
	//{{AFX_MSG_MAP(CFloatWnd)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_HIDE, OnHide)
	ON_COMMAND(ID_MENU_NEW_INDEX, OnMenuNewIndex)
	ON_COMMAND(ID_MENU_SYS_SET, OnMenuSysSet)
	ON_COMMAND(ID_MENU_WEB_QUERY, OnMenuWebQuery)
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_MENU_TRANS_100, OnMenuTrans100)
	ON_COMMAND(ID_MENU_TRANS_20, OnMenuTrans20)
	ON_COMMAND(ID_MENU_TRANS_40, OnMenuTrans40)
	ON_COMMAND(ID_MENU_TRANS_60, OnMenuTrans60)
	ON_COMMAND(ID_MENU_TRANS_80, OnMenuTrans80)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_LOGO, OnLogo)
	ON_COMMAND(ID_MENU_QUICK_QUERY, OnMenuQuickQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatWnd message handlers

BOOL CFloatWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
#if 1
	CBitmap m_Bitmap;
	HBITMAP hBitmap = m_Logo.GetBitmap();
	ASSERT(hBitmap);
	
	m_Bitmap.Attach(hBitmap);
	BITMAP bmp;
	m_Bitmap.GetBitmap(&bmp);
	
	int nX = bmp.bmWidth;
	int nY = bmp.bmHeight;
	
	CRect rcDlgs;
	GetWindowRect(rcDlgs);   //�õ��Ի����Rect �Ի���Ĵ�С
	ScreenToClient(rcDlgs);             //����Ļ��ֵת����Ӧ��ʵ�ʵ�ֵ 
#endif
	int   cx   =   GetSystemMetrics(   SM_CXSCREEN   );  //�����Ļ�ķֱ���
	int   cy   =   GetSystemMetrics(   SM_CYSCREEN   );   
	
	int x = cx*0.9;
	int y = cy*0.1;

//  	int nX = 32;
// 	int nY = 32;
	MoveWindow(x,y,nX,nY);	
	m_Logo.MoveWindow(0,0,nX,nY);
	//CenterWindow();
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

	m_Bitmap.Detach();
	
	//����WS_EX_LAYERED��չ����
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CFloatWnd::OnUpdateTransparent(int iTransparent)
{
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if(hInst)
	{
		typedef BOOL (WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
		SLWA pFun = NULL;
		//ȡ��SetLayeredWindowAttributes����ָ�� 
		pFun = (SLWA)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(pFun)
		{
			pFun(m_hWnd,0,iTransparent,2);
		}
		FreeLibrary(hInst); 
	}
}

void CFloatWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu m_Right;
	m_Right.LoadMenu(IDR_MENU_FLOATWND);
	CMenu *pSub = m_Right.GetSubMenu(0);
	
	ClientToScreen(&point);
	pSub->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this,NULL);
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CFloatWnd::OnHide() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_HIDE);
}

void CFloatWnd::OnMenuNewIndex() 
{
	// TODO: Add your command handler code here
	FsCreateIndex();
}

void CFloatWnd::OnMenuSysSet() 
{
	// TODO: Add your command handler code here
	FsSetSheet();
}

void CFloatWnd::OnMenuWebQuery() 
{
	// TODO: Add your command handler code here
	//����˵�������...����Ϣ http://localhost:6666/slfile/load
	ShellExecute(NULL, "open", "http://localhost:6666/slfile/load", NULL, NULL,SW_SHOWNORMAL); 	
}

UINT CFloatWnd::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nHitTest = CDialog::OnNcHitTest(point);
	
	if (nHitTest == HTCLIENT &&
		::GetAsyncKeyState(MK_LBUTTON) < 0) // ������������£�GetAsyncKeyState�����ķ���ֵС��	
		nHitTest = HTCAPTION;
	
	return nHitTest;

//	return CDialog::OnNcHitTest(point);
}

#define MAX_TRANS_PARENT	255
void CFloatWnd::OnMenuTrans100() 
{
	// TODO: Add your command handler code here
	int nTransParent = MAX_TRANS_PARENT*1;
	OnUpdateTransparent(nTransParent);
}



void CFloatWnd::OnMenuTrans20() 
{
	// TODO: Add your command handler code here
	int nTransParent = MAX_TRANS_PARENT*0.2;
	OnUpdateTransparent(nTransParent);
}

void CFloatWnd::OnMenuTrans40() 
{
	// TODO: Add your command handler code here
	int nTransParent = MAX_TRANS_PARENT*0.4;
	OnUpdateTransparent(nTransParent);	
}

void CFloatWnd::OnMenuTrans60() 
{
	// TODO: Add your command handler code here
	int nTransParent = MAX_TRANS_PARENT*0.6;
	OnUpdateTransparent(nTransParent);		
}

void CFloatWnd::OnMenuTrans80() 
{
	// TODO: Add your command handler code here
	int nTransParent = MAX_TRANS_PARENT*0.8;
	OnUpdateTransparent(nTransParent);		
}

void CFloatWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	FsFastSearch();
	CDialog::OnLButtonDown(nFlags, point);
}

void CFloatWnd::OnLogo() 
{
	// TODO: Add your control notification handler code here
//	FsFastSearch();
}

void CFloatWnd::OnMenuQuickQuery() 
{
	// TODO: Add your command handler code here
	FsFastSearch();	
}
