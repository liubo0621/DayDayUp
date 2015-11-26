//
//  CustomerJniHelper.h
//  DayDayUp
//
//  Created by LIUBO on 15/11/24.
//
//

#ifndef CustomerJniHelper_hpp
#define CustomerJniHelper_hpp

#include <stdio.h>
#include "cocos2d.h"

class CustomerJniHelper : public cocos2d::Ref {
public:
    static CustomerJniHelper* getInstance();
    void callJavaMethod(int n);
};

#endif /* CustomerJniHelper_hpp */
