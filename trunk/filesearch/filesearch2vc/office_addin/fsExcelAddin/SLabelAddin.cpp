// SLabelAddin.cpp : Implementation of CSLabelAddin
#include "stdafx.h"
#include "SLabelExcelAddin.h"
#include "SLabelAddin.h"

_ATL_FUNC_INFO OnClickButtonInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF|VT_BOOL } };

/////////////////////////////////////////////////////////////////////////////
// CSLabelAddin
HRESULT CSLabelAddin::OnConnection(IDispatch *Application, ext_ConnectMode ConnectMode,
								   IDispatch *AddInInst, SAFEARRAY **custom)
{
	OutputDebugStringW(L"[addin] OnConnection\r\n");

	UNREFERENCED_PARAMETER(ConnectMode);
	UNREFERENCED_PARAMETER(AddInInst);
	UNREFERENCED_PARAMETER(custom);

	m_Application = Application;
	ATLASSERT(m_Application);
	AppEvents4::DispEventAdvise((IDispatch*)m_Application);

	CComPtr<_CommandBars> spCmdBars;
	m_Application->get_CommandBars(&spCmdBars);
	ATLASSERT(spCmdBars);

	// ���һ��������
	CComVariant vName("��Ϣ����");
	CComPtr<CommandBar> spNewCmdBar;
	CComVariant vPos(1);
	CComVariant vTemp(VARIANT_TRUE);
	CComVariant vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);
	spNewCmdBar = spCmdBars->Add(vName, vPos, vEmpty, vTemp);

	// ��ȡ����ӹ�������CommandBarControls
	CComPtr<CommandBarControls> spBarControls;
	spNewCmdBar->get_Controls(&spBarControls);
	ATLASSERT(spBarControls);
	
	m_spCmdButtonSet = AddCmdButton(spBarControls, IDB_SET, L"", L"����ļ�������");

	// ��ʾ������
	spNewCmdBar->PutVisible(VARIANT_TRUE);

	if (m_spCmdButtonSet)
	{
		CommandButtonEvents_Set::DispEventAdvise((IDispatch *)m_spCmdButtonSet);
	}

	return S_OK;
}

HRESULT CSLabelAddin::OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY **custom)
{
	OutputDebugStringW(L"[addin] OnDisconnection\r\n");

	UNREFERENCED_PARAMETER(custom);

	if (m_Application)
	{
		AppEvents4::DispEventUnadvise((IDispatch *)m_Application);
	}

	if (m_spCmdButtonSet)
	{
		CommandButtonEvents_Set::DispEventUnadvise((IDispatch *)m_spCmdButtonSet);
	}

	return S_OK;
}

void __stdcall CSLabelAddin::OnClickButtonSet(IDispatch *Ctrl,
		VARIANT_BOOL *CancelDefault)
{
	OutputDebugStringW(L"[addin] OnClickButtonSet\r\n");

}

CComQIPtr<_CommandBarButton> CSLabelAddin::AddCmdButton(CComPtr<CommandBarControls> spBarControls, UINT uBitmapID,BSTR strCaption, BSTR strTag)
{
	// ��Ӱ�ť
	CComVariant vToolBarType(1);
	CComVariant vShow(VARIANT_TRUE);
	CComVariant vEmpty(DISP_E_PARAMNOTFOUND, VT_ERROR);
	CComPtr<CommandBarControl> spNewBar;
	
	// ��Ӱ�ť
	spNewBar = spBarControls->Add(vToolBarType, vEmpty, vEmpty, vEmpty, vShow);
	ATLASSERT(spNewBar);
	
	CComQIPtr<_CommandBarButton> spCmdButton(spNewBar);
	ATLASSERT(spCmdButton);
	HBITMAP hBmp = (HBITMAP)::LoadImage(_Module.GetResourceInstance(),
		MAKEINTRESOURCE(uBitmapID), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
	
    // add zhw  
	// ΪCOMMANDBARBUTTON���뱳��͸����ͼƬ
	m_BitmapHelper.ModifyTransBitmap(hBmp);
	m_BitmapHelper.PutPicture(spCmdButton, hBmp);
	::DeleteObject(hBmp);
	
	spCmdButton->PutStyle(Office::msoButtonIconAndCaption);
	spCmdButton->PutVisible(VARIANT_TRUE);
	spCmdButton->PutCaption(strCaption);
	spCmdButton->PutEnabled(VARIANT_TRUE);
	spCmdButton->PutTooltipText(strTag);
	return spCmdButton;
}

STDMETHODIMP CSLabelAddin::OnOpen(IDispatch *Ctrl, VARIANT *CancelDefault)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSLabelAddin::OnCut(IDispatch *Ctrl, VARIANT *CancelDefault)
{
	return S_OK;
}

STDMETHODIMP CSLabelAddin::OnCopy(IDispatch *Ctrl, VARIANT *CancelDefault)
{
	return S_OK;
}

STDMETHODIMP CSLabelAddin::OnPaste(IDispatch *Ctrl, VARIANT *CancelDefault)
{
	return S_OK;
}
