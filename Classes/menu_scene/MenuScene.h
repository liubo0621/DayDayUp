//
//  MenuScene.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#ifndef __DayDayUp__MenuScene__
#define __DayDayUp__MenuScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <stdio.h>

USING_NS_CC;
using namespace std;
using namespace ui;

enum MenuBtnType { DIFFICULTY, MODEL };

class MenuScene : public Layer {
public:
    virtual bool init();
    static Scene *createScene();

    void addStartBtn();
    void addMenuBtn(Button *parent, const char *text, MenuBtnType type);
    void addWave(Sprite *sender);    //破浪效果
    void addFireWorks(float delta);  //烟花

    ClippingNode *setBtntext(const char *text, Button *btn);

    void showMenuBtn(MenuBtnType type, int startAngle, int rotateAngle, bool isClockwise, int r, int num);
    void dissmissMenuBtn(MenuBtnType type, int num);

    void replaceScene();

    CREATE_FUNC(MenuScene);

protected:
    Size _visibleSize;
    Size _originSize;

    Vector<Button *> _difficultyBtns;
    Vector<Button *> _modelBtns;

    Button *_selectDifficultyBtn;
    Button *_selectModelBtn;
    Button *_startBtn;

    bool _isShow = false;
};

#endif /* defined(__DayDayUp__MenuScene__) */
