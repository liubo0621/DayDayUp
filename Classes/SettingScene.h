//
//  SettingScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/9/3.
//
//

#ifndef __DayDayUp__SettingScene__
#define __DayDayUp__SettingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "MenuScene.h"

USING_NS_CC;

class Setting : public Layer{
public:
    virtual bool init();
    void exit(Ref* pSender,Widget::TouchEventType type);
    
    CREATE_FUNC(Setting);
    
protected:
    Size visibleSize;
    int rank;
};

#endif /* defined(__DayDayUp__SettingScene__) */
