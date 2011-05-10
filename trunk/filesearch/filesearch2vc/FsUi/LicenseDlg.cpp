// LicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "LicenseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLicenseDlg dialog


CLicenseDlg::CLicenseDlg(CWnd* pParent /*=NULL*/)
	: CXTPPropertyPage(CLicenseDlg::IDD)/*CDialog(CLicenseDlg::IDD, pParent)*/
{
	//{{AFX_DATA_INIT(CLicenseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLicenseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLicenseDlg, CDialog)
	//{{AFX_MSG_MAP(CLicenseDlg)
	ON_BN_CLICKED(IDC_BUTTON_REG, OnButtonReg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLicenseDlg message handlers

void CLicenseDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CLicenseDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CLicenseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strLicInfo, strLic1, strNum,strEmail;
	CString strStartTime,strEndTime, strReg;
	LANGUAGE lag = g_lag.m_lag;
	switch(lag)
	{
	case lag_chinese:		
		strLicInfo.LoadString(IDS_LICENSE_INFO);
		strLic1.LoadString(IDS_LICENSE_1);
		strNum.LoadString(IDS_NUM);
		strEmail.LoadString(IDS_STATIC_EMAIL);
		strStartTime.LoadString(IDS_BEGINTIME);
		strEndTime.LoadString(IDS_ENDTIME);
		strReg.LoadString(IDS_REG);
		break;
	case lag_Japanese:
		strLicInfo.LoadString(IDS_LICENSE_INFO_JP);
		strLic1.LoadString(IDS_LICENSE_1_JP);
		strNum.LoadString(IDS_NUM_JP);
		strEmail.LoadString(IDS_STATIC_EMAIL_JP);
		strStartTime.LoadString(IDS_BEGINTIME_JP);
		strEndTime.LoadString(IDS_ENDTIME_JP);
		strReg.LoadString(IDS_REG_JP);
		break;
	case lag_engish:
		strLicInfo.LoadString(IDS_LICENSE_INFO_EN);
		strLic1.LoadString(IDS_LICENSE_1_EN);
		strNum.LoadString(IDS_NUM_EN);
		strEmail.LoadString(IDS_STATIC_EMAIL_EN);
		strStartTime.LoadString(IDS_BEGINTIME_EN);
		strEndTime.LoadString(IDS_ENDTIME_EN);
		strReg.LoadString(IDS_REG_EN);
		break;
	}

	
	SetDlgItemText(IDC_STATIC_LICENSE_INFO, strLicInfo);
	SetDlgItemText(IDC_STATIC_LICENSE_1, strLic1);
	SetDlgItemText(IDC_STATIC_NUM, strNum);
	SetDlgItemText(IDC_STATIC_EMAIL, strEmail);
	SetDlgItemText(IDC_STATIC_BEGINTIME, strStartTime);
	SetDlgItemText(IDC_STATIC_ENDTIME, strEndTime);

	SetDlgItemText(IDC_BUTTON_REG, strReg);

	//////////////////////////////////////////////////////////////////////////

	m_licAgent.Init();
	UpdateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
#define ERROR_INFO_SUCCEED				"ע����Ϣ�Ѿ��ύ��һ��֮��ȷ�Ϻ��������ע�ᣡ\r\n"
#define ERROR_INFO_ORDERNO				"����д�����ţ�\r\n"
#define ERROR_INFO_EMAIL				"����д��ȷ�ĵ������䣡\r\n������Ϣ��"
#define ERROR_INFO_REGISTER_FAILED		"Licenseע��ʧ�ܣ�\r\n"

BOOL CLicenseDlg::UpdateDialog() 
{

	if (m_licAgent.m_bDown)
	{
		//��ע�ᣬ����ʾע����Ϣ��������
		GetDlgItem(IDC_BUTTON_REG)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO)->ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_EDIT_NUM,m_licAgent.m_LicInfo.szOrderNo);
		SetDlgItemText(IDC_EDIT_EMAIL,m_licAgent.m_LicInfo.szEmail);
		((CEdit*)GetDlgItem(IDC_EDIT_NUM))->SetReadOnly(TRUE); 
		((CEdit*)GetDlgItem(IDC_EDIT_EMAIL))->SetReadOnly(TRUE); 

		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME_CONTENT)->ShowWindow(SW_SHOW);
		(CStatic*)GetDlgItem(IDC_STATIC_ENTTIME_CONTENT)->ShowWindow(SW_SHOW);
		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME)->ShowWindow(SW_SHOW);
		(CStatic*)GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_SHOW);


		((CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME_CONTENT))->SetWindowText(m_licAgent.m_LicInfo.szStartDate);
		((CStatic*)GetDlgItem(IDC_STATIC_ENTTIME_CONTENT))->SetWindowText(m_licAgent.m_LicInfo.szEndDate);
		
	}else if (m_licAgent.m_bReg)
	{
		//��Ϣ���ύ
		SetDlgItemText(IDC_EDIT_NUM,m_licAgent.m_LicInfo.szOrderNo);
		SetDlgItemText(IDC_EDIT_EMAIL,m_licAgent.m_LicInfo.szEmail);

		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME_CONTENT)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ENTTIME_CONTENT)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO)->ShowWindow(SW_HIDE);
	
		CString strErrorInfo = "��ʾ��";
		strErrorInfo += ERROR_INFO_SUCCEED;

		(CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO)->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO))->SetWindowText(strErrorInfo.GetBuffer(0));
	}else
	{
		//δע��
		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME_CONTENT)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ENTTIME_CONTENT)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_BEGINTIME)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
		(CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO)->ShowWindow(SW_HIDE);	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CLicenseDlg::OnButtonReg() 
{
	// TODO: Add your control notification handler code here
	CString strNum, strEMail;
	CString strErrorInfo = "��ʾ��";
	
	BOOL bRet = TRUE;
	do 
	{
		GetDlgItemText(IDC_EDIT_NUM,strNum);
		GetDlgItemText(IDC_EDIT_EMAIL,strEMail);
		if (strNum.GetLength() == 0)
		{
			strErrorInfo += ERROR_INFO_ORDERNO;
			bRet = FALSE ;
			break ;
		}

		//���email�Ƿ�Ϸ�
		char szErrInfo[1024] = {0};
		if( m_licAgent.IsEmailAddr(strEMail.GetBuffer(0),szErrInfo) == false)
		{
			strErrorInfo += ERROR_INFO_EMAIL;
			strErrorInfo += szErrInfo;
			bRet = FALSE ;
			break ;		
		}
		
		if (m_licAgent.EventLicense(strNum.GetBuffer(0), strEMail.GetBuffer(0)) == FALSE)
		{
			strErrorInfo += ERROR_INFO_REGISTER_FAILED;
			bRet = FALSE ;
			break ;	
		}

		strErrorInfo += ERROR_INFO_SUCCEED;
	} while (0);
	
	(CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO)->ShowWindow(SW_SHOW);
	((CStatic*)GetDlgItem(IDC_STATIC_ERROR_INFO))->SetWindowText(strErrorInfo.GetBuffer(0));
	
}

HBRUSH CLicenseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_ERROR_INFO) 
		pDC->SetTextColor(RGB(255,0,0)); //����������ɫΪ��ɫ
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
