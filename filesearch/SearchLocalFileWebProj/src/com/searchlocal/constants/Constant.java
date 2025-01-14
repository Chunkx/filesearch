package com.web.searchlocal.constants;

import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;

public class Constant {
	
	/** WEB_INDEX_JSP */
	public static final String WEB_INDEX_JSP = "/web/index.jsp";
	
	/** WEB_RESULT_JSP */
	public static final String WEB_RESULT_JSP = "/web/result.jsp";
	
	/** WEB_RESULT_JSP */
	public static final String VIEW_SWF_JSP = "/web/viewswf.jsp";
	
	public static String path = "";
	static {
		try {
			path = Constant.class.getResource("/").toURI().getPath();
		} catch (URISyntaxException e) {
			// TODO 注意消除资源(关闭I/O等)
			e.printStackTrace();
		}
		// path = path + "tomcat\\webapps\\slfile\\WEB-INF\\classes\\";
	}
	
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
	
	/*
	 * 各个搜索对象的共同信息
	 */
	public static class CommonInfo {

		// 数据库名
		public static final String DATABASE = "COMMONINFO";

		// 表名
		public static final String SEARCHER = "searcher";

		protected CommonInfo() {
		}
	}


	public static String LANGUAGE = "language";

	public static String CHINESE_RS = path
			+ "com\\web\\searchlocal\\properties\\ApplicationResources_cn.properties";

	public static String ENGLISH_RS = path
			+ "com\\web\\searchlocal\\properties\\ApplicationResources_en.properties";

	public static String JPANESE_RS = path
			+ "com\\web\\searchlocal\\properties\\ApplicationResources_jp.properties";

	//	
	public static String confpath = "conf\\";

	/** IS_NO_SEARCH */
	public static final String is_no_search = "IS_NO_SEARCH";

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
	
	public static String web_recentchangfile = "web.recentchangfile";
	
	
	// #------------------------------------------------------------------
	// #--- web error----------------------------------------------------------
	// #------------------------------------------------------------------
	
	public static String web_msg_list = "msglist";
	
	public static String web_info_loading = "web.info.loading";
	
	public static String web_error_nosearcher = "web.error.nosearcher";
	
	public static String web_error_noresult = "web.error.noresult";
	
	public static String web_error_nosetopener = "web.error.nosetopener";

	public static class LanguageClassify {

		public static final String ENGLISH = "english";

		public static final String CHINESE = "chinese";

		public static final String JAPANESE = "japanese";

		protected LanguageClassify() {
		}
	}

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
		
		public static final String SQL = ".sql";

		public static final String ASP = ".asp";
		
		public static final String CONF = ".conf";
		
		public static final String INI = ".ini";

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
			allType.add(SQL);
			allType.add(ASP);
			allType.add(CONF);
			allType.add(INI);
			allType.add(JSP);
			allType.add(PHP);
			allType.add(PROPERTY);
			allType.add(TXT);
			return allType.contains(suffixName);
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
		
		public static List<String> getAllTypeList() {
			List<String> allType = new ArrayList<String>();
			// TODO CHM暂时不做
			allType.add(EXCEL);
			allType.add(WORD);
			allType.add(PDF);
			allType.add(PPT);
			allType.add(HTML);
			allType.add(TXT);
			return allType;
		}
		
		public static String getAllTypes() {
			StringBuffer bf = new StringBuffer();
			bf.append(EXCEL);
			bf.append(",");
			bf.append(WORD);
			bf.append(",");
			bf.append(PDF);
			bf.append(",");
			bf.append(PPT);
			bf.append(",");
			bf.append(HTML);
			bf.append(",");
			bf.append(TXT);
			return bf.toString();
		}
	}

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

	/**
	 * 消息分类
	 */
	public static class MsgCode {

		public static final String MSG = "M";
		
		public static final String WARN = "W";

		public static final String ERR = "E";

		protected MsgCode() {
		}
	}
	
	/**
	 * 
	 */
	public static class PageAction {

		public static final String TO_PREVIOUS = "previous";

		public static final String TO_NEXT = "next";

		protected PageAction() {
		}
	}
	
	/*
	 * 最近变化文件的相关数据
	 */
	public static class RecentFileInfo {

		// 最近文件索引保存目录
		public static final String Id = "-1";

		// 最近文件索引保存目录
		public static final String INDEX_PATH = "recent_files_index";

		// 索引文件Key
		public static final String INDEX_PATH_KEY = "indexpath";

		protected RecentFileInfo() {
		}
	}
	
	public static String ALL = "all";
}