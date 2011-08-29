// sloFileOpenAddin.cpp: implementation of the sloFileOpenAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "officesearch.h"
#include "sloFileOpenAddin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sloFileOpenAddin::sloFileOpenAddin()
{

}

sloFileOpenAddin::~sloFileOpenAddin()
{

}

/*
����ֵ��
     0 �ɹ�
*/
DWORD sloFileOpenAddin::OpenFile_WORD(char* pFileName, int nPage, char* pKeyWords /*= NULL*/)
{
	DWORD dwRet = 0;
	do 
	{
		try
		{
			CoInitialize(NULL);
			
			HRESULT hr = S_OK;
			//����wordʵ��
			Word::_ApplicationPtr spWordApp = NULL;
			{
				hr = spWordApp.CreateInstance(L"Word.Application.11");
			}
			if (FAILED(hr))
			{
				dwRet = -1;
				break;
			}
			spWordApp->put_Visible(VARIANT_TRUE);
			
			//��word�ĵ�
			CComPtr<Word::_Document> document;
			CComPtr<Documents> spDocs = NULL;
			spWordApp->get_Documents(&spDocs);
			if (spDocs == NULL)
			{
				dwRet = -2;
				break;
			}
			
			CComVariant filename(pFileName);
			document = spDocs->Open(&filename, &vtMissing, &vtMissing, &CComVariant(VARIANT_TRUE));
			if (document == NULL)
			{
				dwRet = -3;
				break;
			}

			//���ִ�еڼ�ҳ
			CComPtr<Word::Selection> sel;
			spWordApp->get_Selection(&sel);
			
			CComVariant vName(nPage);
			CComVariant count(1);
			CComVariant What(wdGoToPage);
			CComVariant Which(wdGoToNext);
			CComPtr<Word::Range> range;
			range = sel->GoTo(&What, &Which, &count, &vName);
			
			//û�йؼ��ֲ��ҹ���
			if (!pKeyWords)
			{
				dwRet = 0;
				break;
			}

			//���ҹ���
			VARIANT pagecount;
			sel->get_Information(wdNumberOfPagesInDocument, &pagecount);
			int nPagecount = pagecount.lVal;
			
			CComPtr<Word::Find> find;
			sel->get_Find(&find);
			find->ClearFormatting();
			
			CComVariant vFindText(pKeyWords), vForward(VARIANT_TRUE);
			CComVariant   Warp(_T( "wdFindContinue "));
			hr = find->Execute( &vFindText, 
				&vtMissing, 
				&vtMissing, 
				&vtMissing, 
				&vtMissing, 
				&vtMissing, 
				&vForward, 
				&Warp,
				&vtMissing,
				&vtMissing,
				&vtMissing,
				&vtMissing, 
				&vtMissing, 
				&vtMissing,
				&vtMissing);
// 			COleVariant   Text=(_T("1")); 
// 			CComVariant   MatchCase(_T( "FALSE ")); 
// 			CComVariant   MatchWholeWord(_T( "TRUE ")); 
// 			CComVariant   MatchWildcards(_T( "FALSE ")); 
// 			CComVariant   MatchSoundsLike(_T( "FALSE ")); 
// 			CComVariant   MatchAllWordForms(_T( "FALSE ")); 
// 			CComVariant   Forward(_T( "TRUE ")); 
// 			CComVariant   Warp(_T( "wdFindContinue ")); 
// 			CComVariant   Format(_T( "FALSE ")); 
// 			COleVariant   ReplaceWith(_T( "")); 
// 			CComVariant   Replace(_T( "wdReplaceNone ")); 
// 			CComVariant   MatchKashida(_T( "FALSE ")); 
// 			CComVariant   MatchDiacritics(_T( "FALSE ")); 
// 			CComVariant   MatchAlefHamza(_T( "FALSE ")); 
// 			CComVariant   MatchControl(_T( "FALSE ")); 
// 			hr = find->Execute(&Text,&MatchCase,&MatchWholeWord,&MatchWildcards, 
// 				&MatchSoundsLike,&MatchAllWordForms,&Forward,&Warp,&Format, 
// 				&ReplaceWith,&Replace,&MatchKashida,&MatchDiacritics, 
// 				&MatchAlefHamza,&MatchControl); 
		}
		catch (...)
		{
			dwRet = -1;
			break;
		}
	} while (0);
	
	CoUninitialize();

	return dwRet;
	
}

DWORD sloFileOpenAddin::OpenFile_EXCEL(char* pFileName, char* pSheet, int nRow, char* pKeyWords/* = NULL*/)
{
	DWORD dwRet = 0;
	do 
	{
		try
		{
			CoInitialize(NULL);
			
			//����excelʵ��
			Excel::_ApplicationPtr spExcelApp;
			HRESULT hr = spExcelApp.CreateInstance(L"Excel.Application");
			if (FAILED(hr))
			{
				dwRet = -1;
				break;
			}
			
			spExcelApp->put_Visible(0, VARIANT_TRUE);
			
			//��excel�ĵ�
			CComPtr<Excel::Workbooks> spWorkbooks = NULL;
			spExcelApp->get_Workbooks(&spWorkbooks);
			if (spWorkbooks == NULL)
			{
				dwRet = -2;
				break;
			}
			
			CComPtr<Excel::_Workbook> m_spWb;
			m_spWb = spWorkbooks->Open(pFileName);
			if (m_spWb == NULL)
			{
				dwRet = -3;
				break;
			}
			
			//��λ
			char szSheet[MAX_PATH] = {0};
			sprintf(szSheet, "%s!R%dC1", pSheet, nRow);
			CComVariant vsheet(szSheet);
			spExcelApp->Goto(&vsheet, &vtMissing, 0);
			
			//û�йؼ��ֲ��ҹ���
			if (!pKeyWords)
			{
				break;
			}

			//���ҹؼ��� 
			CComVariant What(pKeyWords), LookIn(xlFormulas), LookAt(xlPart), SearchOrder(xlByRows);
			CComPtr<Excel::Range> range;
			range = spExcelApp->GetCells()->Find(&What, 
				&vtMissing, 
				&LookIn,
				&LookAt, 
				&SearchOrder, 
				xlNext, 
				&vtMissing, 
				&vtMissing,	
				&vtMissing);

			range->Activate();	
		}
		catch (...)
		{
			dwRet = -1;
			break;	
		}
	} while (0);
	
	CoUninitialize();

	return dwRet;

}

DWORD sloFileOpenAddin::OpenFile_PPT(char* pFileName, int nPage, char* pKeyWords/* = NULL*/)
{
	DWORD dwRet = 0;
	do 
	{
		try
		{
			CoInitialize(NULL);
			PPT::_ApplicationPtr spPPTApp;
			HRESULT hr = spPPTApp.CreateInstance(L"PowerPoint.Application");
			if (FAILED(hr))
			{
				dwRet = -1;
				break;
			}
			
			spPPTApp->put_Visible(msoTrue);
			
			spPPTApp->Activate();
			
			CComPtr<Presentations> spPresents = NULL;
			spPPTApp->get_Presentations(&spPresents);
			if (spPresents == NULL)
			{
				dwRet = -2;
				break;
			}
			
			CComPtr<PPT::_Presentation> m_spPres;
			m_spPres = spPresents->Open(pFileName, msoFalse, msoFalse, msoTrue);
			if (m_spPres == NULL)
			{
				dwRet = -3;
				break;
			}
			
			CComVariant item(2);
			m_spPres->GetSlides()->Item(&item)->Select();
			
			//��������
			CComPtr<PPT::DocumentWindow> docwin;
			spPPTApp->get_ActiveWindow(&docwin);
			
			CComPtr<PPT::Shapes> shapes;
			docwin->GetSelection()->GetSlideRange()->get_Shapes(&shapes);
			
			CComBSTR strText(pKeyWords);
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
		catch (...)
		{
			dwRet = -1;
			break;
		}

	} while (0);

	return dwRet;
}

DWORD sloFileOpenAddin::OpenFile_PDF(char* pFileName, int nPage, char* pKeyWords/* = NULL*/)
{
	DWORD dwRet =0 ;

	return dwRet;
}

DWORD sloFileOpenAddin::OpenFile_TXT(char* pFileName, int nRow, char* pKeyWords/* = NULL*/)
{
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
			_splitpath(pFileName, NULL, NULL, fname, ext);


			sprintf(szWindowsName, "%s%s - ���±�", fname,ext);

			//�����ı��Ƿ��
			HWND txtwnd = FindWindow("NotePad", szWindowsName);
			if (txtwnd == NULL)
			{
				//�򿪸��ĵ�
				ShellExecute(NULL, "open", pFileName, NULL, NULL, SW_SHOW);

				Sleep(200);

				//�ٴμ����ı��Ƿ��

				txtwnd = FindWindow("NotePad", szWindowsName);
				if (txtwnd == NULL)
				{
					dwRet = -3;
					break;
				}
			}

			//����edit�ؼ����
			HWND hwnd = FindWindowEx(txtwnd, NULL, "Edit", "");
			
			CEdit edit;
			edit.Attach(hwnd);
			
			edit.SetFocus();
			
			int nLine = nRow - 1;
			int nIndex = -1;
			//lineת��
			nIndex = edit.LineIndex(nLine);
			if (nIndex == -1)
			{
				//���������˷�Χ
				dwRet = -2;
				break;
			}
			edit.SetSel(nIndex,nIndex);
				
			edit.Detach();
		}
		catch (...)
		{
			dwRet = -1;
			break;
		}
	} while (0);

	return dwRet;	
}
