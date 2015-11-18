//
//  SecondGame.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//弹力模式

#ifndef __DayDayUp__SecondGame__
#define __DayDayUp__SecondGame__

#include <stdio.h>
#include "NormalScene.h"
#include "cocos2d.h"

USING_NS_CC;

class ElasticityModel: public Game{
public:
    static Scene * createScene();
    virtual bool init();
    
    CREATE_FUNC(ElasticityModel);
};


#endif /* defined(__DayDayUp__SecondGame__) */
