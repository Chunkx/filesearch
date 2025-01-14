#if !defined(AFX_CUSTOMCIHUIDLG_H__EFA4B7EB_83FC_4E95_B7CB_643E3650E6E4__INCLUDED_)
#define AFX_CUSTOMCIHUIDLG_H__EFA4B7EB_83FC_4E95_B7CB_643E3650E6E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomCiHuiDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomCiHuiDlg dialog

class CCustomCiHuiDlg :public  CXTPPropertyPage/*CDialog*/
{
// Construction
public:
	CCustomCiHuiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCustomCiHuiDlg)
	enum { IDD = IDD_DIALOG_CUSTOM_DICT };
	CString	m_strBrowsePath;
	BOOL	m_bIsOverCiHui;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomCiHuiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
// Implementation
protected:
	CXTPButton m_btnFolder;
	CXTPButton m_btnShowAllCihui;
	CFont f;
	// Generated message map functions
	//{{AFX_MSG(CCustomCiHuiDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonCheckExistcihui();
	afx_msg void OnButtonBrowseCihui();
	afx_msg void OnCheckOvercihui();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMCIHUIDLG_H__EFA4B7EB_83FC_4E95_B7CB_643E3650E6E4__INCLUDED_)
