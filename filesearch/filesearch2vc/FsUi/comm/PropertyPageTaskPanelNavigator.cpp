// PropertyPageTaskPanelNavigator.cpp: implementation of the CPropertyPageTaskPanelNavigator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FsUi.h"
#include "PropertyPageTaskPanelNavigator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropertyPageTaskPanelNavigator::CPropertyPageTaskPanelNavigator()
{

}

CPropertyPageTaskPanelNavigator::~CPropertyPageTaskPanelNavigator()
{

}

BOOL CPropertyPageTaskPanelNavigator::Create()
{
	CFont* pFont = m_pSheet->GetFont();
	
	if (!CXTPTaskPanel::Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP | WS_GROUP, CRect(0, 0, 0, 0), m_pSheet, 2))
		return FALSE;
	
	SetBehaviour(xtpTaskPanelBehaviourToolbox);
	SetTheme(xtpTaskPanelThemeShortcutBarOffice2007);


	CXTPPaintManager::SetTheme( xtpThemeRibbon);
	SetSelectItemOnFocus(TRUE);
	
	SetIconSize(CSize(18, 18));
//	SetGroupIconSize(CSize(55, 55));

	SetFont(pFont);
	
	//��ӻ�������group
	CXTPTaskPanelGroup* pGroup = AddGroup(0,IDI_ICON_BASIC_SET);
	pGroup->SetCaption(g_lag.LoadString("title.basicset"));

	//��ӳ���item
	int i = 0;
	CXTPTaskPanelGroupItem* pItem = pGroup->AddLinkItem(0, IDI_ICON_BASIC_SET_NORMAL);
	pItem->SetCaption(g_lag.LoadString("title.general"));

	CXTPPropertyPage* pPage = m_pSheet->GetPage(0);
	pItem->SetItemData((DWORD_PTR)pPage);
	pPage->m_dwData = (DWORD_PTR)pItem;

	//��Ӵʻ�item
	i = 1;
	pItem = pGroup->AddLinkItem(1, IDI_ICON_BASIC_SET_DIC);
	pItem->SetCaption(g_lag.LoadString("title.words"));
	
	pPage = m_pSheet->GetPage(1);
	pItem->SetItemData((DWORD_PTR)pPage);
	pPage->m_dwData = (DWORD_PTR)pItem;

	//���Ƥ��item
	i = 2;
	pItem = pGroup->AddLinkItem(2, IDI_ICON_BASIC_SET_SKIN);
	pItem->SetCaption(g_lag.LoadString("title.skin"));
	
	pPage = m_pSheet->GetPage(2);
	pItem->SetItemData((DWORD_PTR)pPage);
	pPage->m_dwData = (DWORD_PTR)pItem;

	//����ȼ�item
	i = 3;
	pItem = pGroup->AddLinkItem(3, IDI_ICON_HOTKEY);
	pItem->SetCaption(g_lag.LoadString("title.hotkey"));
	
	pPage = m_pSheet->GetPage(3);
	pItem->SetItemData((DWORD_PTR)pPage);
	pPage->m_dwData = (DWORD_PTR)pItem;

	//���ע�����group
	pGroup = AddGroup(1,IDI_ICON_REGISTER);
	pGroup->SetCaption(g_lag.LoadString("title.licensemanager"));

	//���ע����Ϣitem
	pItem = pGroup->AddLinkItem(0, IDI_ICON_REGISTER_INFO);
	pItem->SetCaption(g_lag.LoadString("title.licenseinfo"));
	
	pPage = m_pSheet->GetPage(4);
	pItem->SetItemData((DWORD_PTR)pPage);
	pPage->m_dwData = (DWORD_PTR)pItem;

//	pGroup = AddGroup(2);
//	pGroup->SetCaption(_T("״̬������"));

	int nMargin = 2;
	GetPaintManager()->m_rcItemOuterMargins.SetRect(nMargin, nMargin, nMargin, nMargin);

	Reposition();

	GetImageManager()->SetIcon(IDI_ICON_BASIC_SET_NORMAL, IDI_ICON_BASIC_SET_NORMAL);
	GetImageManager()->SetIcon(IDI_ICON_SET_NORMAL, IDI_ICON_SET_NORMAL);
	GetImageManager()->SetIcon(IDI_ICON_BASIC_SET, IDI_ICON_BASIC_SET);
	GetImageManager()->SetIcon(IDI_ICON_CIHUI, IDI_ICON_CIHUI);
	GetImageManager()->SetIcon(IDI_ICON_CIHUI1, IDI_ICON_CIHUI1);
	GetImageManager()->SetIcon(IDI_ICON_BASIC_SET_DIC, IDI_ICON_BASIC_SET_DIC);
	GetImageManager()->SetIcon(IDI_ICON_REGISTER, IDI_ICON_REGISTER);
	GetImageManager()->SetIcon(IDI_ICON_REGISTER_INFO, IDI_ICON_REGISTER_INFO);	
	GetImageManager()->SetIcon(IDI_ICON_BASIC_SET_SKIN, IDI_ICON_BASIC_SET_SKIN);
	GetImageManager()->SetIcon(IDI_ICON_HOTKEY, IDI_ICON_HOTKEY);
//	SetGroupIconSize( CSize(16, 24));
	SetGroupIconSize( CSize(24, 48));

//	m_pSheet->SetPageBorderStyle(xtpPageBorderFrame);
	
	return TRUE;
}

void CPropertyPageTaskPanelNavigator::OnPageSelected(CXTPPropertyPage* pPage)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)pPage->m_dwData;
	SetFocusedItem(pItem);
}

void CPropertyPageTaskPanelNavigator::SetFocusedItem(CXTPTaskPanelItem* pItem, BOOL bDrawFocusRect /*= FALSE*/, BOOL bSetFocus)
{
	if (m_pItemFocused != pItem && pItem && pItem->GetType() == xtpTaskItemTypeLink)
	{
		CXTPPropertyPage* pPage = (CXTPPropertyPage*)pItem->GetItemData();
		if (!m_pSheet->SetActivePage(pPage))
		{
			return;
		}
	}
	
	CXTPTaskPanel::SetFocusedItem(pItem, bDrawFocusRect, bSetFocus);
	
}