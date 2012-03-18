// AddinSavefileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "officeaddin.h"
#include "AddinSavefileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddinSavefileDlg dialog


CAddinSavefileDlg::CAddinSavefileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddinSavefileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddinSavefileDlg)
	m_strDocpath = _T("");
	//}}AFX_DATA_INIT
}


void CAddinSavefileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddinSavefileDlg)
	DDX_Control(pDX, IDC_TREE_GROUP, m_treeGroup);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strDocpath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddinSavefileDlg, CDialog)
	//{{AFX_MSG_MAP(CAddinSavefileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddinSavefileDlg message handlers

BOOL CAddinSavefileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 1, 1);
	
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	ASSERT(hIcon);
	m_ilTreeIcons.Add (hIcon);
	
	//VERIFY(m_treeGroup.Create(WS_VISIBLE|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_EDITLABELS |TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, 0));
	m_treeGroup.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);
	
	//ö��applicationDataĿ¼�µ�slfile(C:\Documents and Settings\All Users\Application Data\slfile)����ȡ��������Ϣ
	BrowseFile(0, "D:\\test");
// 	HTREEITEM hItem;
// 	//��ȡ�������
// 	int nGroupSize = 3;
// 	for (int i = 0; i < nGroupSize; i++)
// 	{
// 		hItem = m_treeGroup.InsertItem ("folder", 0, 0);
// 		//sloMysqlAgent::GetInstance()->GetTypesFromDB(sloMysqlAgent::GetInstance()->m_GroupList[i].nID);
// 		m_treeGroup.SetItemState (hItem, TVIS_BOLD, TVIS_BOLD);
// 		//��ȡ�÷����µ����͸���
// 		int nTypeSize = 2;
// 		for (int j = 0; j < nTypeSize; j++)
// 		{
// 			m_treeGroup.InsertItem ("subfolder", 1, 1, hItem);
// 		}
// 		m_treeGroup.Expand(hItem, TVE_EXPAND);
// 	}
// 	
// 	hItem = m_treeGroup.GetRootItem();
// 	m_treeGroup.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
// 	//m_treeGroup.EnableMultiSelect(FALSE);
	
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddinSavefileDlg::SetDcoumentPath(char* szFilePath)
{
	m_strDocpath = szFilePath;
}


void CAddinSavefileDlg::BrowseFile(int CallNum, CString strFile)
{
	CallNum++;
	CFileFind ff;
	CString szDir = strFile;
	
	if(szDir.Right(1) != "\\")
		szDir += "\\";
	
	szDir += "*.*";
	
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			//�����һ����Ŀ¼���õݹ��������һ����
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileTitle();
			int i =0;
			switch(CallNum)
			{
			case 1:
				strHTFir = m_treeGroup.InsertItem(strTitle,0,0,NULL);						
				break;
			case 2:
				strHTSec = m_treeGroup.InsertItem(strTitle,0,0,strHTFir);											
				break;
			case 3:
				strHtThi = m_treeGroup.InsertItem(strTitle,0,0,strHTSec);					
				break;
			case 4:
				strHtFor = m_treeGroup.InsertItem(strTitle,0,0,strHtThi);					
				break;
			default:
				strHtFif = m_treeGroup.InsertItem(strTitle,0,0,strHtFor);
				break;					
			}
			BrowseFile(CallNum,strPath);
		}
		else if(!ff.IsDirectory() && !ff.IsDots())
		{
			//��ʾ��ǰ���ʵ��ļ�
// 			CString strPath;
// 			CString strTitle;
// 			strPath = ff.GetFilePath();
// 			strTitle = ff.GetFileTitle();
// 			switch(CallNum)
// 			{
// 			case 1:
// 				strRoot = m_treeGroup.InsertItem(strTitle,0,0,NULL);
// 				break;
// 			case 2:
// 				strHtEnd = m_treeGroup.InsertItem(strTitle,0,0,strHTFir);
// 				break;
// 			case 3:
// 				strHtEnd = m_treeGroup.InsertItem(strTitle,0,0,strHTSec);				
// 				break;
// 			case 4:
// 				strHtEnd = m_treeGroup.InsertItem(strTitle,0,0,strHtThi);
// 				break;
// 			case 5:
// 				strHtEnd = m_treeGroup.InsertItem(strTitle,0,0,strHtFor);
// 				break;
// 			default:
// 				strHtEnd = m_treeGroup.InsertItem(strTitle,0,0,strHtFif);
// 				break;
// 			}
		}
	}
	ff.Close();//�ر�
}

void CAddinSavefileDlg::OnOK() 
{
	// TODO: Add extra validation here
	//�����ж��Ƿ�ѡ����
	HTREEITEM hItem = m_treeGroup.GetSelectedItem();
	if (hItem == NULL)
	{
		//��ʾ��Ҫѡ��Ŀ��Ŀ¼
		MessageBox("��ѡ��Ŀ��Ŀ¼!");
		return ;
	}
	CString strDir = m_treeGroup.GetItemText(hItem);

	//�����ļ���ַд��dat�ļ�������T_DOCUMENT����һ����¼
#if 0
	DocumentItem docitem;
	memset(&docitem, NULL, sizeof(DocumentItem));
	sloMysqlAgent::GetInstance()->AddDoucmentRecord(docitem);

#endif

	CDialog::OnOK();
}
