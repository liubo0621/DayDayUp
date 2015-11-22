//
//  DialogueLayer.h
//  DayDayUp
//
//  Created by LIUBO on 15/11/21.
//
//

#ifndef DialogueLayer_hpp
#define DialogueLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

typedef std::function<void()> LeftCallback;
typedef std::function<void()> RightCallback;

class DialogueLayer : public Layer {
public:
    static DialogueLayer* create(const char* dialogueBg = "dialogue.png", const char* leftImg = "left.png", const char* rightImg = "right.png");
    virtual bool init(const char* dialogueBg, const char* leftImg, const char* rightImg);

    void setTitle(const char* title, int size = 120);
    void setLeftBtnText(const char* text);
    void setRightBtnText(const char* text);

public:
    LeftCallback leftCallback;
    RightCallback rightCallback;

private:
    Sprite* _dialogue;
    Button* _leftBtn;
    Button* _rightBtn;
};

#endif /* DialogueLayer_hpp */
