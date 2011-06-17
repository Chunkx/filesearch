// FastSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "FastSearchDlg.h"
#include "sltFastSearchThread.h"


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
//	m_pSearchThread = NULL;
	//}}AFX_DATA_INIT
}


void CFastSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastSearchDlg)
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
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
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

BOOL CFastSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_FIND);
	SetIcon(m_hIcon,TRUE); //����Ϊ��ͼ��
	//SetIcon(m_hIcon,FALSE);//����ΪСͼ��

	// TODO: Add extra initialization here
	SetWindowText(g_lag.LoadString("title.fastsearch"));
// 	if (NULL == m_pSearchThread)
// 	{
// 		OutputDebugString("AfxBeginThread ");
// 		m_pSearchThread = (CSearchThread*)AfxBeginThread(RUNTIME_CLASS(CSearchThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED , 0);		
// 		m_pSearchThread->m_hParentWnd = this->GetSafeHwnd();
// 		m_pSearchThread->ResumeThread();
// 	}	

	sltFastSearchThread::newInstance();
	sltFastSearchThread::getInstance()->Init(this->GetSafeHwnd());
	sltFastSearchThread::getInstance()->startup();

	SetWinPos();
	if (m_agent.GetAllPath() == 0)
	{
		int nSize = m_agent.m_PathList.size();
		for (int i =0 ; i < nSize; i++)
		{
			m_BoxList.AddString(m_agent.m_PathList[i].szPath);
		}
	}

	m_BoxList.InsertString(0,g_lag.LoadString("label.fastserlist"));
	m_BoxList.SelectString(0,g_lag.LoadString("label.fastserlist"));

	CreateTaskPanel();

	//excel, word,pdf,txt,ppt,html
	//ѭ����ʾgroup
	AddToolboxGroup(1, "word", IDI_ICON_WORD);
	AddToolboxGroup(2, "excel", IDI_ICON_EXCEL);
	AddToolboxGroup(3, "ppt", IDI_ICON_PPT);
	AddToolboxGroup(4, "pdf", IDI_ICON_PDF);
	AddToolboxGroup(5, "txt", IDI_ICON_TXT);
	AddToolboxGroup(6, "html", IDI_ICON_HTML);

#if 0
	std::vector<string> desp;
	std::string str1 = "desp1";
	std::string str2 = "desp2";
	desp.push_back(str1);
	desp.push_back(str2);
	AddLinkItem(1, 1,1,"3.txt", desp);
	AddLinkItem(1, 2,1,"2.doc", desp);
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastSearchDlg::OnEventNotify()
{
	//��ȡ�����Key��SelBox
	CString strKey;
 	GetDlgItemText(IDC_EDIT_SEARCH_KEY, strKey);

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

	//����ļ����ڣ����¼�µ�ǰkey
	if (m_agent.IsKeyFileExist())
	{
		sltFastSearchThread::getInstance()->PostMsg(fast);
		return ;
	}

	sltFastSearchThread::getInstance()->DoLog(fast);

}

void CFastSearchDlg::OnChangeEditSearchKey() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	OnEventNotify();

}

//ȴ��Ŀ¼�¼�
void CFastSearchDlg::OnSelchangeComboPath() 
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
//			SearchRectord sr;
//			memcpy(&sr, &m_agent.m_RecList[i],sizeof(SearchRectord));
			
			int nID = GetFileID(m_agent.m_RecList[i].szFileType);
			//////////////////////////////////////////////////////////////////////////
			//��ʾ������
			AddLinkItem(nID, i,1,m_agent.m_RecList[i].szFileName,m_agent.m_RecList[i].DespList);
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
	m_wndTaskPanel.AllowDrag(TRUE);

	int nMargin = 0;
	m_wndTaskPanel.GetPaintManager()->m_rcGroupInnerMargins.SetRect(nMargin, nMargin, nMargin, nMargin);
	m_wndTaskPanel.Reposition();
 
	return TRUE;
}

void CFastSearchDlg::AddToolboxGroup(UINT nID, LPCTSTR lpszCaption, int nIconIndex)
{
	CXTPTaskPanelGroup* pFolder = m_wndTaskPanel.AddGroup(nID, nIconIndex);	
	pFolder->SetCaption(lpszCaption);
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
void CFastSearchDlg::AddLinkItem(UINT nFolderID, UINT nItemID, int nIconIndex, LPCTSTR lpszCaption, std::vector<string> DespList)
{

	CXTPTaskPanelGroup* pFolder = m_listMap[nFolderID].pGroup;
	if (!pFolder)
		return ;

	m_listMap[nFolderID].nItemSize++;
	CXTPTaskPanelGroupItem* pPointer = pFolder->AddLinkItem(nItemID, 0/*imageid[nFolderID-1]*/);
	int nSize = DespList.size();
	if (nFolderID == 6)
	{
		//html����ʾ�������ݣ�ֻ��ʾ����
		int nNewSize = strlen(lpszCaption) + 5;
		char* pCaption = new char[nNewSize + 1];
		memset(pCaption, NULL, nNewSize + 1);
		sprintf(pCaption,"%s (%d)",lpszCaption, nSize);

		pPointer->SetCaption(pCaption);
	
		if (pCaption)
		{
			delete[] pCaption;
			pCaption = NULL;
		}
	}else
	{
		for (int i = 0; i < nSize; i++)
		{
			std::string strTextItem = "              " + DespList[i];
			pFolder->AddTextItem(strTextItem.c_str());
		}
		pPointer->SetCaption(lpszCaption);
	}

	pPointer->SetItemSelected(TRUE);
	pPointer->AllowDrag(FALSE);
	pPointer->AllowDrop(FALSE);
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
// 	COLORREF backColor = RGB(216, 231, 252); //office 2003����ɫ
// 	pDC->SetBkMode(TRANSPARENT);             //���ÿؼ�����͸��
// 	
// 	// �ж����ǲ�����Ҫ�ĵĿؼ�ID 
// 	if( pWnd->GetDlgCtrlID() == IDCANCEL || pWnd->GetDlgCtrlID() == IDOK )
// 	{
// 		pDC->SetBkColor(RGB(153, 255, 204));
// 	}
// 	
// 	return CreateSolidBrush(backColor);      //��������ˢ��	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

LRESULT CFastSearchDlg::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case XTP_TPN_CLICK:
		{
//			MessageBox("Click Event:");
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());

			//if (IsToggleButtons())
			{
				pItem->SetItemSelected(!pItem->IsItemSelected());
			}

			CString strFileName = pItem->GetCaption();
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
			std::string strFilePath = m_agent.GetFilePathFromName(strFileName.GetBuffer(0));
			if (strFilePath.size() != 0)
			{				
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

				if(m_setAgent.GetSoftPath(strGroupName.GetBuffer(0),szPath) == 0 )
				{
					ShellExecute(this->m_hWnd,"open",szPath,strFilePath.c_str(),"",SW_SHOW );
				}else
					ShellExecute(NULL, "open",strFilePath.c_str(),NULL, NULL, SW_SHOW);
			}
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
//		MessageBox("XTP_TPN_RCLICK Event:");
		//		OnTaskPanelRButtonDown((CXTPTaskPanelItem*)lParam);
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
