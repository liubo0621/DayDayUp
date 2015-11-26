//
//  CustomerJniHelper.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/11/24.
//
//

#include "CustomerJniHelper.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

static CustomerJniHelper* customerJniHelper = nullptr;

CustomerJniHelper* CustomerJniHelper::getInstance() {
    if (customerJniHelper == nullptr) {
        customerJniHelper = new (std::nothrow) CustomerJniHelper();
        if (customerJniHelper) {
            customerJniHelper->autorelease();
        } else {
            CC_SAFE_DELETE(customerJniHelper);
        }
    }
    return customerJniHelper;
}

void CustomerJniHelper::callJavaMethod(int n) {
    JniMethodInfo minfo;
    JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/JNIHelper", "callJavaMethod", "(I)V");
    JniHelper::getEnv()->CallStaticVoidMethod(minfo.classID, minfo.methodID, n);
}