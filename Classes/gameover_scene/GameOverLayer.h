//
//  GameOverLayer.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#ifndef __DayDayUp__GameOverLayer__
#define __DayDayUp__GameOverLayer__

#include "cocos2d.h"

USING_NS_CC;

class GameOver : public Layer {
public:
    virtual bool init();
    const char* screenShoot();

    CREATE_FUNC(GameOver);

private:
    Size visibleSize;
};
#endif /* defined(__DayDayUp__GameOverLayer__) */
