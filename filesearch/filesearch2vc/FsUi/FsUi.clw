; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=13
Class1=CFsUiApp
LastClass=CLicenseDlg
NewFileInclude2=#include "FsUi.h"
ResourceCount=20
NewFileInclude1=#include "stdafx.h"
Class2=CCreateIndexDlg
LastTemplate=CDialog
Resource1=IDD_DIALOG_ALL_CIHUI
Class3=CCustomCiHuiDlg
Resource2=IDD_DIALOG_SET_ATTR
Class4=CModifyIndexDlg
Resource3=IDD_DIALOG_CUSTOM_DICT (Chinese (P.R.C.))
Class5=CShowAllCiHuiDlg
Resource4=IDD_DIALOG_MODIFY_INDEX
Class6=CSetAttributeDlg
Resource5=IDD_FLOATWND
Class7=CFastSearchDlg
Class8=CSearchThread
Resource6=IDD_DIALOG_SKIN (Chinese (P.R.C.))
Class9=CLicenseDlg
Resource7=IDD_DIALOG_SAVE_FILE
Class10=CSkinDlg
Resource8=IDD_DIALOG_MODIFY_INDEX (Chinese (P.R.C.))
Resource9=IDD_DIALOG_ALL_CIHUI (Chinese (P.R.C.))
Resource10=IDD_DIALOG_LICENSE (Chinese (P.R.C.))
Resource11=IDD_DIALOG_FAST_SEARCH
Resource12=IDD_DIALOG_FAST_SEARCH (Chinese (P.R.C.))
Resource13=IDD_DIALOG_CREATE_INDEX (Chinese (P.R.C.))
Resource14=IDD_DIALOG_CUSTOM_DICT
Resource15=IDD_DIALOG_SET_ATTR (Chinese (P.R.C.))
Resource16=IDD_DIALOG_HOTKEY
Class11=CFloatWnd
Resource17=IDD_DIALOG_SKIN
Resource18=IDD_DIALOG_LICENSE
Class12=CAddinSavefileDlg
Resource19=IDD_DIALOG_CREATE_INDEX
Class13=CHotkeyDlg
Resource20=IDR_MENU_FLOATWND

[CLS:CFsUiApp]
Type=0
HeaderFile=FsUi.h
ImplementationFile=FsUi.cpp
Filter=N
LastObject=CFsUiApp
BaseClass=CWinApp
VirtualFilter=AC

[DLG:IDD_DIALOG_CREATE_INDEX]
Type=1
Class=CCreateIndexDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SEARCH_DIR,static,1342308354
Control4=IDC_EDIT_SEARCH_PATH,edit,1350633600
Control5=IDC_BUTTON_BROWSE,button,1342242816
Control6=IDC_STATIC_OBJECT_TYPE,static,1342308354
Control7=IDC_CHECK_EXCEL,button,1342242819
Control8=IDC_CHECK_WORD,button,1342242819
Control9=IDC_CHECK_PPT,button,1342242819
Control10=IDC_CHECK_PDF,button,1342242819
Control11=IDC_CHECK_HTML,button,1342242819
Control12=IDC_CHECK_TXT,button,1342242819

[CLS:CCreateIndexDlg]
Type=0
HeaderFile=CreateIndexDlg.h
ImplementationFile=CreateIndexDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCreateIndexDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_CUSTOM_DICT]
Type=1
Class=CCustomCiHuiDlg
ControlCount=13
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_CIHUI_2,static,1342308352
Control4=IDC_EDIT_CIHUI_PATH,edit,1350633600
Control5=IDC_BUTTON_BROWSE_CIHUI,button,1342242816
Control6=IDC_CHECK_OVERCIHUI,button,1342242819
Control7=IDC_BUTTON_CHECK_EXISTCIHUI,button,1342242816
Control8=IDC_STATIC_CUSTOM_CIHUI,static,1342308352
Control9=IDC_STATIC_CIHUI_1,static,1342308352
Control10=IDC_STATIC_IMPORT_CIHUI,static,1342308352
Control11=IDC_STATIC_CIHUI_3,static,1342308352
Control12=IDC_STATIC,static,1342177283
Control13=IDC_STATIC,static,1342177283

[CLS:CCustomCiHuiDlg]
Type=0
HeaderFile=CustomCiHuiDlg.h
ImplementationFile=CustomCiHuiDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCustomCiHuiDlg

[DLG:IDD_DIALOG_MODIFY_INDEX]
Type=1
Class=CModifyIndexDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SEARCH_DIR_MODIFY,static,1342308354
Control4=IDC_STATIC_OBJECT_TYPE_MODIFY,static,1342308354
Control5=IDC_STATIC_SEARCHPATH,static,1342308352
Control6=IDC_CHECK_EXCEL,button,1342242819
Control7=IDC_CHECK_WORD,button,1342242819
Control8=IDC_CHECK_PDF,button,1342242819
Control9=IDC_CHECK_PPT,button,1342242819
Control10=IDC_CHECK_HTML,button,1342242819
Control11=IDC_CHECK_TXT_MODIFY,button,1342242819
Control12=IDC_CHECK_CHM,button,1073807363
Control13=IDC_BUTTON_DEL_INDEX,button,1342242816

[CLS:CModifyIndexDlg]
Type=0
HeaderFile=ModifyIndexDlg.h
ImplementationFile=ModifyIndexDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CModifyIndexDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_ALL_CIHUI]
Type=1
Class=CShowAllCiHuiDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_ALL_CIHUI,listbox,1352728835
Control3=IDC_STATIC_NEW_WORDS,static,1342308354
Control4=IDC_EDIT_KEY_WORD,edit,1350631552

[CLS:CShowAllCiHuiDlg]
Type=0
HeaderFile=ShowAllCiHuiDlg.h
ImplementationFile=ShowAllCiHuiDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CShowAllCiHuiDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_SET_ATTR]
Type=1
Class=CSetAttributeDlg
ControlCount=23
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_IE_PATH,edit,1350633600
Control10=IDC_STATIC_SET_1,static,1342308352
Control11=IDC_EDIT_WORD_PATH,edit,1350633600
Control12=IDC_EDIT_EXCEL_PATH,edit,1350633600
Control13=IDC_EDIT_PPT_PATH,edit,1350633600
Control14=IDC_EDIT_PDF_PATH,edit,1350633600
Control15=IDC_EDIT_TXT_PATH,edit,1350633600
Control16=IDC_BUTTON_BROWSER_IE,button,1342242816
Control17=IDC_BUTTON_BROWSER_WORD,button,1342242816
Control18=IDC_BUTTON_BROWSER_EXCEL,button,1342242816
Control19=IDC_BUTTON_BROWSER_PPT,button,1342242816
Control20=IDC_BUTTON_BROWSER_PDF,button,1342242816
Control21=IDC_BUTTON_BROWSER_TXT,button,1342242816
Control22=IDC_STATIC_SOFT_SET,static,1342308352
Control23=IDC_STATIC,static,1342177283

[CLS:CSetAttributeDlg]
Type=0
HeaderFile=SetAttributeDlg.h
ImplementationFile=SetAttributeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetAttributeDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_FAST_SEARCH]
Type=1
Class=CFastSearchDlg
ControlCount=7
Control1=IDC_EDIT_SEARCH_KEY,edit,1350631556
Control2=IDC_COMBO_PATH,combobox,1344340227
Control3=IDC_STATIC_FIND,static,1073741838
Control4=IDC_COMBO_PATH_RECENT,combobox,1344340227
Control5=IDC_STATIC,button,1073741831
Control6=IDC_STATIC_BACK,static,1073741838
Control7=IDC_PROGRESS_FAST_SEARCH,msctls_progress32,1082130432

[CLS:CFastSearchDlg]
Type=0
HeaderFile=FastSearchDlg.h
ImplementationFile=FastSearchDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=CFastSearchDlg

[CLS:CSearchThread]
Type=0
HeaderFile=SearchThread.h
ImplementationFile=SearchThread.cpp
BaseClass=CWinThread
Filter=N
LastObject=CSearchThread

[DLG:IDD_DIALOG_LICENSE]
Type=1
Class=CLicenseDlg
ControlCount=27
Control1=IDC_STATIC_LICENSE_INFO,static,1342308352
Control2=IDC_STATIC_LICENSE_1,static,1342308352
Control3=IDC_STATIC_NUM,static,1342308352
Control4=IDC_STATIC_EMAIL,static,1342308352
Control5=IDC_EDIT_NUM,edit,1350631552
Control6=IDC_EDIT_EMAIL,edit,1350631552
Control7=IDC_STATIC_ENDTIME,static,1342308352
Control8=IDC_BUTTON_REG,button,1342242816
Control9=IDC_STATIC_BEGINTIME_CONTENT,static,1342308352
Control10=IDC_STATIC_ENTTIME_CONTENT,static,1342308352
Control11=IDC_STATIC_BEGINTIME,static,1342308352
Control12=IDC_STATIC_ERROR_INFO,static,1342308352
Control13=IDC_STATIC,static,1342177283
Control14=IDC_STATIC_HAND_LICENSE_BACK,static,1342177283
Control15=IDC_STATIC_LICENSE_BACKINFO,static,1342308352
Control16=IDC_EDIT_LICENSE_BACK_PATH,edit,1350633600
Control17=IDC_BUTTON_LICENSE_BACK,button,1342242816
Control18=IDC_STATIC_LICENSE_BACK_TIP,static,1342308352
Control19=IDC_BUTTON_BROWSE_BACK_PATH,button,1342242816
Control20=IDC_STATIC_LICENSE_BACK_INFO2,static,1342308352
Control21=IDC_STATIC_HAND_LICENSE_RESOTRE,static,1342177283
Control22=IDC_STATIC_LICENSE_RESTOREINFO,static,1342308352
Control23=IDC_STATIC_LICENSE_RESTORE_INFO2,static,1342308352
Control24=IDC_STATIC_LICENSE_RESTORE_TIP,static,1342308352
Control25=IDC_EDIT_LICENSE_RESTORE_PATH,edit,1350633600
Control26=IDC_BUTTON_BROWSE_RESTORE_PATH,button,1342242816
Control27=IDC_BUTTON_LICENSE_RESTORE,button,1342242816

[CLS:CLicenseDlg]
Type=0
HeaderFile=LicenseDlg.h
ImplementationFile=LicenseDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_NUM
VirtualFilter=dWC

[DLG:IDD_DIALOG_SKIN]
Type=1
Class=CSkinDlg
ControlCount=12
Control1=IDC_STATIC_SKIN_SET,static,1342308352
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC_SKIN_SELECT,static,1342308352
Control4=IDC_LIST_SKIN,listbox,1353777411
Control5=IDC_STATIC_SKIN_PREVIEW,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC_AUTHOR,static,1342308354
Control8=IDC_STATIC_AUTHOR_NAME,static,1342308352
Control9=IDC_STATIC_HOMEPAGE,static,1342308354
Control10=IDC_STATIC_HOMEPAGE_NAME,static,1342308352
Control11=IDC_STATIC_SKIN_EAMIL,static,1342308354
Control12=IDC_STATIC_SKIN_EAMIL_NAME,static,1342308352

[CLS:CSkinDlg]
Type=0
HeaderFile=SkinDlg.h
ImplementationFile=SkinDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STATIC_AUTHOR

[DLG:IDD_DIALOG_CREATE_INDEX (Chinese (P.R.C.))]
Type=1
Class=CCreateIndexDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SEARCH_DIR,static,1342308354
Control4=IDC_EDIT_SEARCH_PATH,edit,1350633600
Control5=IDC_BUTTON_BROWSE,button,1342242816
Control6=IDC_STATIC_OBJECT_TYPE,static,1342308354
Control7=IDC_CHECK_EXCEL,button,1342242819
Control8=IDC_CHECK_WORD,button,1342242819
Control9=IDC_CHECK_PPT,button,1342242819
Control10=IDC_CHECK_PDF,button,1342242819
Control11=IDC_CHECK_HTML,button,1342242819
Control12=IDC_CHECK_TXT,button,1342242819

[DLG:IDD_DIALOG_CUSTOM_DICT (Chinese (P.R.C.))]
Type=1
Class=CCustomCiHuiDlg
ControlCount=13
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC_CIHUI_2,static,1342308352
Control4=IDC_EDIT_CIHUI_PATH,edit,1350633600
Control5=IDC_BUTTON_BROWSE_CIHUI,button,1342242816
Control6=IDC_CHECK_OVERCIHUI,button,1342242819
Control7=IDC_BUTTON_CHECK_EXISTCIHUI,button,1342242816
Control8=IDC_STATIC_CUSTOM_CIHUI,static,1342308352
Control9=IDC_STATIC_CIHUI_1,static,1342308352
Control10=IDC_STATIC_IMPORT_CIHUI,static,1342308352
Control11=IDC_STATIC_CIHUI_3,static,1342308352
Control12=IDC_STATIC,static,1342177283
Control13=IDC_STATIC,static,1342177283

[DLG:IDD_DIALOG_MODIFY_INDEX (Chinese (P.R.C.))]
Type=1
Class=CModifyIndexDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SEARCH_DIR_MODIFY,static,1342308354
Control4=IDC_STATIC_OBJECT_TYPE_MODIFY,static,1342308354
Control5=IDC_STATIC_SEARCHPATH,static,1342308352
Control6=IDC_CHECK_EXCEL,button,1342242819
Control7=IDC_CHECK_WORD,button,1342242819
Control8=IDC_CHECK_PDF,button,1342242819
Control9=IDC_CHECK_PPT,button,1342242819
Control10=IDC_CHECK_HTML,button,1342242819
Control11=IDC_CHECK_TXT_MODIFY,button,1342242819
Control12=IDC_CHECK_CHM,button,1073807363
Control13=IDC_BUTTON_DEL_INDEX,button,1342242816

[DLG:IDD_DIALOG_ALL_CIHUI (Chinese (P.R.C.))]
Type=1
Class=CShowAllCiHuiDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_ALL_CIHUI,listbox,1352728835
Control3=IDC_STATIC_NEW_WORDS,static,1342308354
Control4=IDC_EDIT_KEY_WORD,edit,1350631552

[DLG:IDD_DIALOG_SET_ATTR (Chinese (P.R.C.))]
Type=1
Class=CSetAttributeDlg
ControlCount=23
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_IE_PATH,edit,1350633600
Control10=IDC_STATIC_SET_1,static,1342308352
Control11=IDC_EDIT_WORD_PATH,edit,1350633600
Control12=IDC_EDIT_EXCEL_PATH,edit,1350633600
Control13=IDC_EDIT_PPT_PATH,edit,1350633600
Control14=IDC_EDIT_PDF_PATH,edit,1350633600
Control15=IDC_EDIT_TXT_PATH,edit,1350633600
Control16=IDC_BUTTON_BROWSER_IE,button,1342242816
Control17=IDC_BUTTON_BROWSER_WORD,button,1342242816
Control18=IDC_BUTTON_BROWSER_EXCEL,button,1342242816
Control19=IDC_BUTTON_BROWSER_PPT,button,1342242816
Control20=IDC_BUTTON_BROWSER_PDF,button,1342242816
Control21=IDC_BUTTON_BROWSER_TXT,button,1342242816
Control22=IDC_STATIC_SOFT_SET,static,1342308352
Control23=IDC_STATIC,static,1342177283

[DLG:IDD_DIALOG_FAST_SEARCH (Chinese (P.R.C.))]
Type=1
Class=CFastSearchDlg
ControlCount=3
Control1=IDC_EDIT_SEARCH_KEY,edit,1350631552
Control2=IDC_COMBO_PATH,combobox,1344340226
Control3=IDC_STATIC,static,1342177795

[DLG:IDD_DIALOG_LICENSE (Chinese (P.R.C.))]
Type=1
Class=CLicenseDlg
ControlCount=13
Control1=IDC_STATIC_LICENSE_INFO,static,1342308352
Control2=IDC_STATIC_LICENSE_1,static,1342308352
Control3=IDC_STATIC_NUM,static,1342308352
Control4=IDC_STATIC_EMAIL,static,1342308352
Control5=IDC_EDIT_NUM,edit,1350631552
Control6=IDC_EDIT_EMAIL,edit,1350631552
Control7=IDC_STATIC_ENDTIME,static,1342308352
Control8=IDC_BUTTON_REG,button,1342242816
Control9=IDC_STATIC_BEGINTIME_CONTENT,static,1342308352
Control10=IDC_STATIC_ENTTIME_CONTENT,static,1342308352
Control11=IDC_STATIC_BEGINTIME,static,1342308352
Control12=IDC_STATIC_ERROR_INFO,static,1342308352
Control13=IDC_STATIC,static,1342177283

[DLG:IDD_DIALOG_SKIN (Chinese (P.R.C.))]
Type=1
Class=CSkinDlg
ControlCount=12
Control1=IDC_STATIC_SKIN_SET,static,1342308352
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC_SKIN_SELECT,static,1342308352
Control4=IDC_LIST_SKIN,listbox,1352728835
Control5=IDC_STATIC_SKIN_PREVIEW,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC_AUTHOR,static,1342308354
Control8=IDC_STATIC_AUTHOR_NAME,static,1342308352
Control9=IDC_STATIC_HOMEPAGE,static,1342308354
Control10=IDC_STATIC_HOMEPAGE_NAME,static,1342308352
Control11=IDC_STATIC_SKIN_EAMIL,static,1342308354
Control12=IDC_STATIC_SKIN_EAMIL_NAME,static,1342308352

[DLG:IDD_FLOATWND]
Type=1
Class=CFloatWnd
ControlCount=1
Control1=IDC_LOGO,static,1342177294

[CLS:CFloatWnd]
Type=0
HeaderFile=FloatWnd.h
ImplementationFile=FloatWnd.cpp
BaseClass=CDialog
Filter=W
LastObject=CFloatWnd
VirtualFilter=dWC

[MNU:IDR_MENU_FLOATWND]
Type=1
Class=?
Command1=ID_MENU_NEW_INDEX
Command2=ID_MENU_SYS_SET
Command3=ID_MENU_WEB_QUERY
Command4=ID_MENU_TRANS_100
Command5=ID_MENU_TRANS_80
Command6=ID_MENU_TRANS_60
Command7=ID_MENU_TRANS_40
Command8=ID_MENU_TRANS_20
Command9=ID_HIDE
CommandCount=9

[DLG:IDD_DIALOG_SAVE_FILE]
Type=1
Class=CAddinSavefileDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PATH,edit,1350631552
Control5=IDC_TREE_GROUP,SysTreeView32,1350631431
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342177287

[CLS:CAddinSavefileDlg]
Type=0
HeaderFile=AddinSavefileDlg.h
ImplementationFile=AddinSavefileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAddinSavefileDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_HOTKEY]
Type=1
Class=CHotkeyDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC_HOTKEY,static,1342308352
Control3=IDC_STATIC_HOTKEY_FAST_SEARCH,static,1342308352
Control4=IDC_STATIC_HOTKEY_WEB_SEARCH,static,1342308352
Control5=IDC_STATIC_HOTKEY_DESC,static,1342308352
Control6=IDC_HOTKEY_FAST_SEARCH,msctls_hotkey32,1350631424
Control7=IDC_HOTKEY_WEB_SEARCH,msctls_hotkey32,1350631424
Control8=IDC_BUTTON_SET_HOTKEY,button,1342242816

[CLS:CHotkeyDlg]
Type=0
HeaderFile=HotkeyDlg.h
ImplementationFile=HotkeyDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_HOTKEY_FAST_SEARCH
VirtualFilter=dWC

