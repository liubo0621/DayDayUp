//
//  TimeModel.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#ifndef __DayDayUp__TimeModel__
#define __DayDayUp__TimeModel__

#include <stdio.h>
#include "NormalScene.h"
#include "cocos2d.h"

USING_NS_CC;

class TimeModel : public Game{
public:
    static Scene * createScene();
    virtual bool init();
    virtual void onEnter();
    
    void reduceTime(float delta);
    
    virtual void addScore(float delta);
    
    CREATE_FUNC(TimeModel);
    
private:
    int _leftTime;
    Label * _timeLable;
};


#endif /* defined(__DayDayUp__TimeModel__) */
