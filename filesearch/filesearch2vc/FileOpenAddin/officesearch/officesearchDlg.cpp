// officesearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "officesearch.h"
#include "officesearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COfficesearchDlg dialog

COfficesearchDlg::COfficesearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COfficesearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COfficesearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COfficesearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COfficesearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COfficesearchDlg, CDialog)
	//{{AFX_MSG_MAP(COfficesearchDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COfficesearchDlg message handlers

BOOL COfficesearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COfficesearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COfficesearchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COfficesearchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/*

  Sub Macro1()
  '
  ' Macro1 Macro
  ' ���� 2011-8-22 �� δ���� ¼��
  '
  Selection.GoTo What:=wdGoToPage, Which:=wdGoToNext, Name:="3"
  Selection.Find.ClearFormatting
  With Selection.Find
  .Text = "123"
  .Replacement.Text = ""
  .Forward = True
  .Wrap = wdFindContinue
  .Format = False
  .MatchCase = False
  .MatchWholeWord = False
  .MatchByte = True
  .MatchWildcards = False
  .MatchSoundsLike = False
  .MatchAllWordForms = False
  End With
  Selection.Find.Execute
  End Sub

  Selection.GoTo What:=wdGoToPage, Which:=wdGoToNext, Name:="2"
  Selection.Find.ClearFormatting
  With Selection.Find
  .Text = "123"
  .Replacement.Text = ""
  .Forward = True
  .Wrap = wdFindContinue
  .Format = False
  .MatchCase = False
  .MatchWholeWord = False
  .MatchByte = True
  .MatchWildcards = False
  .MatchSoundsLike = False
  .MatchAllWordForms = False
  End With
    Selection.Find.Execute

*/
//��wordָ��ҳ
void COfficesearchDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
//	fileaddin.OpenFile_WORD("D:\\2.doc", 2, "134");

//	return ;

	CoInitialize(NULL);
	
	HRESULT hr = S_OK;
	Word::_ApplicationPtr spWordApp = NULL;
	{
		hr = spWordApp.CreateInstance(L"Word.Application");
	}
	if (FAILED(hr))
	{
		MessageBox("Open CreateInstance failed!");
		CoUninitialize();
		return ;
	}
	spWordApp->put_Visible(VARIANT_TRUE);

	CComPtr<Word::_Document> document;
	CComPtr<Documents> spDocs = NULL;
	spWordApp->get_Documents(&spDocs);
	if (spDocs == NULL)
	{
		MessageBox("Open get_Documents failed!");
		OutputDebugStringA("[EnumWord] ��ȡ�ĵ����󼯺�ʧ��\n");
		return ;
	}
	
	document = spDocs->Open(&CComVariant("E:\\2.doc"), &vtMissing, &vtMissing, &CComVariant(VARIANT_TRUE));
	if (document == NULL)
	{
		OutputDebugStringA("[EnumWord] ���ĵ�ʧ��\n");
		MessageBox("Open D:\\2.doc failed!");
		return ;
	}
	//���ִ�еڼ�ҳ

	int nPage  =0;
	CComPtr<Word::Selection> sel;
	spWordApp->get_Selection(&sel);

	CComVariant vName(4);
	CComVariant count(1);
	CComVariant What(wdGoToPage);
	CComVariant Which(wdGoToNext);
	CComPtr<Word::Range> range;
	range = sel->GoTo(&What, &Which, &count, &vName);
/*
	VARIANT pagecount;
	sel->get_Information(wdNumberOfPagesInDocument, &pagecount);
	int nPagecount = pagecount.lVal;

 	CComPtr<Word::Find> find;
	sel->get_Find(&find);
	find->ClearFormatting();


	CComVariant vFindText("123"), vForward(VARIANT_TRUE);
	VARIANT_BOOL prop;
	hr = find->Execute(&vFindText, 
		&vtMissing, 
		&vtMissing, 
		&vtMissing, 
		&vtMissing, 
		&vtMissing, 
		&vForward, 
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing, 
		&vtMissing, 
		&vtMissing,
		&vtMissing);
*/

//	CoUninitialize();

}

/*

  Sub Macro2()
  '
  ' Macro2 Macro
  ' ���� δ���� ¼�ƣ�ʱ��: 2011-8-22
  '
	  	'
		Sheets("Sheet2").Select
		Application.Goto Reference:="Sheet2!R20C1"
		End Sub

  Sub Macro2()
  '
  ' Macro2 Macro
  ' ���� δ���� ¼�ƣ�ʱ��: 2011-8-22
  '
  
	'
    Sheets("Sheet2").Select
    Range("A18").Select
    Cells.Find(What:="4", After:=ActiveCell, LookIn:=xlFormulas, LookAt:= _
	xlPart, SearchOrder:=xlByRows, SearchDirection:=xlNext, MatchCase:=False _
	, MatchByte:=False, SearchFormat:=False).Activate
	End Sub
*/
void COfficesearchDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
//	fileaddin.OpenFile_EXCEL("h:\\3.xls", "Sheet3", 20, "123");

	CoInitialize(NULL);

	Excel::_ApplicationPtr spExcelApp;
	HRESULT hr = spExcelApp.CreateInstance(L"Excel.Application");
	if (FAILED(hr))
	{
		CoUninitialize();
		return;
	}
	
	spExcelApp->put_Visible(0, VARIANT_TRUE);

	CComPtr<Excel::Workbooks> spWorkbooks = NULL;
	spExcelApp->get_Workbooks(&spWorkbooks);
	if (spWorkbooks == NULL)
	{
		OutputDebugStringA("[EnumExcel] ��ȡ���������󼯺�ʧ��\n");
		return ;
	}
	
	CComPtr<Excel::_Workbook> m_spWb;
	m_spWb = spWorkbooks->Open("E:\\3.xls");
	if (m_spWb == NULL)
	{
		OutputDebugStringA("[EnumExcel] �򿪹�����ʧ��\n");
		return ;
	}
	
	//��λ
	CComVariant vsheet("Sheet3!R20C1");
	spExcelApp->Goto(&vsheet, &vtMissing, 0);
	
	//����
	CComVariant What("123"), LookIn(xlFormulas), LookAt(xlPart), SearchOrder(xlByRows);
	spExcelApp->GetCells()->Find(&What, &vtMissing, &LookIn,
		&LookAt, &SearchOrder, xlNext, 
		&vtMissing, &vtMissing,	&vtMissing)->Activate();

}

/*
 ActiveWindow.Selection.SlideRange.Shapes("Rectangle 3").Select

ShapeRange.TextFrame.TextRange
*/
void COfficesearchDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here

//	fileaddin.OpenFile_PPT("H:\\1.ppt", 2, "5");
	CoInitialize(NULL);
	PPT::_ApplicationPtr spPPTApp;
	HRESULT hr = spPPTApp.CreateInstance(L"PowerPoint.Application");
	if (FAILED(hr))
	{
		CoUninitialize();
		return ;
	}
	
	spPPTApp->put_Visible(msoTrue);

	spPPTApp->Activate();
	
	CComPtr<Presentations> spPresents = NULL;
	spPPTApp->get_Presentations(&spPresents);
	if (spPresents == NULL)
	{
		OutputDebugStringA("[EnumPPT] ��ȡ���ڶ��󼯺�ʧ��\n");
		return ;
	}
		
	CComPtr<PPT::_Presentation> m_spPres;
	m_spPres = spPresents->Open("E:\\1.ppt", msoFalse, msoFalse, msoTrue);
	if (m_spPres == NULL)
	{
		OutputDebugStringA("[EnumPPT] ��ppt�ĵ�ʧ��\n");
		return ;
	}

	CComVariant item(8);
	m_spPres->GetSlides()->Item(&item)->Select();

	//��������
	CComPtr<PPT::DocumentWindow> docwin;
	spPPTApp->get_ActiveWindow(&docwin);
	
	CComPtr<PPT::Shapes> shapes;
	docwin->GetSelection()->GetSlideRange()->get_Shapes(&shapes);

	CComBSTR strText("java");
	int nCount = shapes->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CComVariant index = i+1;
		CComPtr<PPT::Shape> shape = shapes->Item(&index);
		CComPtr<PPT::TextRange> textrange;
		textrange = shape->GetTextFrame()->GetTextRange()->Find(strText.m_str, 1, msoFalse, msoFalse);
		int nCount = textrange->GetLength();
		int nstart = textrange->GetStart();

		if (nCount != 0 && nstart != 1)
		{
			textrange->Select();
			break;
		}
	}
	
}

void COfficesearchDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
//	fileaddin.OpenFile_TXT("H:\\1.txt", 7, "3524");

	char szFileName[MAX_PATH] = "E:\\1.php";
	DWORD dwRet =0 ;
	do 
	{
		try
		{
			//ȥ��·��		
			char szWindowsName[MAX_PATH] = {0};
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(szFileName, NULL, NULL, fname, ext);
			
			
			sprintf(szWindowsName, "%s%s - ���±�", fname,ext);
			
			//�����ı��Ƿ��
			CWnd* txtwnd = FindWindow("NotePad", szWindowsName);
			if (txtwnd == NULL)
			{
				//��ȡ���±�·��
				char szNotePadPath[MAX_PATH] = {0};
				char szWinDir[MAX_PATH] = {0};
				GetWindowsDirectory(szWinDir, MAX_PATH);
				sprintf(szNotePadPath, "%s\\notepad.exe", szWinDir);
				//�򿪸��ĵ�
				ShellExecute(NULL, "open", szNotePadPath, szFileName, NULL, SW_SHOW);
				
				Sleep(200);
				
				//�ٴμ����ı��Ƿ��
				
				txtwnd = FindWindow("NotePad", szWindowsName);
				if (txtwnd == NULL)
				{
					MessageBox("FindWindow NotePad failed!");
					dwRet = -3;
					break;
				}
			}
			
			//����edit�ؼ����
			HWND hwnd = FindWindowEx(txtwnd->m_hWnd, NULL, "Edit", "");
			
			CEdit edit;
			edit.Attach(hwnd);
			
			edit.SetFocus();
			
			int nLine = 3 - 1;
			int nIndex = -1;
			//lineת��
			nIndex = edit.LineIndex(nLine);
			if (nIndex == -1)
			{				
				MessageBox("FindWindow LineIndex failed!");
				//���������˷�Χ
				dwRet = -2;
				break;
			}
			edit.SetSel(nIndex,nIndex);
			
			edit.Detach();
		}
		catch (...)
		{
				MessageBox("exception!");
			dwRet = -1;
			break;
		}
	} while (0);
	
}

void COfficesearchDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here

	HRESULT                             hr; 
	CString FileName,LoadFile,SaveFile,SaveFileName,Temp1; 
	CHAR             FilePath[255]; 
	VARIANT           Temp; 
	COleVariant     TEMP2; 
	CTime NowTime; 
	hr=CoInitialize(NULL); 
	VARIANT_BOOL   m_ok,m_ok1; 
	long   i,j,m,n; 
	
	Word::_ApplicationPtr   MyOffice( "Word.Application.9 "); 
	::GetCurrentDirectory(255,FilePath);   
	FileName.Format( "D:\\",FilePath);   
	LoadFile=FileName+ "\\2.doc "; 
	TEMP2=(COleVariant)LoadFile; 
	Temp=TEMP2.Detach();   
	if   (MyOffice)   { 
		hr=MyOffice-> Documents-> Open(&Temp);   
		if(FAILED(hr)) 
		{ 
			::AfxMessageBox( "�ļ�����ʾ���� "); 
			MyOffice-> Quit(); 
		} 
		Temp1= "123"; 
		TEMP2.Clear();   
		TEMP2=(COleVariant)Temp1; 
		Temp=TEMP2.Detach();   
		i=MyOffice-> GetSelection()-> GetStart(); 
		j=MyOffice-> GetSelection()-> GetEnd(); 
		MyOffice-> GetSelection()-> PutStart(0); 
		MyOffice-> GetSelection()-> PutEnd(100000); 
		i=MyOffice-> GetSelection()-> GetStart(); 
		j=MyOffice-> GetSelection()-> GetEnd(); 
		m_ok=MyOffice-> ActiveWindow-> GetSelection()-> GetFind()-> Execute(&Temp); 
		if(m_ok==0) 
		{ 
			::AfxMessageBox( "�����ļ�ʧ�� "); 
		} 
		else 
		{ 
			::AfxMessageBox( "�����ļ��ɹ� "); 
		} 
		
		MyOffice-> Quit(); 
	} 

/*	_Application   myApp;   
	Documents   myDocs;   
	_Document   myDoc;   
	Range   myRange;   
	Find   fndInDoc;   
	Replacement   rpInDoc;   
	
	if(!myApp.CreateDispatch( "Word.Application "))   
	{   
		AfxMessageBox(_T( "failed "));   
	}   
	myApp.SetVisible(TRUE); 
	
	COleVariant   vTrue((short)TRUE),   
		vFalse((short)FALSE), 
		vOpt((long)DISP_E_PARAMNOTFOUND,   VT_ERROR); 
	COleVariant   FileName= "D:\\2.doc "; 
	myDocs=myApp.GetDocuments(); 
	myDoc=myDocs.Add(FileName,vOpt,vOpt,vOpt); 
	myRange=myDoc.GetContent();   
	fndInDoc=myRange.GetFind();   
	fndInDoc.ClearFormatting();   
	
	rpInDoc=fndInDoc.GetReplacement();   
	rpInDoc.ClearFormatting();   
	
	COleVariant       varstrNull( " ");       
	COleVariant       varZero((short)0);       
	COleVariant       varTrue(short(1),VT_BOOL);       
	COleVariant       varFalse(short(0),VT_BOOL);     
	
	COleVariant   Text(_T( "123 "));   
	CComVariant   MatchCase(varFalse);   
	CComVariant   MatchWholeWord(varFalse);   
	CComVariant   MatchWildcards(varFalse);   
	CComVariant   MatchSoundsLike(varFalse);   
	CComVariant   MatchAllWordForms(varFalse);   
	CComVariant   Forward(varTrue);   
	CComVariant   Wrap((short)1);///!!!!!!!!!!!!!!!!!   
	CComVariant   format(varFalse);   
	CComVariant   ReplaceWith(_T( "�л����񹲺͹����� "));   
	CComVariant   Replace((short)2);////////!!!!!!!!��Ҫ������ 
	CComVariant   MatchKashida(varstrNull);   
	CComVariant   MatchDiacritics(varstrNull);   
	CComVariant   MatchAlefHamza(varstrNull);   
	CComVariant   MatchControl(varstrNull); 
	AfxMessageBox(_T( "���1 "));     //������ 
	
	fndInDoc.Execute(&Text,   &MatchCase,   &MatchWholeWord,   &MatchWildcards,   &MatchSoundsLike,   
		&MatchAllWordForms,   &Forward,   &Wrap,   &format,   &ReplaceWith,&Replace,&MatchKashida,&MatchDiacritics, 
		&MatchAlefHamza,   &MatchControl); 
	AfxMessageBox(_T( "���2 "));     //������ 
	
	
	myDoc.Save();   
	AfxMessageBox(_T( "ok! "));   //test 
	
	myRange.ReleaseDispatch(); 
	fndInDoc.ReleaseDispatch(); 
	rpInDoc.ReleaseDispatch(); 
	myDocs.ReleaseDispatch(); 
	myDoc.ReleaseDispatch(); 
	
	CComVariant   SaveChanges(false),OriginalFormat,RouteDocument; 
	
	myApp.Quit(&SaveChanges,&OriginalFormat,&RouteDocument); 
myApp.ReleaseDispatch();
*/

/*
	CComPtr<Word::_Document> document;
	CComPtr<Documents> spDocs = NULL;
	CApplication     myApp;     
	CDocuments   myDocs;   
	CDocument0   myDoc;   
	CRange   myRange;   
	CFind   fndInDoc;   
	CReplacement   rpInDoc;   
	
	if(!myApp.CreateDispatch( "Word.Application "))   
	{   
		AfxMessageBox(_T( "failed "));   
		return; 
	}   
	myApp.put_Visible(FALSE); 
	
	COleVariant   vTrue((short)TRUE),   
		vFalse((short)FALSE), 
		vOpt((long)DISP_E_PARAMNOTFOUND,   VT_ERROR); 
	COleVariant   FileName= "c:\\Hello.doc "; 
	myDocs=myApp.get_Documents(); 
	myDoc=myDocs.Add(FileName,vOpt,vOpt,vOpt); 
	myRange=myDoc.get_Content();   
	fndInDoc=myRange.get_Find();   
	fndInDoc.ClearFormatting();   
	
	rpInDoc=fndInDoc.get_Replacement();   
	rpInDoc.ClearFormatting();   
	
	COleVariant       varstrNull( " ");       
	COleVariant       varZero((short)0);       
	COleVariant       varTrue(short(1),VT_BOOL);       
	COleVariant       varFalse(short(0),VT_BOOL);     
	COleVariant   vInt((long)-1,     VT_I4),vIntF((long)0,     VT_I4); 
	COleVariant   Text(_T( "two "));   
	CComVariant   MatchCase(varFalse);   
	CComVariant   MatchWholeWord(varFalse);   
	CComVariant   MatchWildcards(varFalse);   
	CComVariant   MatchSoundsLike(varFalse);   
	CComVariant   MatchAllWordForms(varFalse);   
	CComVariant   Forward(varTrue);   
	CComVariant   Wrap((short)1);///!!!!!!!!!!!!!!!!!   
	CComVariant   format(varFalse);   
	CComVariant   ReplaceWith(_T( "�л����񹲺͹����� "));   
	CComVariant   Replace((short)2);////////!!!!!!!!��Ҫ������ 
	CComVariant   MatchKashida(varstrNull);   
	CComVariant   MatchDiacritics(varstrNull);   
	CComVariant   MatchAlefHamza(varstrNull);   
	CComVariant   MatchControl(varstrNull); 
	AfxMessageBox(_T( "���1 "));     //������ 
	
	fndInDoc.Execute(&Text,   &MatchCase,   &MatchWholeWord,   &MatchWildcards,   &MatchSoundsLike,   
		&MatchAllWordForms,   &Forward,   &Wrap,   &format,   &ReplaceWith,&Replace,&MatchCase,   &MatchWholeWord,   &MatchWildcards,   &MatchSoundsLike);//&MatchKashida,&MatchDiacritics, 
	//&MatchAlefHamza,   &MatchControl); 
	AfxMessageBox(_T( "���2 "));     //������ 
	
	
	//myDoc.Save();   
	
	myDoc.Close(vInt,vIntF,vFalse); 
	AfxMessageBox(_T( "ok! "));   //test 
	
	myRange.ReleaseDispatch(); 
	fndInDoc.ReleaseDispatch(); 
	rpInDoc.ReleaseDispatch(); 
	myDocs.ReleaseDispatch(); 
	myDoc.ReleaseDispatch(); 
	
	CComVariant   SaveChanges(false),OriginalFormat,RouteDocument; 
	
	myApp.Quit(&vOpt,&vOpt,&vOpt); 
myApp.ReleaseDispatch();
*/	
}

void COfficesearchDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, "open", "D:\\Program Files\\Adobe\\Reader 8.0\\Reader\\AcroRd32.exe", "/a page=3 d:\\1.pdf", NULL, SW_SHOW);
	
}
