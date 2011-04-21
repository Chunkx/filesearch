// ShowAllCiHuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "ShowAllCiHuiDlg.h"
#include "sloCustomCiHuiAgent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowAllCiHuiDlg dialog


CShowAllCiHuiDlg::CShowAllCiHuiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowAllCiHuiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowAllCiHuiDlg)
	m_strKeyWord = _T("");
	//}}AFX_DATA_INIT
}


void CShowAllCiHuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowAllCiHuiDlg)
	DDX_Control(pDX, IDC_LIST_ALL_CIHUI, m_AllCiHuiListBox);
	DDX_Text(pDX, IDC_EDIT_KEY_WORD, m_strKeyWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowAllCiHuiDlg, CDialog)
	//{{AFX_MSG_MAP(CShowAllCiHuiDlg)
	ON_EN_CHANGE(IDC_EDIT_KEY_WORD, OnChangeEditKeyWord)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowAllCiHuiDlg message handlers

BOOL CShowAllCiHuiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strNewWords, strClose;
	
	strNewWords.LoadString(IDS_NEW_WORDS);
	strClose.LoadString(IDS_CLOSE);
	
	SetDlgItemText(IDC_STATIC_NEW_WORDS, strNewWords);
	SetDlgItemText(IDCANCEL, strClose);

	//////////////////////////////////////////////////////////////////

	ReadAllCiHui();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CShowAllCiHuiDlg::ReadAllCiHui()
{
    FILE *fp;   
    char *content;   
	
	sloCustomCiHuiAgent cus;
	cus.GetProFilePath();
    // ��ֻ����ʽ���ļ�   
    if((fp = fopen(cus.m_szcustomtxtPath, "r")) == NULL) {   
        printf("\r��ȡ�ļ�%sʱ�����쳣");   
        return -1;   
    }   
	
    // ���ļ�ָ�������ļ�ĩβ   
    fseek(fp, 0, SEEK_END);   
    // ȡ���ļ�ָ��ĵ�ǰλ�ã����ļ��ĳ���    
    int len = ftell(fp);   
    // ���ļ�ָ��ָ����ļ���ʼ��   
    rewind(fp);   
    // ��̬�����ڴ�   
    content = (char*)malloc(len + 1);
	memset(content, NULL, len+1);
    // ��ȡ�ļ����ݵ�content��   
    fread(content, 1, len, fp);    
    // ��content�����һλ��Ϊ0�����ַ���������־   
    content[len] = 0;   
    // ��ӡ�ļ�����   
    printf("%s\r\n", content);   
	
    // �ر��ļ�ָ��   
    fclose(fp);   

	//�����ʻ�
	char* pBegin = content, 
	char* pCur = pBegin;
	for (int i = 0; i < len; i++)
	{
		if (*pCur == '\n')
		{
			int nLen = pCur-pBegin;
			char* szItem = new char[nLen+1];
			memset(szItem, NULL, nLen+1);
			strncpy(szItem, pBegin, nLen);
			m_AllCiHuiList.push_back(szItem);
			m_AllCiHuiListBox.AddString(szItem);
			if (szItem)
			{
				delete szItem;
				szItem = NULL;
			}

			pBegin = pCur+1;
		}

		if (*pCur == 0x00)
			break ;

		pCur++;
	}


    // �ͷ�content�ڴ�   
    free(content); 
	
	return 0;
}

void CShowAllCiHuiDlg::OnChangeEditKeyWord() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//���listbox�б�
	ClearListBox();
 
	int nCount = m_AllCiHuiList.size();

	for (int i = 0; i < nCount; i++)
	{
		std::string strItem = m_AllCiHuiList[i];
		if (strstr(strItem.c_str(), m_strKeyWord.GetBuffer(0)))
		{
			m_AllCiHuiListBox.AddString(strItem.c_str());
		}
	}

	UpdateData(FALSE);
}


void CShowAllCiHuiDlg::ClearListBox()
{
	// Delete every other item from the list box.
	int nCount = m_AllCiHuiListBox.GetCount();
	for (int i=0;i < nCount; i++)
	{
		m_AllCiHuiListBox.DeleteString( 0 );
	}
}
