// FastSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "FastSearchDlg.h"
#include "sltFastSearchThread.h"

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastSearchDlg dialog


CFastSearchDlg::CFastSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastSearchDlg)
	m_strKey = _T("");
	m_bDestory = TRUE;
	m_bCommboxRecentStatus = TRUE;
//	m_pSearchThread = NULL;
	m_bEditForce = FALSE;
	//}}AFX_DATA_INIT
}


void CFastSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastSearchDlg)
	DDX_Control(pDX, IDC_EDIT_SEARCH_KEY, m_keyEdit);
	DDX_Control(pDX, IDC_STATIC_FIND, m_static_find);
	DDX_Control(pDX, IDC_COMBO_PATH_RECENT, m_BoxListRecent);
	DDX_Control(pDX, IDC_COMBO_PATH, m_BoxList);
	DDX_Text(pDX, IDC_EDIT_SEARCH_KEY, m_strKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFastSearchDlg)
	ON_CBN_EDITCHANGE(IDC_COMBO_PATH, OnEditchangeComboPath)
	ON_EN_CHANGE(IDC_EDIT_SEARCH_KEY, OnChangeEditSearchKey)
	ON_CBN_SELCHANGE(IDC_COMBO_PATH, OnSelchangeComboPath)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_CBN_SELCHANGE(IDC_COMBO_PATH_RECENT, OnSelchangeComboPathRecent)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_KEY, OnKillfocusEditSearchKey)
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
	ON_WM_KILLFOCUS()
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_KEY, OnSetfocusEditSearchKey)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS_MSG, OnProgressChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastSearchDlg message handlers

void CFastSearchDlg::OnEditchangeComboPath() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_BoxList.GetCurSel();		//�õ���ѡ����������
	CString m_strtemp;						//��ŵõ��ı༭������
	m_BoxList.GetLBText(nIndex,m_strtemp);	//�õ���ѡ�����ݵ�����
}

void CFastSearchDlg::SetWinPos()
{
	CRect rcDlgs;
	GetWindowRect(rcDlgs);   //�õ��Ի����Rect �Ի���Ĵ�С
	ScreenToClient(rcDlgs);             //����Ļ��ֵת����Ӧ��ʵ�ʵ�ֵ 
	
	int   cx   =   GetSystemMetrics(   SM_CXSCREEN   );  //�����Ļ�ķֱ���
	int   cy   =   GetSystemMetrics(   SM_CYSCREEN   );   

	//cx cy,������Ļ�����½ǵ�x,y��ֵ 
	MoveWindow(cx-rcDlgs.Width(),cy-rcDlgs.Height(),rcDlgs.Width(),rcDlgs.Height(),TRUE);   // 

	//MoveWindow�Ĳ���ǰ�����ǶԻ����x,yλ��  
	//���ĸ��ǶԻ���Ĵ�С ������Ը����ùܣ� 
	SetWindowPos(&wndTopMost,cx-rcDlgs.Width(),cy-rcDlgs.Height()-30,rcDlgs.Width(),rcDlgs.Height(),SWP_NOSIZE); 
}


void CFastSearchDlg::SetComboxPos(BOOL bRecent)
{

	RECT rcDlgs;
	m_BoxList.GetWindowRect(&rcDlgs);   //�õ��Ի����Rect �Ի���Ĵ�С
	ScreenToClient(&rcDlgs);             //����Ļ��ֵת����Ӧ��ʵ�ʵ�ֵ 
	
	//cx cy,������Ļ�����½ǵ�x,y��ֵ 
	if (m_bCommboxRecentStatus == TRUE)
	{
		//��ǰ״̬����ѡ�С�����ĵ���״̬
		if (bRecent == TRUE)
		{
			//�ɡ�����ĵ���-���������ĵ���
			return ;
		}else
		{
			//�ɡ�����ĵ���-��������Ŀ¼��
			m_BoxList.MoveWindow(rcDlgs.left,rcDlgs.top,(rcDlgs.right - rcDlgs.left + 45),(rcDlgs.bottom - rcDlgs.top),TRUE);   // 
			m_bCommboxRecentStatus = FALSE;
			//��������ĵ�commbox
			m_BoxListRecent.ShowWindow(SW_HIDE);
		}
	}else{
		//��ǰ״̬����ѡ�С�����Ŀ¼��״̬
		if (bRecent == TRUE)
		{
			//�ɡ�����Ŀ¼��-���������ĵ���
			m_BoxList.MoveWindow(rcDlgs.left,rcDlgs.top,(rcDlgs.right - rcDlgs.left - 45),(rcDlgs.bottom - rcDlgs.top),TRUE);   // 
			m_bCommboxRecentStatus = TRUE;
			//��ʾ����ĵ�commbox
			m_BoxListRecent.ShowWindow(SW_SHOWNORMAL);
		}else
		{
			//�ɡ�����Ŀ¼��-��������Ŀ¼��
			return ;
		}
	}
}




void CFastSearchDlg::SetStaticFindPos()
{
	
	RECT rcDlgs;
	m_static_find.GetWindowRect(&rcDlgs);   //�õ��Ի����Rect �Ի���Ĵ�С
	ScreenToClient(&rcDlgs);             //����Ļ��ֵת����Ӧ��ʵ�ʵ�ֵ 
	
	m_static_find.MoveWindow(rcDlgs.left,rcDlgs.top,(rcDlgs.right - rcDlgs.left + 3),(rcDlgs.bottom - rcDlgs.top + 3),TRUE);   // 	
}

#define EDIT_TEXT	"���� �ĵ����ݹؼ���"
BOOL CFastSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_FIND);
	SetIcon(m_hIcon,TRUE); //����Ϊ��ͼ��

	
//    m_FilenameEdit.SubclassDlgItem(IDC_EDIT_SEARCH_KEY, this);
	
//	CString strData = "test";
//	m_FilenameEdit.SetFilename(strData);

	// TODO: Add extra initialization here
	m_keyEdit.SetIcon(IDI_ICON_FAST_SEARCH2);
	SetDlgItemText(IDC_EDIT_SEARCH_KEY, EDIT_TEXT);

	SetWindowText(g_lag.LoadString("title.fastsearch"));

	sltFastSearchThread::newInstance();
	sltFastSearchThread::getInstance()->Init(this->GetSafeHwnd());
	sltFastSearchThread::getInstance()->startup();

	SetWinPos();
	m_BoxList.ResetContent(); // Clean up all contents
	if (m_agent.GetAllPath() == 0)
	{
		int nSize = m_agent.m_PathList.size();
		for (int i =0 ; i < nSize; i++)
		{
			m_BoxList.AddString(m_agent.m_PathList[i].szPath);
		}
	}

	m_BoxList.InsertString(0, g_lag.LoadString("combox.recent"));
	m_BoxList.InsertString(1,g_lag.LoadString("label.fastserlist"));
	m_BoxList.SetCurSel(0);

	//��ʼ��������ĵ�boxlist
	m_BoxListRecent.InsertString(0, g_lag.LoadString("combox.recent3day"));
	m_BoxListRecent.InsertString(1, g_lag.LoadString("combox.recent1week"));
	m_BoxListRecent.InsertString(2, g_lag.LoadString("combox.recent2week"));
	m_BoxListRecent.InsertString(3, g_lag.LoadString("combox.recent1mon"));
	m_BoxListRecent.SetCurSel(0);

	CreateTaskPanel();

	//excel, word,pdf,txt,ppt,html
	//ѭ����ʾgroup
	char szTooltip[MAX_PATH*2] = {0};
//	sprintf(szTooltip,"���� ��չ��/��£�� �����б�\r\n�����б�:\r\n     ��������������ĵ�\r\n     ���Ҽ����������ĵ����ڵ�Ŀ¼");
	AddToolboxGroup(1, "word", szTooltip, IDI_ICON_WORD);
	AddToolboxGroup(2, "excel", szTooltip, IDI_ICON_EXCEL);
	AddToolboxGroup(3, "ppt", szTooltip, IDI_ICON_PPT);
	AddToolboxGroup(4, "pdf", szTooltip, IDI_ICON_PDF);
	AddToolboxGroup(5, "txt", szTooltip, IDI_ICON_TXT);
	AddToolboxGroup(6, "html", szTooltip, IDI_ICON_HTML);

#if 0
	std::vector<string> desp;
	std::string str1 = "desp1";
	std::string str2 = "desp2";
	desp.push_back(str1);
	desp.push_back(str2);
	AddLinkItem(1, 1,1,"D:\\slfile�����ļ�\\����dir3\\2009 ��������Ŀ������.xls", desp);
	AddLinkItem(1, 1,1,"D:\\slfile�����ļ�\\����dir3\\1 2.doc", desp);
	AddLinkItem(1, 2,1,"d:\\1 2.txt", desp);
	AddLinkItem(2, 1,1,"3.xls", desp);
	AddLinkItem(3, 1,1,"3.ppt", desp);
#endif

	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_WORD, IDI_ICON_WORD);
	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_EXCEL, IDI_ICON_EXCEL);
	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_PPT, IDI_ICON_PPT);
	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_PDF, IDI_ICON_PDF);
	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_TXT, IDI_ICON_TXT);
	m_wndTaskPanel.GetImageManager()->SetIcon(IDI_ICON_HTML, IDI_ICON_HTML);
	m_wndTaskPanel.SetGroupIconSize( CSize(16, 16));
	m_wndTaskPanel.SetIconSize( CSize(16, 16));

	UpdateGroupsCaption();

//	SetStaticFindPos();

	m_fileopen.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastSearchDlg::OnEventNotify()
{
	BOOL bRectAll = FALSE;
	//��ȡ�����Key��SelBox
	CString strKey;
 	GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKey);

	if (strKey.GetLength() == 0)
	{
		//�����Ϊ�գ��������ʾ�б�
		//���LinkItem
		ClearGroupsItems();

		//��������group������������
		UpdateGroupsCaption();

		return ;
	}

	int nIndex = m_BoxList.GetCurSel();		//�õ���ѡ����������
	CString strtemp;						//��ŵõ��ı༭������
	m_BoxList.GetLBText(nIndex,strtemp);	//�õ���ѡ�����ݵ�����

	//����·��,ƥ�����id
	int nId = m_agent.GetPathIndex(strtemp.GetBuffer(0));
	char szID[MAX_PATH] = {0};
	itoa(nId, szID, 10);

	FastItem fast;
	memset(&fast, NULL, sizeof(FastItem));
	strcpy(fast.szKey, strKey.GetBuffer(0));
	strcpy(fast.szID, szID);
	if (nId == -1)
	{	
		SetComboxPos(TRUE);
		//��ȡ����ĵ�commbox����
		int nRecIndex = m_BoxListRecent.GetCurSel();		//�õ���ѡ����������
		CString strRectemp;						//��ŵõ��ı༭������
		m_BoxListRecent.GetLBText(nRecIndex,strRectemp);	//�õ���ѡ�����ݵ�����
		//����typeֵ
		if( strRectemp == g_lag.LoadString("combox.recent3day"))
			strcpy(fast.szType, "3");
		if( strRectemp == g_lag.LoadString("combox.recent1week"))
			strcpy(fast.szType, "7");
		if( strRectemp == g_lag.LoadString("combox.recent2week"))
			strcpy(fast.szType, "14");
		if( strRectemp == g_lag.LoadString("combox.recent1mon"))
			strcpy(fast.szType, "30");

		//��ѡ������ĵ���ʱ,�鿴�༭��������Ƿ��ǿո�
		CString strTemp = strKey;
		int nLen = strTemp.GetLength();
		if (nLen)
		{
			strTemp.TrimLeft(" ");
			nLen = strTemp.GetLength();
			if (nLen == 0)
			{
				//�����ȫ�ǿո�
				bRectAll = TRUE;
			}
		}
		
	}else{
		SetComboxPos(FALSE);
	}

	if(strKey == EDIT_TEXT)
		return ;

	if (bRectAll)
	{
		//ѡ������ĵ���,�༭��������Ŀո���ֱ�Ӳ��Ҽ�¼
		OnProgressChangeRecent(fast);
	}else
	{
		//����ļ����ڣ����¼�µ�ǰkey
		if (m_agent.IsKeyFileExist())
		{
			sltFastSearchThread::getInstance()->PostMsg(fast);
			return ;
		}
		
		sltFastSearchThread::getInstance()->DoLog(fast);
	}
}

void CFastSearchDlg::OnChangeEditSearchKey() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	//�����ȡ��ֵ
	m_bEditForce = TRUE;
	CString strKey;
	GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKey);
	if(strKey == EDIT_TEXT)
	{
		SetDlgItemText(IDC_EDIT_SEARCH_KEY, "");
	}

	OnEventNotify();

}

//ȴ��Ŀ¼�¼�
void CFastSearchDlg::OnSelchangeComboPath() 
{
	// TODO: Add your control notification handler code here
	
	OnEventNotify();
}

void CFastSearchDlg::OnSelchangeComboPathRecent() 
{
	// TODO: Add your control notification handler code here
	OnEventNotify();	
}

//�����ļ�����¼�
void CFastSearchDlg::OnProgressChange(WPARAM wParam, LPARAM lParam)
{

	//���LinkItem
	ClearGroupsItems();

	//��T_Result��ȡ������
	if( m_agent.GetSearchRecords() == 0)
	{
		//��ʾ�ڽ�����
		int nCount = m_agent.m_RecList.size();
		for (int i = 0; i < nCount; i++)
		{
			int nID = GetFileID(m_agent.m_RecList[i].szFileType);
			//////////////////////////////////////////////////////////////////////////
			//��ʾ������
			AddLinkItemEx(nID, i,1,m_agent.m_RecList[i].szFileName,m_agent.m_RecList[i].DespList, m_agent.m_RecList[i].szFilePath);
		}
	}

	//��������group������������
	UpdateGroupsCaption();

}


//��������ĵ�������ո���ʾ����
void CFastSearchDlg::OnProgressChangeRecent(FastItem item)
{	
	//���LinkItem
	ClearGroupsItems();
	
	//��T_Result��ȡ������
	int nDays = atoi(item.szType);
	if( m_agent.GetSearchRecords_Recent(nDays) == 0)
	{
		//��ʾ�ڽ�����
		int nCount = m_agent.m_RecList.size();
		for (int i = 0; i < nCount; i++)
		{
			int nID = GetFileID(m_agent.m_RecList[i].szFileType);
			//////////////////////////////////////////////////////////////////////////
			//��ʾ������
			AddLinkItemEx(nID, i,1,m_agent.m_RecList[i].szFileName,m_agent.m_RecList[i].DespList, m_agent.m_RecList[i].szFilePath, FALSE);
		}
	}
	
	//��������group������������
	UpdateGroupsCaption();
	
}

int CFastSearchDlg::GetFileID(char* szFileType)
{
	if (strlen(szFileType) == 0)
		return -1;

	if (!strcmp(szFileType, WORD_NAME))
		return 1;
	if (!strcmp(szFileType, EXCEL_NAME))
		return 2;
	if (!strcmp(szFileType, PPT_NAME))
		return 3;
	if (!strcmp(szFileType, PDF_NAME))
		return 4;
	if (!strcmp(szFileType, TXT_NAME))
		return 5;
	if (!strcmp(szFileType, HTML_NAME))
		return 6;
	
	return -1;

}

BOOL CFastSearchDlg::CreateTaskPanel()
{
	
	if (!m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP, CRect(5, 0, 358, 482), this, 1))
		return FALSE;
	
	m_wndTaskPanel.SetOwner(this);

	//	m_wndTaskPanel.GetImageManager()->SetIcons(IDB_TOOLBOXICONS, 0, 0, CSize(16, 16));
	m_wndTaskPanel.SetIconSize(CSize(25, 25));
	m_wndTaskPanel.SetBehaviour(xtpTaskPanelBehaviourExplorer/*xtpTaskPanelBehaviourToolbox*/);
	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOfficeXPPlain/*xtpTaskPanelThemeToolbox*/);
	m_wndTaskPanel.SetSelectItemOnFocus(TRUE);
	m_wndTaskPanel.SetHotTrackStyle(xtpTaskPanelHighlightText);
	m_wndTaskPanel.AllowDrag(xtpTaskItemAllowDragDefault);

	//֧��html��ʽ��tooltip��ʾ
	m_wndTaskPanel.GetToolTipContext()->SetStyle(xtpToolTipHTML);

	int nMargin = 0;
	m_wndTaskPanel.GetPaintManager()->m_rcGroupInnerMargins.SetRect(nMargin, nMargin, nMargin, nMargin);
	m_wndTaskPanel.Reposition();
 
	return TRUE;
}

/*
����:
nID:			Group�����ID			
lpszCaption:	Group�����caption
lpszTooltip:	Group����������������ʾ��caption
nIconIndex:     Group����ͼ����������
*/
void CFastSearchDlg::AddToolboxGroup(UINT nID, LPCTSTR lpszCaption, LPCTSTR lpszTooltip/* = ""*/, int nIconIndex/* = 0*/)
{
	CXTPTaskPanelGroup* pFolder = m_wndTaskPanel.AddGroup(nID, nIconIndex);	
	pFolder->SetCaption(lpszCaption);
	pFolder->SetTooltip(lpszTooltip);
	m_listMap[nID].pGroup = pFolder;
	m_listMap[nID].nItemSize = 0;
}

int imageid[6] = {	IDI_ICON_WORD,
					IDI_ICON_EXCEL,
					IDI_ICON_PPT,
					IDI_ICON_PDF,
					IDI_ICON_TXT,
					IDI_ICON_HTML
					};
void CFastSearchDlg::AddLinkItem(UINT nFolderID, UINT nItemID, int nIconIndex, LPCTSTR lpszCaption, std::vector<DespInfo> DespList,LPCTSTR lpszTooltip/* = ""*/,BOOL bShowTextItem/* = FALSE*/)
{

	CXTPTaskPanelGroup* pFolder = m_listMap[nFolderID].pGroup;
	if (!pFolder)
		return ;

	m_listMap[nFolderID].nItemSize++;
	CXTPTaskPanelGroupItem* pPointer = pFolder->AddLinkItem(nItemID, 0/*imageid[nFolderID-1]*/);
	pPointer->SetTooltip(lpszTooltip);
	int nSize = DespList.size();
	if (nFolderID == 6)
	{
		//html����ʾ�������ݣ�ֻ��ʾ����
		int nNewSize = strlen(lpszCaption) + 5;
		char* pCaption = new char[nNewSize + 1];
		memset(pCaption, NULL, nNewSize + 1);
		if (bShowTextItem)
		{
			sprintf(pCaption,"%s (%d)",lpszCaption, nSize);
		}else
			sprintf(pCaption,"%s",lpszCaption);

		pPointer->SetCaption(pCaption);
	
		if (pCaption)
		{
			delete[] pCaption;
			pCaption = NULL;
		}
	}else
	{
		if (bShowTextItem)
		{
			for (int i = 0; i < nSize; i++)
			{
				std::string strTextItem = "              " + DespList[i].strDesp;
				pFolder->AddTextItem(strTextItem.c_str());
			}
		}

		pPointer->SetCaption(lpszCaption);
	}

	pPointer->SetItemSelected(TRUE);
	pPointer->AllowDrag(FALSE);
	pPointer->AllowDrop(FALSE);
}

void CFastSearchDlg::AddLinkItemEx(UINT nFolderID, UINT nItemID, int nIconIndex, LPCTSTR lpszCaption, std::vector<DespInfo> DespList,LPCTSTR lpszTooltip/* = ""*/,BOOL bShowTextItem/* = FALSE*/)
{	
	CXTPTaskPanelGroup* pFolder = m_listMap[nFolderID].pGroup;
	if (!pFolder)
		return ;
	
	m_listMap[nFolderID].nItemSize++;
	CXTPTaskPanelGroupItem* pPointer = pFolder->AddLinkItem(nItemID, 0/*imageid[nFolderID-1]*/);
	pPointer->SetTooltip(lpszTooltip);
	int nSize = DespList.size();
	if (nFolderID == 6)
	{
		//html����ʾ�������ݣ�ֻ��ʾ����
		int nNewSize = strlen(lpszCaption) + 5;
		char* pCaption = new char[nNewSize + 1];
		memset(pCaption, NULL, nNewSize + 1);
		if (bShowTextItem)
		{
			sprintf(pCaption,"%s (%d)",lpszCaption, nSize);
		}else
			sprintf(pCaption,"%s",lpszCaption);
		
		pPointer->SetCaption(pCaption);
		
		if (pCaption)
		{
			delete[] pCaption;
			pCaption = NULL;
		}
	}else
	{
		pPointer->SetCaption(lpszCaption);
		if (bShowTextItem)
		{
			for (int i = 0; i < nSize; i++)
			{
				std::string strTextItem = "      " + DespList[i].strDesp;
				pPointer = pFolder->AddLinkItem(nItemID, 0/*imageid[nFolderID-1]*/);
				pPointer->SetCaption(strTextItem.c_str());
				pPointer->SetTooltip(DespList[i].strContent.c_str());
				pPointer->SetTextColor(RGB(0, 0, 0));
				pPointer->SetDragText(lpszTooltip);
			}
		}
	}
	
// 	pPointer->SetItemSelected(TRUE);
// 	pPointer->AllowDrag(FALSE);
// 	pPointer->AllowDrop(FALSE);
}

//���LinkItem
void CFastSearchDlg::ClearGroupsItems() 
{
	// TODO: Add your control notification handler code here
	int nSize = m_listMap.size();
	for (int i = 1; i <= nSize; i++)
	{
		CXTPTaskPanelGroup* pFolder = m_listMap[i].pGroup;
		if (!pFolder)
			continue;
		
		pFolder->GetItems()->Clear(TRUE);
		m_listMap[i].nItemSize = 0;

	}
}

//��������group��caption
void CFastSearchDlg::UpdateGroupsCaption() 
{
	// TODO: Add your control notification handler code here
	int nSize = m_listMap.size();
	for (int i = 1; i <= nSize; i++)
	{
		CXTPTaskPanelGroup* pFolder = m_listMap[i].pGroup;
		if (!pFolder)
			continue;

		std::string strCaption;
		switch(i)
		{
		case 1:
			strCaption = "word  (";
			break;
		case 2:
			strCaption = "excel (";
			break;
		case 3:
			strCaption = "ppt     (";
			break;
		case 4:
			strCaption = "pdf     (";
			break;
		case 5:
			strCaption = "txt     (";
			break;
		case 6:
			strCaption = "html  (";
			break;
		default:
			break ;
		}
		
		TCHAR szCaption[MAX_PATH] = {0};
		sprintf(szCaption, "%s%d)",strCaption.c_str(), m_listMap[i].nItemSize);
		pFolder->SetCaption(szCaption);	
	}
}

void CFastSearchDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	sltFastSearchThread::getInstance()->shutdownflag();
}

HBRUSH CFastSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd-> GetDlgCtrlID() == IDC_EDIT_SEARCH_KEY) 
	{ 
		if (!m_bEditForce)
		{
			pDC-> SetTextColor(RGB(192, 192, 192)); 
		}else
		{
			pDC-> SetTextColor(RGB(0, 0, 0)); 
		}
	} 	

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CFastSearchDlg::OnTaskPanelClickDownEvent(WPARAM wParam, LPARAM lParam)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
	TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
	
	//if (IsToggleButtons())
	{
		pItem->SetItemSelected(!pItem->IsItemSelected());
	}

	CString strFileName = pItem->GetCaption();
	//���item���ļ���������������
	//���ǰ3���ַ��Ƿ�Ϊ���ַ�
	CString strFirst = strFileName.Left(3);
	if (strFirst != "   ")
	{
		//������ĵ�����
		CString strGroupCaption = pItem->GetItemGroup()->GetCaption();
		if (strGroupCaption.Find(HTML_NAME, 0) != -1)
		{  
			//html ,ȥ���ļ������(%d)
			std::string sFileName = strFileName.GetBuffer(0);
			int nPos = sFileName.find_last_of('(');
			sFileName = sFileName.substr(0, nPos - 1);
			strFileName = sFileName.c_str();
		}
		
		//find file full path from file name
		//���ַ�ʽ���ܵ��£�һ���ļ�������Ӧ���·�������ȡ����ȷ
		//	std::string strFilePath = m_agent.GetFilePathFromName(strFileName.GetBuffer(0));
		std::string strFilePath = pItem->GetTooltip().GetBuffer(0);
		//�������������¼������ĵ�
		if (XTP_TPN_CLICK == wParam && strFilePath.size() != 0)
		{	
			//�������������¼������ĵ�
			m_bDestory = FALSE;
			
			char szPath[MAX_PATH] = {0};
			CString strGroupName;
			if (strGroupCaption.Find(HTML_NAME, 0) != -1)
			{
				strGroupName = HTML_NAME;
			}
			if (strGroupCaption.Find(WORD_NAME, 0) != -1)
			{
				strGroupName = WORD_NAME;
			}
			if (strGroupCaption.Find(EXCEL_NAME, 0) != -1)
			{
				strGroupName = EXCEL_NAME;
			}
			if (strGroupCaption.Find(PPT_NAME, 0) != -1)
			{
				strGroupName = PPT_NAME;
			}
			if (strGroupCaption.Find(PDF_NAME, 0) != -1)
			{
				strGroupName = PDF_NAME;
			}
			if (strGroupCaption.Find(TXT_NAME, 0) != -1)
			{
				strGroupName = TXT_NAME;
			}
			
			//���򿪵��ļ��Ƿ����
			if (PathFileExists(strFilePath.c_str()) == FALSE)
			{
				MessageBox(g_lag.LoadString("message.searchfilenoexist"),g_lag.LoadString("title.fastsearch"),MB_OK | MB_ICONWARNING);
				
				return ;
			}
			
			if(m_setAgent.GetSoftPath(strGroupName.GetBuffer(0),szPath) == 0 )
			{
				//�򿪽��̲������ļ�ȫ·���У�������С� �����ո񣩣����·�������ü��ϡ���
				//����޷����ļ�·���д��пո���ļ�
				std::string strFilePathOpen = "\"";
				strFilePathOpen += strFilePath;
				strFilePathOpen += "\"";
				flog.Print(LL_DEBUG_INFO,"[info]ShellExecute softpath=%s, filepath=%s\r\n",szPath,strFilePathOpen.c_str());
			
				ShellExecute(NULL, "open", szPath, strFilePathOpen.c_str(), "", SW_SHOW );
			}else
			{
				flog.Print(LL_DEBUG_INFO,"[info]ShellExecute filepath=%s\r\n",strFilePath.c_str());
				ShellExecute(NULL, "open", strFilePath.c_str(), NULL, NULL, SW_SHOW);
			}
		}
		
		//�����Ҽ�������¼������ĵ����ڵ��ļ���
		if (XTP_TPN_RCLICK == wParam && strFilePath.size() != 0)
		{
			//�����Ҽ�������¼������ĵ����ڵ��ļ���
			//�����ļ�����ȡ�ļ����ڵ�·��
			m_bDestory = FALSE;
			
			//���ĵ����ڵ�Ŀ¼����ѡ����ĵ�
			char szParam[_MAX_PATH+64]={0};
			strcpy(szParam,"/e,/select, ");
			strcat(szParam,strFilePath.c_str());
			ShellExecute(NULL,"open","explorer",szParam,NULL,SW_SHOW);		
		}	
	}else
	{
		//��ʱֻ���������Ϣ
		if (XTP_TPN_CLICK != wParam)
			return ;

		//�������������������
		//��ȡ�ļ�·��,ҳ�����ؼ���
		CString strDesp = strFileName;
		strDesp.TrimLeft();	//��27ҳ
		CString strDespFileName = pItem->GetDragText();
		CString strKeyWords;
		GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKeyWords);

		//���򿪵��ļ��Ƿ����
		if (PathFileExists(strDespFileName.GetBuffer(0)) == FALSE)
		{
			MessageBox(g_lag.LoadString("message.searchfilenoexist"),g_lag.LoadString("title.fastsearch"),MB_OK | MB_ICONWARNING);
			
			return ;
		}

		//��ȡ�ĵ�����
		CString strGroupCaption = pItem->GetItemGroup()->GetCaption();

		//�������������¼������ĵ�
		if (strDespFileName.GetLength() != 0)
		{	
			//�������������¼������ĵ�
			m_bDestory = FALSE;
			
			if (m_fileopen.m_bInit == FALSE)
			{
				if(m_fileopen.Init() == FALSE)
					return ;
			}
			if (strGroupCaption.Find(HTML_NAME, 0) != -1)
			{

			}
			if (strGroupCaption.Find(WORD_NAME, 0) != -1)
			{
				//��word�ĵ�
				//���ݵڼ�ҳ��������ҳ����//��27ҳ
				int nPage, nStart, nEnd;
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("ҳ",0);
				if (nEnd <= nStart)
				{
					nPage = 1;
				}else
				{
					CString strPage = strDesp.Mid(nStart+2, nEnd - 2);
					nPage = atoi(strPage.GetBuffer(0));	
				}

				m_fileopen.m_fnopenWordFile(strDespFileName.GetBuffer(0), nPage, strKeyWords.GetBuffer(0));
			}
			if (strGroupCaption.Find(EXCEL_NAME, 0) != -1)
			{
				//��excel�ĵ�
				//�����С�ҳ��12��
				int nRow, nStart, nEnd;
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("��",nStart);
				if (nEnd <= nStart)
				{
					nRow = 1;
				}else
				{
					CString strRow = strDesp.Mid(nStart+2, nEnd - 2);
					nRow = atoi(strRow.GetBuffer(0));	
				}

				//����sheet��
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("��",0);
				if (nEnd <= nStart)
				{
					//����ִ�д��ļ�
					return ;
				}
				CString strSheet = strDesp.Mid(nStart+2, nEnd - 2);

				m_fileopen.m_fnopenExcelFile(strDespFileName.GetBuffer(0),strSheet.GetBuffer(0) ,nRow,strKeyWords.GetBuffer(0) );

			}
			if (strGroupCaption.Find(PPT_NAME, 0) != -1)
			{
				//��ppt�ĵ�
				//��1ҳ
				int nPage, nStart, nEnd;
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("ҳ",0);
				if (nEnd <= nStart)
				{
					nPage = 1;
				}else
				{
					CString strPage = strDesp.Mid(nStart+2, nEnd - 2);
					nPage = atoi(strPage.GetBuffer(0));	
				}
				
				m_fileopen.m_fnopenPPTFile(strDespFileName.GetBuffer(0), nPage, strKeyWords.GetBuffer(0));

			}
			if (strGroupCaption.Find(PDF_NAME, 0) != -1)
			{
				int nPage, nStart, nEnd;
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("ҳ",0);
				if (nEnd <= nStart)
				{
					nPage = 1;
				}else
				{
					CString strPage = strDesp.Mid(nStart+2, nEnd - 2);
					nPage = atoi(strPage.GetBuffer(0));	
				}

				//��pdf�ĵ�
				char szPath[MAX_PATH] = {0};
				if(m_setAgent.GetSoftPath(PDF_NAME,szPath) == 0 )
				{
					//�򿪽��̲������ļ�ȫ·���У�������С� �����ո񣩣����·�������ü��ϡ���
					//����޷����ļ�·���д��пո���ļ�
					char szCmd[MAX_PATH*4] = {0};
					sprintf(szCmd, "/a page=%d \"%s\"", nPage, strDespFileName.GetBuffer(0));
// 					std::string strFilePathOpen = "\"";
// 					strFilePathOpen += strDespFileName.GetBuffer(0);
// 					strFilePathOpen += "\"";
					flog.Print(LL_DEBUG_INFO,"[info]ShellExecute softpath=%s, filepath=%s\r\n",szPath,szCmd);
					
					ShellExecute(this->m_hWnd, "open", szPath, szCmd, "",SW_SHOW );
				}else
				{
					flog.Print(LL_DEBUG_INFO,"[info]ShellExecute filepath=%s\r\n",strDespFileName.GetBuffer(0));
					ShellExecute(NULL, "open", strDespFileName.GetBuffer(0), NULL, NULL, SW_SHOW);
				}
			}
			if (strGroupCaption.Find(TXT_NAME, 0) != -1)
			{
				//��txt�ĵ�
				//��3��
				int nPage, nStart, nEnd;
				nStart = strDesp.Find("��", 0);
				nEnd = strDesp.Find("��",0);
				if (nEnd <= nStart)
				{
					nPage = 1;
				}else
				{
					CString strPage = strDesp.Mid(nStart+2, nEnd - 2);
					nPage = atoi(strPage.GetBuffer(0));	
				}
				
				m_fileopen.m_fnopenTxtFile(strDespFileName.GetBuffer(0), nPage, strKeyWords.GetBuffer(0));
			}
		}
	}
}

LRESULT CFastSearchDlg::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case XTP_TPN_CLICK:
		{
			OnTaskPanelClickDownEvent(wParam, lParam);
		}
		break;
	case XTP_TPN_DBLCLICK:
		{
			//////////////////////////////////////////////////////////////////////////
			//add code
//			MessageBox("DBLCLICK Event:");
		}
		break;	
	case XTP_TPN_RCLICK:
		{
			OnTaskPanelClickDownEvent(wParam, lParam);
		}
		break;
	default:
		break;	
	}
	return 0;
}

void CFastSearchDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	//������ڷǼ���״̬����رմ���
	if(nState==WA_INACTIVE && m_bDestory)
	{
		//DestroyWindow();  //DoModal�������᷵��
		CDialog::OnCancel();
	}
}


void CFastSearchDlg::OnKillfocusEditSearchKey() 
{
	// TODO: Add your control notification handler code here
	m_bEditForce = FALSE;
	CString strKey;
	GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKey);
	if(strKey.GetLength() == 0)
	{
		SetDlgItemText(IDC_EDIT_SEARCH_KEY, EDIT_TEXT);
	}
}

void CFastSearchDlg::OnSetfocusEditSearchKey() 
{
	// TODO: Add your control notification handler code here
	m_bEditForce = TRUE;
	CString strKey;
	GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKey);
	if(strKey == EDIT_TEXT)
	{
		OutputDebugString("[slfile]OnSetfocusEditSearchKey");
		SetDlgItemText(IDC_EDIT_SEARCH_KEY, "");
	}	
}
