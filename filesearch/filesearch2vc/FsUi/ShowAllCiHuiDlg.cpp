// ShowAllCiHuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "ShowAllCiHuiDlg.h"

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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowAllCiHuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowAllCiHuiDlg)
	DDX_Control(pDX, IDC_LIST_ALL_CIHUI, m_AllCiHuiListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowAllCiHuiDlg, CDialog)
	//{{AFX_MSG_MAP(CShowAllCiHuiDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowAllCiHuiDlg message handlers

BOOL CShowAllCiHuiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_AllCiHuiListBox.AddString("11");
	m_AllCiHuiListBox.AddString("22");

	ReadAllCiHui();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CShowAllCiHuiDlg::ReadAllCiHui()
{
    FILE *fp;   
    char *content;   
	
    // ��ֻ����ʽ���ļ�   
    if((fp = fopen("C:\\custom.txt", "r")) == NULL) {   
        printf("\r��ȡ�ļ�D:\\test.txtʱ�����쳣");   
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
		if (*pCur == '\n' || *pCur == 0x00)
		{
			int nLen = pCur-pBegin;
			char* szItem = new char[nLen+1];
			memset(szItem, NULL, nLen+1);
			strncpy(szItem, pBegin, nLen);
			m_AllCiHuiListBox.AddString(szItem);
			if (szItem)
			{
				delete szItem;
				szItem = NULL;
			}

			pBegin = pCur+1;
		}
		pCur++;
	}


    // �ͷ�content�ڴ�   
    free(content); 
	
	return 0;
}
