//
//  PropsScene.h
//  DayDayUp
//
//  Created by LIUBO on 16/1/10.
//
//

#ifndef PropsScene_h
#define PropsScene_h

#include "NormalScene.h"
#include <stdio.h>
class PropsScene : public NormalModel {
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PropsScene);
};

#endif /* PropsScene_h */
