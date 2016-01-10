//
//  DarkScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#ifndef __DayDayUp__DarkScene__
#define __DayDayUp__DarkScene__

#include <stdio.h>
#include "NormalScene.h"
#include "cocos2d.h"

USING_NS_CC;

class DarkModel : public NormalModel{
public:
    static Scene * createScene();
    virtual bool init();
    
    CREATE_FUNC(DarkModel);
};


#endif /* defined(__DayDayUp__DarkScene__) */
