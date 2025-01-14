/**
 * $RCSfile: Constant.java
 * $Revision: 1.0
 * $Date: Jan 30, 2011
 *
 * Copyright (C) 2010 SlFile, Inc. All rights reserved.
 *
 * This software is the proprietary information of SlFile, Inc.
 * Use is subject to license terms.
 */
package com.searchlocal.constants;

import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;

/**
 * 常量类
 * 
 * <p>Title: 常量类</p>
 * <p>Description: </p>
 * <p>site: www.slfile.net</p>
 * @author changsong:qianjinfu@gmail.com
 * @version 1.0
 */
public class Constant {

	public static String datapath = "";
	static {
		try {
			datapath = Constant.class.getResource("/").toURI().getPath();

			// 发布的时候注释掉
//            datapath = datapath.substring(1);
//            datapath = datapath.replaceAll("classes/", "");
		} catch (URISyntaxException e) {
			// TODO 注意消除资源(关闭I/O等)
			e.printStackTrace();
		}
	}
	
	// #------------------------------------------------------------------
	// #--- web ----------------------------------------------------------
	// #------------------------------------------------------------------
	public static String web_searchname = "web.searchname";

	public static String web_searchpath = "web.searchpath";

	public static String web_changesearch = "web.changesearch";

	public static String web_allsearchtype = "web.allsearchtype";

	public static String web_webpage = "web.webpage";

	public static String web_source = "web.source";

	public static String web_button = "web.button";

	public static String web_returnhome = "web.returnhome";

	public static String web_at = "web.at";

	public static String web_findfile = "web.findfile";

	public static String web_rows = "web.rows";

	public static String web_the = "web.the";

	public static String web_photogragh = "web.photogragh";

	public static String web_excel_the = "web.excel.the";

	public static String web_excel_page = "web.excel.page";

	public static String web_row = "web.row";

	public static String web_page = "web.page";

	public static String web_chapter = "web.chapter";

	public static String web_previouspage = "web.previouspage";

	public static String web_nextpage = "web.nextpage";

	public static String web_preview = "web.preview";
	
	public static String web_view = "web.view";
	
	public static String web_innewwin = "web.innewwin";

	// #------------------------------------------------------------------
	// #------------------------------------------------------------------
	public static String label_dicfilename = "label.dicfilename";

	public static String label_dic_note_r1 = "label.labeldicnote_r1";

	public static String label_dic_note_r2 = "label.labeldicnote_r2";

	public static String label_dic_note_r3 = "label.labeldicnote_r3";

	public static String label_searchname = "label.searchname";

	public static String label_searchdir = "label.searchdir";

	public static String label_indexdir = "label.indexdir";

	public static String label_doc = "label.doc";

	public static String label_contact = "label.contact";

	public static String label_homepage = "label.homepage";

	public static String label_setopenersoft = "label.setopenersoft";

	public static String label_ie = "label.ie";

	public static String label_word = "label.word";

	public static String label_excel = "label.excel";

	public static String label_ppt = "label.ppt";

	public static String label_pdf = "label.pdf";

	public static String label_txt = "label.txt";

	public static String label_keyword = "label.keyword";

	// #------------------------------------------------------------------
	// #-----------------------buttons------------------------------------
	// #------------------------------------------------------------------
	public static String button_select = "button.select";

	public static String button_confirm = "button.confirm";

	public static String button_cancel = "button.cancel";

	public static String button_delete = "button.delete";

	public static String button_close = "button.close";

	public static String button_search = "button.search";

	public static String button_viedic = "button.viewdic";

	// #------------------------------------------------------------------
	// #-----------------------frame title--------------------------------
	// #------------------------------------------------------------------
	public static String title_createnew = "title.createnew";

	public static String title_aboutsoft = "title.aboutsoft";

	public static String title_modifysearch = "title.modifysearch";

	public static String title_setopenersoft = "title.setopenersoft";

	public static String title_adddic = "title.adddic";

	public static String title_viewdic = "title.viewdic";

	public static String file_chooser_selectdicpath = "file.chooser.selectdicpath";

	public static String dialog_title_setopenersoft = "dialog.title.setopenersoft";

	public static String file_filter_description = "file.filter.description";

	public static String dialog_title_selectpath = "dialog.title.selectpath";

	public static String dialog_title_selectindexpath = "dialog.title.selectindexpath";

	public static String file_chooser_setsearchpath = "file.chooser.setsearchpath";

	public static String file_chooser_setindexpath = "file.chooser.setindexpath";

	// #------------------------------------------------------------------
	// #-----------------------searchApp menu-----------------------------
	// #------------------------------------------------------------------
	public static String searchapp_title = "searchapp.title";

	public static String searchapp_localsearch = "searchapp.localsearch";

	public static String searchapp_creatingindex = "searchapp.creatingindex";

	public static String searchapp_updatingindex = "searchapp.updatingindex";

	public static String searchapp_deletingindex = "searchapp.deletingindex";

	public static String menu_createdsearch = "menu.createdsearch";

	public static String menu_createsearch = "menu.createsearch";

	public static String menu_customdic = "menu.customdic";

	public static String menu_option = "menu.option";

	public static String menu_about = "menu.about";

	public static String menu_help = "menu.help";

	public static String menu_exit = "menu.exit";

	// #------------------------------------------------------------------
	// #-----------------------checkobx-----------------------------
	// #------------------------------------------------------------------
	public static String checkbox_iscoverdic = "checkbox.iscoverdic";

	// #------------------------------------------------------------------
	// #-----------------------messagessearch-----------------------------
	// #------------------------------------------------------------------
	public static String message_search = "message.search";

	// #------------------------------------------------------------------
	// #--- frame error --------------------------------------------------
	// #------------------------------------------------------------------
	public static String errors_outofversion = "errors.outofversion";

	public static String errors_hasrepeat = "errors.hasrepeat";

	public static String errors_confirmdelete = "errors.confirmdelete";

	public static String errors_confirmimportfile = "errors.confirmimportfile";

	public static String errors_notnull = "errors.notnull";

	public static String errors_hascharacter = "errors.hascharacter";

	public static String errors_isallnums = "errors.isallnums";

	public static String errors_isnotdicfile = "errors.isnotdicfile";

	public static String errors_dicnotexisted = "errors.dicnotexisted";

	public static String errors_lincese = "errors.lincese";

	// #------------------------------------------------------------------
	// #--- frame info --------------------------------------------------
	// #------------------------------------------------------------------
	public static String info_nodic = "info.nodic";

	public static String info_completeindex = "info.completeindex";

	public static String info_updatedindex = "info.updatedindex";

	public static String info_deletedindex = "info.deletedindex";

	public static String info_currentedindex = "info.currentedindex";

	// #------------------------------------------------------------------
	// #--- about frame content ----------------------------------------
	// #------------------------------------------------------------------
	public static String about_frame_r1 = "about.frame.r1";

	public static String about_frame_r2 = "about.frame.r2";

	public static String about_frame_r3 = "about.frame.r3";

	public static String about_frame_r4 = "about.frame.r4";

	public static String about_frame_r5 = "about.frame.r5";

	public static String about_frame_r6 = "about.frame.r6";

	public static String about_frame_r7 = "about.frame.r7";

	public static String about_frame_r8 = "about.frame.r8";

	// #------------------------------------------------------------------
	// #--- 配置路径-------------------------------------------------------
	// #------------------------------------------------------------------
	 public static String path =
	 "tomcat\\webapps\\slfile\\WEB-INF\\classes\\";

	public static String lib_path = "tomcat\\webapps\\slfile\\WEB-INF\\lib\\";

	public static String suffixname = ".csv";

	public static String jcomdll = "jcom.dll";

	public static String libmysqldll = "libmysql.dll";

	// public static String path = "src\\";

	public static String dicpath = "dic\\";

	public static String filedatapath = "data/file";

	public static String worddatapath = "data/word";

	public static String exceldatapath = "data/excel";

	public static String pptdatapath = "data/ppt";

	public static String pdfdatapath = "data/pdf";

	public static String txtdatapath = "data/txt";

	public static String htmldatapath = "data/html";

	public static String chmdatapath = "data/chm";

	public static String diccompilepath = "dic\\.compiled";
	
	public static String clientPath = "client";

	public static String confpath = "tomcat\\webapps\\slfile\\WEB-INF\\conf\\";

    // public static String confpath = "conf\\";

	public static String icospath = "icos/search";

	public static String titleicopath = "icos//titleico.jpg";

	public static String licencepath = "licence.dat";

	// #------------------------------------------------------------------
	// #--- 语言相关配置----------------------------------------------------
	// #------------------------------------------------------------------
	public static String LANGUAGE = "language";

	public static String LANGUAGE_RS = path + "com\\searchlocal\\properties\\language.properties";

	public static String CHINESE_RS = path
			+ "com\\searchlocal\\properties\\ApplicationResources_cn.properties";

	public static String ENGLISH_RS = path
			+ "com\\searchlocal\\properties\\ApplicationResources_en.properties";

	public static String JPANESE_RS = path
			+ "com\\searchlocal\\properties\\ApplicationResources_jp.properties";

	public static final String HAS_ERROR = "1";

	public static final String NO_ERROR = "0";
	

	// #------------------------------------------------------------------
	// #--- 数据库相关配置参数-----------------------------------------------
	// #------------------------------------------------------------------
	public static class DBConstansInfo {

		/** DATABASE_CLASS_FOR_NAME */
		public final static String DATABASE_CLASS_FOR_NAME = "driverClassName";

		/** DATABASE_URL */
		public final static String DATABASE_URL = "url";

		/** DATABASE_USER */
		public final static String DATABASE_USER = "username";

		/** DATABASE_PASSWORD */
		public final static String DATABASE_PASSWORD = "password";

		protected DBConstansInfo() {
		}
	}

	// #------------------------------------------------------------------
	// #--- 后缀名文件分类--------------------------------------------------
	// #------------------------------------------------------------------
	public static class FileClassify {

		public static final String XLS = ".xls";

		public static final String XLS2007 = ".xlsx";

		public static final String DOC = ".doc";

		public static final String DOC2007 = ".docx";

		public static final String PDF = ".pdf";

		public static final String PPT = ".ppt";

		public static final String PPT2007 = ".pptx";

		public static final String TXT = ".txt";

		public static final String CHM = ".chm";

		public static final String HTM = ".HTM";

		public static final String HTML = ".HTML";

		protected FileClassify() {
		}

		public static boolean isHtmlcontain(String suffixName) {
			List<String> allType = new ArrayList<String>();
			allType.add(HTM);
			allType.add(HTML);
			allType.add(HTML.toLowerCase());
			allType.add(HTM.toLowerCase());
			return allType.contains(suffixName);
		}

		public static boolean is2007Doccontain(String suffixName) {
			List<String> allType = new ArrayList<String>();
			allType.add(XLS2007);
			allType.add(DOC2007);
			allType.add(PPT2007);
			allType.add(XLS2007.toUpperCase());
			allType.add(DOC2007.toUpperCase());
			allType.add(PPT2007.toUpperCase());
			return allType.contains(suffixName);
		}
	}

	// #------------------------------------------------------------------
	// #--- 代码文件分类----------------------------------------------------
	// #------------------------------------------------------------------
	public static class SrcClassify {

		public static final String JAVA = ".java";

		public static final String C = ".h";

		public static final String JAVASCRIPT = ".js";

		public static final String RUBY = ".rb";

		public static final String ASPX = ".aspx";

		public static final String ASP = ".asp";

		public static final String JSP = ".jsp";

		public static final String PHP = ".php";

		public static final String XML = ".xml";

		public static final String PROPERTY = ".properities";

		public static final String TXT = ".txt";

		protected SrcClassify() {
		}

		public static boolean iscontain(String suffixName) {
			List<String> allType = new ArrayList<String>();
			allType.add(JAVA);
			allType.add(C);
			allType.add(JAVASCRIPT);
			allType.add(RUBY);
			allType.add(ASPX);
			allType.add(ASP);
			allType.add(JSP);
			allType.add(PHP);
			allType.add(PROPERTY);
			allType.add(TXT);
			return allType.contains(suffixName);
		}
	}

	public static class DbErrClassify {

		public static final String DB_E001 = "DB_E001";

		public static final String DB_E002 = "DB_E002";

		public static final String DB_E003 = "DB_E002";

		public static final String DB_E004 = "DB_E004";

		public static final String DB_E005 = "DB_E005";

		public static final String DB_E006 = "DB_E006";

		protected DbErrClassify() {
		}
	}

	/*
	 * 图标消息区分
	 */
	public static class ToolTipsClassify {

		public static final int TOOLTIPS_LOCALSEARCH = 0;

		public static final int TOOLTIPS_CREATINGINDEX = 1;

		public static final int TOOLTIPS_UPDATINGINDEX = 2;

		public static final int TOOLTIPS_DELETINGINDEX = 3;

		protected ToolTipsClassify() {
		}
	}

	/*
	 * 异常代码区分
	 */
	public static class LogicErrClassify {

		public static final String LG_E001 = "LG_E001";

		public static final String LG_E002 = "LG_E002";

		public static final String LG_E003 = "LG_E002";

		public static final String LG_E004 = "LG_E004";

		public static final String LG_E005 = "LG_E005";

		public static final String LG_E006 = "LG_E006";

		protected LogicErrClassify() {
		}
	}

	/*
	 * 语言区分
	 */
	public static class LanguageClassify {

		public static final String ENGLISH = "english";

		public static final String CHINESE = "chinese";

		public static final String JAPANESE = "japanese";

		protected LanguageClassify() {
		}
	}

	/*
	 * 文件后缀名区分
	 */
	public static class FileNameClassify {

		public static final String EXCEL = "excel";

		public static final String WORD = "word";

		public static final String PDF = "pdf";

		public static final String PPT = "ppt";

		public static final String HTML = "html";

		public static final String TXT = "txt";

		public static final String CHM = "chm";

		protected FileNameClassify() {
		}
	}
	
	/*
	 * 菜单名
	 */
	public static class MenuName {

		// 新建搜索
		public static final String CREATESEARCH = "createsearch";

		// 设定
		public static final String OPTION = "option";

		// 自定义词典
		public static final String ADDDIC = "adddic";

		// 关于
		public static final String ABOUT = "about";
		
		// 帮助
		public static final String HELP = "help";

		// 退出
		public static final String EXIT = "exit";

		protected MenuName() {
		}
	}

	/*
	 * 是否完成操作
	 */
	public static class HasOperFlg {
		
		// 1：操作完毕
		public static final String HAS_OPER = "1";

		// 0： 待操作
		public static final String HAS_NOT_OPER = "0";

		protected HasOperFlg() {
		}
	}
	
	/*
	 * 是否已建索引
	 */
	public static class HasCreateIndexFlg {
		
		// 1：已建立索引
		public static final String HAS_CREAT_INDEX = "1";

		// 0： 待建立索引
		public static final String HAS_NO_CREAT_INDEX = "0";

		protected HasCreateIndexFlg() {
		}
	}

	/*
	 * 是否已更新
	 */
	public static class HasUpdateFlg {
		
		// 1：已更新
		public static final String HAS_UPATE = "1";

		// 0：待更新
		public static final String HAS_NOT_UPATE = "0";

		protected HasUpdateFlg() {
		}
	}
	
	/*
	 * 是否已删除
	 */
	public static class HasDelFlg {
		
		// 1：已删除
		public static final String HAS_DEL = "1";

		// 0：未删除
		public static final String HAS_NOT_DEL = "0";

		protected HasDelFlg() {
		}
	}
	
	/*
	 * 文件区分标志位
	 */
	public static class OperFlag {

		// 1： 新增
		public static final String NEED_ADD = "1";

		// 2：修改
		public static final String NEED_UPDATE = "2";

		// 3：删除
		public static final String NEED_DEL = "3";

		protected OperFlag() {
		}
	}

	/*
	 * 高于各个搜索对象的共同信息
	 */
	public static class CommonInfo {

		// 数据库名
		public static final String DATABASE = "COMMONINFO";
		
		// 表名
		public static final String SEARCHER = "searcher";


		protected CommonInfo() {
		}
	}
	
	/* 字典文件 */
	public static final String DICFILE = "dicfile";
	
	/* 是否覆盖 */
	public static final String ISOVERWRITER="isoverwrite";
	
	
}