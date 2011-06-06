// SkinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FsUi.h"
#include "SkinDlg.h"
#include "sloSkinAgent.h"
#include "imageprocessors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg dialog


CSkinDlg::CSkinDlg(CWnd* pParent /*=NULL*/)
	: CXTPPropertyPage(CSkinDlg::IDD)/*: CDialog(CSkinDlg::IDD, pParent)*/,
	m_bitmap(GetSysColor(COLOR_3DFACE)), 
	m_bitmapOrg(GetSysColor(COLOR_3DFACE)),
	m_sizePrev(0, 0)
{
	//{{AFX_DATA_INIT(CSkinDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_bGrayscale = FALSE;
	m_bRotate = FALSE;
	m_bShearDown = FALSE;
	m_bShearAcross = FALSE;
	m_bBlur = FALSE;
	m_bShrink = TRUE;
	m_bEnlarge = FALSE;
	m_bFlipHorz = FALSE;
	m_bFlipVert = FALSE;
	m_bNegate = FALSE;
	m_bReplaceBlack = FALSE;
	m_bWeighting = TRUE;
	m_sTimeTaken = _T("");
	m_bUseProcessorArray = TRUE;
	m_bSharpen = FALSE;
	m_nBlurFactor = 4;
	m_nSharpenFactor = 4;
	m_nShrinkFactor = 3;
	m_nEnlargeFactor = 4;
	m_nRotationAngle = 5;
	m_nYShearAmount = 10;
	m_nXShearAmount = 10;
	//}}AFX_DATA_INIT
	memset(m_szSkinTheme, NULL, MAX_PATH);

}


void CSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinDlg)
	DDX_Control(pDX, IDC_LIST_SKIN, m_skinbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinDlg)
	ON_LBN_SELCHANGE(IDC_LIST_SKIN, OnSelchangeListSkin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg message handlers
#define THREME_NAME_NORMAL			"<Ĭ��Ƥ��>"
#define THREME_NAME_2007			"Office2007"
#define THREME_NAME_VISTA			"Vista"
#define THREME_NAME_XP_ROYALE		"WinXP.Royale"
#define THREME_NAME_XP_LUNA			"WinXP.Luna"
BOOL CSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_STATIC_SKIN_SET, g_lag.LoadString("label.skinset"));
	SetDlgItemText(IDC_STATIC_SKIN_SELECT, g_lag.LoadString("label.selectskin"));
	SetDlgItemText(IDC_STATIC_SKIN_PREVIEW, g_lag.LoadString("label.skinpreview"));
	SetDlgItemText(IDC_STATIC_AUTHOR, g_lag.LoadString("label.skinauthor"));

	SetDlgItemText(IDC_STATIC_AUTHOR_NAME, g_lag.LoadString("label.skinauthorname"));
	SetDlgItemText(IDC_STATIC_HOMEPAGE, g_lag.LoadString("label.skinhomepage"));
	SetDlgItemText(IDC_STATIC_HOMEPAGE_NAME, g_lag.LoadString("label.skinhomepagename"));
	SetDlgItemText(IDC_STATIC_SKIN_EAMIL, g_lag.LoadString("label.skinemail"));
	SetDlgItemText(IDC_STATIC_SKIN_EAMIL_NAME, g_lag.LoadString("label.skinemailname"));

	m_skinbox.AddString(THREME_NAME_NORMAL);
	m_skinbox.AddString(THREME_NAME_2007);
	m_skinbox.AddString(THREME_NAME_VISTA);
	m_skinbox.AddString(THREME_NAME_XP_ROYALE);
	m_skinbox.AddString(THREME_NAME_XP_LUNA);
	m_skinbox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
extern DWORD g_dwApplyID;
extern sloSkinAgent	g_skin;
BOOL CSkinDlg::OnApply()
{
	//	ASSERT_VALID(this);	
	if (g_dwApplyID == 3)
	{
		//������д���ļ�������������
		if (strlen(m_szSkinTheme))
		{
			g_skin.UpdateSkin(m_szSkinTheme);
		}
		
		return TRUE;
	}
	
	return TRUE;
}

BOOL CSkinDlg::OnSetActive()
{
	g_dwApplyID = 3;
	
	return CXTPPropertyPage::OnSetActive();
}

BOOL CSkinDlg::OnKillActive()
{
	return CXTPPropertyPage::OnKillActive();
}

void CSkinDlg::OnSelchangeListSkin() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_skinbox.GetCurSel();
	CString strText;
	m_skinbox.GetText(nIndex, strText);
	memset(&m_szSkinTheme, NULL, MAX_PATH);
	if (strText == THREME_NAME_NORMAL)
		strText = THREME_NAME_2007;

	memcpy(&m_szSkinTheme, strText.GetBuffer(0), strText.GetLength());

	SetModified();
	//�任Ԥ��ͼ��
//	MessageBox(strText);

	CEnBitmap bitmap;
	CBitmap bit;
	if (strText == THREME_NAME_NORMAL || strText == THREME_NAME_2007 )
	{
		bit.LoadBitmap(IDB_BITMAP_OFFICE2007);
	}else if (strText == THREME_NAME_VISTA )
	{
		bit.LoadBitmap(IDB_BITMAP_VISTA);
	}else if (strText == THREME_NAME_XP_ROYALE)
	{
		bit.LoadBitmap(IDB_BITMAP_XPPROYALE);
	}else if (strText == THREME_NAME_XP_LUNA)
	{
		bit.LoadBitmap(IDB_BITMAP_XPPLUNA);
	}

	if (bitmap.CopyImage(&bit))
	{
		m_bitmapOrg.DeleteObject();
		m_bitmapOrg.Attach(bitmap.Detach());
		
		VERIFY(m_bitmap.CopyImage(&m_bitmapOrg));
	
		UpdateData(FALSE);
		
		OnChangeProcessing();
	}
}

void CSkinDlg::OnChangeProcessing() 
{
	UpdateData();

//	if (m_bSpinning)
//		return;

	if (m_bitmapOrg.GetSafeHandle())
	{
		m_bitmap.CopyImage(&m_bitmapOrg);
		
		DWORD dwTick = GetTickCount();

		if (m_bUseProcessorArray)
		{
			CImageRotator processRotate(m_nRotationAngle * 30, m_bWeighting);
			CImageShearer processShear(m_bShearAcross ? (m_nXShearAmount - 5) * 40 : 0, 
										m_bShearDown ? (m_nYShearAmount - 5) * 40 : 0, m_bWeighting);
			CImageGrayer processGray;
			CImageBlurrer processBlur(m_nBlurFactor + 1);
			CImageSharpener processSharpen(m_nSharpenFactor + 1);
			CImageResizer processEnlarge(1 + (m_nEnlargeFactor + 1) / 10.0);
			CImageResizer processShrink((m_nShrinkFactor + 1) / 10.0);
			CImageNegator processNegate;
			CImageFlipper processFlip(m_bFlipHorz, m_bFlipVert); 
			CColorReplacer processColor(0, 255);

			C32BIPArray aProcessors;

			if (m_bGrayscale)
				aProcessors.Add(&processGray);
			
			if (m_bNegate)
				aProcessors.Add(&processNegate);
			
			if (m_bReplaceBlack)
				aProcessors.Add(&processColor);
			
			if (m_bFlipHorz || m_bFlipVert)
				aProcessors.Add(&processFlip);
			
			if (m_bEnlarge)
				aProcessors.Add(&processEnlarge);
			
			if (m_bShrink)
				aProcessors.Add(&processShrink);
			
			if (m_bBlur)
				aProcessors.Add(&processBlur);
			
			if (m_bShearDown || m_bShearAcross)
				aProcessors.Add(&processShear);
			
			if (m_bRotate)
				aProcessors.Add(&processRotate);

			if (m_bSharpen)
				aProcessors.Add(&processSharpen);

			m_bitmap.ProcessImage(aProcessors);
		}
		else
		{
			if (m_bGrayscale)
				m_bitmap.GrayImage();
			
			if (m_bNegate)
				m_bitmap.NegateImage();
			
			if (m_bReplaceBlack)
				m_bitmap.ReplaceColor(0, 255);
			
			if (m_bFlipHorz || m_bFlipVert)
				m_bitmap.FlipImage(m_bFlipHorz, m_bFlipVert);
			
			if (m_bEnlarge)
				m_bitmap.ResizeImage(1 + (m_nEnlargeFactor + 1) / 10.0);
			
			if (m_bShrink)
				m_bitmap.ResizeImage((m_nShrinkFactor + 1) / 10.0);
			
			if (m_bBlur)
				m_bitmap.BlurImage(m_nBlurFactor + 1);
			
			if (m_bShearDown || m_bShearAcross)
				m_bitmap.ShearImage(m_bShearAcross ? (m_nXShearAmount - 5) * 40 : 0, 
									m_bShearDown ? (m_nYShearAmount - 5) * 40 : 0, m_bWeighting);

			if (m_bRotate)
				m_bitmap.RotateImage(m_nRotationAngle * 30, m_bWeighting);

			if (m_bSharpen)
				m_bitmap.SharpenImage(m_nSharpenFactor + 1);
		}

		m_sTimeTaken.Format("%0.03f seconds", (GetTickCount() - dwTick) / 1000.0f);
		UpdateData(FALSE);

		Invalidate(FALSE);
	}
}

void CSkinDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_bitmap.GetSafeHandle())
	{
		CDC dcMem;
		
		if (dcMem.CreateCompatibleDC(&dc))
		{
			CBitmap* pOldBM = dcMem.SelectObject(&m_bitmap);
			BITMAP BM;
			
			m_bitmap.GetBitmap(&BM);
			
			dc.BitBlt(175, 73, BM.bmWidth, BM.bmHeight, &dcMem, 0, 0, SRCCOPY);
			
			dcMem.SelectObject(pOldBM);
			
			// fill any gaps
			if (m_sizePrev.cx > BM.bmWidth)
			{
				CRect rGap(BM.bmWidth, 0, m_sizePrev.cx, max(BM.bmHeight, m_sizePrev.cy));
				dc.FillSolidRect(rGap, GetSysColor(COLOR_3DFACE));
			}
			
			if (m_sizePrev.cy > BM.bmHeight)
			{
				CRect rGap(0, BM.bmHeight, max(BM.bmWidth, m_sizePrev.cx), m_sizePrev.cy);
				dc.FillSolidRect(rGap, GetSysColor(COLOR_3DFACE));
			}
			
			m_sizePrev.cx = BM.bmWidth;
			m_sizePrev.cy = BM.bmHeight;
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}