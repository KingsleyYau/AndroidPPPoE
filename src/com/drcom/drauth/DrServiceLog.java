package com.drcom.drauth;

import java.io.File;
import java.io.FileWriter;
import java.util.Calendar;
import java.util.Date;

import android.content.Context;
import android.util.Log;

/**
 * 应用初始化时要初始化这个类
 * @author zhaojunjie
 *
 */
public class DrServiceLog {
	private static Context mContext;
	
	private static DrServiceLog thisInstance = null;
	public static DrServiceLog getInstance(Context context) {
		if (thisInstance == null) {
			thisInstance = new DrServiceLog(context);
			return thisInstance;
		} else {
			return thisInstance;
		}
	}
	
	private DrServiceLog(Context context){
		mContext = context;
	}
	
	static public void writeLog(String filePath, String logName, String strLog){
		if(mContext != null){
			final String LASTNAME_OF_LOGFILE = ".txt";
			try {
				// 是否有存储卡
		        if(existSDcard())
		        {
		        	// 清除日志
		        	//clearInvalidLog(TIME_FOR_CLEAN_PUSH_LOG);
		        	String strLogToFile = "";
		        	// 系统时间
		            Calendar cal = Calendar.getInstance();
		            int year = cal.get(Calendar.YEAR);
		            int month = cal.get(Calendar.MONTH)+1;
		            int day = cal.get(Calendar.DAY_OF_MONTH);

		            int hour = cal.get(Calendar.HOUR_OF_DAY);
		            int minute = cal.get(Calendar.MINUTE);
		            int second = cal.get(Calendar.SECOND);
		            int milsecond = cal.get(Calendar.MILLISECOND);

		            strLogToFile = "(" + String.valueOf(hour) + ":" + String.valueOf(minute) + ":"
					+ String.valueOf(second) + ":" + String.valueOf(milsecond) + ")";
		            strLogToFile += strLog + "\n";

		            // 文件名
		            String packageName = mContext.getPackageName();
		            String fileName = packageName + "_" + logName + "_" + year +"_"+ month + "_" + day + LASTNAME_OF_LOGFILE;
		            // 文件夹
		            String pathName = filePath;
		            pathName += mContext.getPackageName() + "/";

		            File path = new File(pathName);
		            File file = new File(pathName + fileName);
		            if( !path.exists()) {
		                path.mkdirs();
		            }
		            if( !file.exists()) {
						file.createNewFile();
		            }
		            // 写文件
		            FileWriter writer = new FileWriter(file, true);
		            writer.write(strLogToFile);
		            writer.close();
		        }
			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
    /*
     * 清除旧日志
     * 默认保存10天的日志
     * day: 保存天数
     */
	static public void clearInvalidLog(String filePath, long time) {
		if(mContext != null){
			try {
				// 是否有存储卡
		        if(existSDcard()) {
		        	String pathName = filePath + mContext.getPackageName() + "/";
		        	File logDir = new File(pathName);
		            File[] files = logDir.listFiles();
		            for(File file : files) {
		            	if(file.isFile()) {
		            		long disparity = file.lastModified() + time;
		            		Date dateFile = new Date(disparity);
		            		Date dateCurrent = new Date(System.currentTimeMillis());
		            		// 当前时间减去文件最后修改时间10天
		            		if(dateCurrent.after(dateFile)) {
		            			Log.d("DrPalmPushService.clearInvalidLog", "delete file:" + file.getAbsolutePath());
		            			file.delete();
		            		}
		            	}
		            }
		        }
			}
			catch (Exception e) {
			}
		}
    }
    /*
     * 判断存储卡是否存在
     */
    public static boolean existSDcard()
    {
        if (android.os.Environment.MEDIA_MOUNTED.equals(android.os.Environment.getExternalStorageState()))
        {
            return true;
        }
        else
            return false;
    }
}
