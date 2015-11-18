//
//  MenuScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#ifndef __DayDayUp__MenuScene__
#define __DayDayUp__MenuScene__

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace ui;

class MenuScene:public Layer{
public:
    virtual bool init();
    static Scene *createScene();
    
    void addBtn(Vec2 position,string btnName,string textName);
    void addWave(Sprite * sender);//破浪效果
    void addFireWorks(float delta);//烟花
    void moveBack();
    
    CREATE_FUNC(MenuScene);
    
protected:
    Size visibleSize;
    bool _isShowSetting;
    
    Sprite * _settingBorder;
    
    Vector<Button *> _buttons;
    Button * _settingBtn;
    Button * goHomeBtn;
    Layer * settingLayer;
    
    int _rank;
    string _model;
};

#endif /* defined(__DayDayUp__MenuScene__) */
