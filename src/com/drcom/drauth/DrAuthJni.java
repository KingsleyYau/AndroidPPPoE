/**
 * DrPPOEJni拨号接口
 * @author Kingsley Yau
 * date: 2014年2月14日
 * Email: Kingsleyyau@gmail.com
 */
package com.drcom.drauth;

public class DrAuthJni {
	private static final String LIBRARY_NAME = "drauth";
	static{
		try{
			System.loadLibrary(LIBRARY_NAME);
		}catch( Exception e){
			e.printStackTrace();
		}

	}
	
	/*
	 * 获取应用root权限
	 */
	public native boolean IsRoot();
	public native boolean ReleaseDrSniffer(String sdcardPath);
	public native String SystemComandExcute(String command);
	public native String SystemComandExcuteWithRoot(String command);
	
	/*
	 * 获取网络接口和状态
	 */
	public native String[] GetDevice();
	public native String[] GetMacAddress();
	public native String[] GetIPAddress();
	
	
	
	/*
	 * LoginPPOE 登录
	 * ret:0:成功 
	 */
	public native int LoginPPOE(String username, String passowrd);
	/*
	 * LogoutPPOE 注销
	 * ret:0:成功
	 * 
	 */
	public native int LogoutPPOE();
	/*
	 * StatusPPOE 是否在线
	 * ret:true:在线 false:失败
	 */
	public native boolean StatusPPOE();
	
}
