package org.cocos2dx.cpp;

import android.R;
import android.os.Handler;
import android.os.Message;



public class JNIHelper {
	private static Handler mHandler;

	public static void init(Handler handler) {
		JNIHelper.mHandler = handler;
	}

	private static void callJavaMethod(int n) {
		System.out.println("-----JNIHelper------"+n);
		Message msg = mHandler.obtainMessage();
		msg.what = n;
		msg.sendToTarget();
	}
	
}
