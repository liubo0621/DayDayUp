//
//  TwinkleScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#ifndef __DayDayUp__TwinkleScene__
#define __DayDayUp__TwinkleScene__

#include <stdio.h>
#include "NormalScene.h"
#include "cocos2d.h"

USING_NS_CC;

class TwinkleModel : public Game{
public:
    static Scene * createScene();
    virtual bool init();
    
    CREATE_FUNC(TwinkleModel);
};


#endif /* defined(__DayDayUp__TwinkleScene__) */
