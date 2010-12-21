package com.searchlocal.util;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

import com.searchlocal.constants.Constant;

public class MessageUtil {

	public static Properties prop = new Properties();

	/**
	 * @param args
	 */
	public MessageUtil() {
		FileInputStream in;
		try {
			if (prop.isEmpty()) {
				in = new FileInputStream(Constant.path
						+ "com\\searchlocal\\properties\\msg.properties");
				prop.load(in);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static String getMsgbyId(String messid) {
		new MessageUtil();
		String mes = "";
		Object temp = prop.get(messid);
		if (temp != null) {
			mes = (String) temp;
		}
		return mes;
	}

}
