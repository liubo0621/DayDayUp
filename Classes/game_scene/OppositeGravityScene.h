//
//  OppositeGravityScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/28.
//
//

#ifndef __DayDayUp__OppositeGravityScene__
#define __DayDayUp__OppositeGravityScene__

#include <stdio.h>
#include "NormalScene.h"
#include "cocos2d.h"

USING_NS_CC;

class OppositeGravityModel : public NormalModel{
public:
    static Scene * createScene();
    virtual bool init();
    virtual void onEnter();
    //移动障碍物
    virtual void moveObstacles(float delta);

    
    CREATE_FUNC(OppositeGravityModel);
    
private:
    bool _onTouch;
    int _moveStep; //障碍物向下移动的距离
};



#endif /* defined(__DayDayUp__OppositeGravityScene__) */
