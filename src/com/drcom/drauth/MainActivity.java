/**
 * DrPPOEJni拨号测试
 * @author Kingsley Yau
 * date: 2014年2月14日
 * Email: Kingsleyyau@gmail.com
 */
package com.drcom.drauth;

import java.io.DataOutputStream;
import java.io.IOException;

import com.drcom.ppoe.R;

import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	private DrAuthJni jni = new DrAuthJni();
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
//        String[] filelist = null;
//        Class cls;
//        AssetManager am = this.getAssets();
//        am.open(fileName)
                
//        for(String filepath : filelist) {
//        	Log.d("MainActivity", filepath);
//        }
        
        final TextView macAddressTextView = (TextView)this.findViewById(R.id.textView);
        Button button1 = (Button)this.findViewById(R.id.button1);
        Button button2 = (Button)this.findViewById(R.id.button2);
        Button button3 = (Button)this.findViewById(R.id.button3);
        Button button4 = (Button)this.findViewById(R.id.button4);
        
        String textViewValue = "";
        
        String[] deviceList = jni.GetDevice();
        for(String device : deviceList) {
        	textViewValue += device;
        	textViewValue += ";";
        }
        textViewValue += "\n";
        
        String[] macAddresses = jni.GetMacAddress();
        for(String macAddress : macAddresses) {
        	textViewValue += macAddress;
        	textViewValue += ";";
        }
        textViewValue += "\n";
        
        String[] ipAddresses = jni.GetIPAddress();
        for(String ipAddress : ipAddresses) {
        	textViewValue += ipAddress;
        	textViewValue += ";";
        }
        textViewValue += "\n\n";
        macAddressTextView.setText(textViewValue);

    	String path = Environment.getExternalStorageDirectory().getAbsolutePath();
		path += "/";
		jni.ReleaseDrSniffer(path);
		
//        if(!jni.IsRoot()) {
//        	String path = Environment.getExternalStorageDirectory().getAbsolutePath();
//    		path += "/drsinffer";
////    		path = "/data/data/com.drcom.ppoe/lib/drsinffer";
//    		jni.ReleaseDrSniffer(path);
//    		
//        }
//        if(jni.IsRoot()) {
//        	// already root access
//        	Log.d("MainActivity", "Already root access");
//        }
//        else {
//        	// get root access
//        	String path = Environment.getExternalStorageDirectory().getAbsolutePath();
//        	path += "/drsu";
//			boolean ret = jni.RootMyself(path);
//        	Log.d("MainActivity", "RootMyself:" + ret + " path:" + path);
//			
//        	DataOutputStream output = null;
//        	try {
//        		Process process = null;
//        		process = Runtime.getRuntime().exec("/system/bin/su");
//        		output = new DataOutputStream(
//					process.getOutputStream());
//        		output.writeBytes("chown root:root /system/bin/drsu && chmod 4755 /system/bin/drsu\n");
//        		output.writeBytes("exit\n");
//        		output.flush();
//        		output.close();
//        		Log.d("MainActivity", "Process ok");
//        	}catch (IOException e) {
//        		e.printStackTrace();
//        	}
//        }
        
        button1.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				new Thread() {
					@Override
					public void run(){
						int ret = jni.LoginPPOE("aa1", "123");
						Log.d("MainActivity", "LoginPPOE:" + ret);
					};
				}.start();
			}
		});
        
        button2.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				new Thread() {
					@Override
					public void run(){
						int ret = jni.LogoutPPOE();
						Log.d("MainActivity", "LogoutPPOE:" + ret);
					};
				}.start();
			}
		});

        button3.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				boolean ret = jni.StatusPPOE();
				Log.d("MainActivity", "StatusPPOE:" + ret);
				
				String[] deviceList = jni.GetDevice();
				String textViewValue = "";
				for(String device : deviceList) {
		        	textViewValue += device;
		        	textViewValue += ";";
		        }
				textViewValue += "\n";
				String[] macAddresses = jni.GetMacAddress();
		        for(String macAddress : macAddresses) {
		        	textViewValue += macAddress;
		        	textViewValue += ";";
		        }
		        textViewValue += "\n";
				String[] ipAddresses = jni.GetIPAddress();
		        for(String ipAddress : ipAddresses) {
		        	textViewValue += ipAddress;
		        	textViewValue += ";";
		        }
		        textViewValue += "\n";
				macAddressTextView.setText(textViewValue);
			}
		});
        
        button4.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String textViewValue = jni.SystemComandExcute("ls -l /system/bin/drsu");
				macAddressTextView.setText(textViewValue);
			}
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    class CmdThread extends Thread {
    	public void run() {
    		try {
				Thread.sleep(60000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    		String command = "ifconfig";
    		jni.SystemComandExcute(command);
    	}
    }
}
