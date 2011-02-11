// FsUiTestDlg.h : header file
//

#if !defined(AFX_FSUITESTDLG_H__8FFA2E2F_97CC_40A6_A2EC_E83FA5472379__INCLUDED_)
#define AFX_FSUITESTDLG_H__8FFA2E2F_97CC_40A6_A2EC_E83FA5472379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFsUiTestDlg dialog

class CFsUiTestDlg : public CDialog
{
// Construction
public:
	CFsUiTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFsUiTestDlg)
	enum { IDD = IDD_FSUITEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFsUiTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFsUiTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSUITESTDLG_H__8FFA2E2F_97CC_40A6_A2EC_E83FA5472379__INCLUDED_)