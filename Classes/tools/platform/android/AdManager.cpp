//
//  AdManager.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/12/27.
//
//

#include "AdManager.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

void AdManager::showAD(int n) {
    JniMethodInfo minfo;
    JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/JNIHelper", "callJavaMethod", "(I)V");
    JniHelper::getEnv()->CallStaticVoidMethod(minfo.classID, minfo.methodID, n);
}

void AdManager::share(const char* path, int score) {
    showAD(score);
    //    JniMethodInfo minfo;
    //    JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/JNIHelper", "share", "(Ljava/lang/String;I)V");
    //    JniHelper::getEnv()->CallStaticVoidMethod(minfo.classID, minfo.methodID, "hah", score);
    //    JniHelper::getEnv()->DeleteLocalRef(minfo.classID);
}
