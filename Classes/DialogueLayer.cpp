//
//  DialogueLayer.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/11/21.
//
//

#include "DialogueLayer.h"

DialogueLayer *DialogueLayer::create(const char *dialogueBg, const char *leftImg, const char *rightImg) {
    DialogueLayer *ret = new (std::nothrow) DialogueLayer();
    if (ret && ret->init(dialogueBg, leftImg, rightImg)) {
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool DialogueLayer::init(const char *dialogueBg, const char *leftImg, const char *rightImg) {
    if (!Layer::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B::BLACK);
    bg->setOpacity(100);
    addChild(bg);

    //屏蔽层
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch *touch, Event *event) { return true; };
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _dialogue = Sprite::create(dialogueBg);
    _dialogue->setPosition(visibleSize / 2);
    _dialogue->setScale(0.7);
    addChild(_dialogue, 100);

    _leftBtn = Button::create(leftImg);
    _dialogue->addChild(_leftBtn);
    _leftBtn->setAnchorPoint(Vec2::ZERO);
    _leftBtn->setPosition(Vec2(_dialogue->getContentSize().width * 0.15, _dialogue->getContentSize().height * 0.12));

    _rightBtn = Button::create(rightImg);
    _dialogue->addChild(_rightBtn);
    _rightBtn->setAnchorPoint(Vec2(1, 0));
    _rightBtn->setPosition(Vec2(_dialogue->getContentSize().width * 0.85, _dialogue->getContentSize().height * 0.12));

    _leftBtn->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (leftCallback != nullptr) {
                leftCallback();
            }
        }
    });
    _rightBtn->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (rightCallback != nullptr) {
                rightCallback();
            }
        }
    });

    return true;
}

void DialogueLayer::setTitle(const char *title, int size) {
    auto label = Label::create();
    label->setString(title);
    label->setSystemFontSize(size);
    label->setDimensions(_dialogue->getContentSize().width * 0.9, _dialogue->getContentSize().height * 0.6);
    //    label->setColor(Color3B::RED);
    //    label->setHorizontalAlignment(TextHAlignment::CENTER);  //水平
    label->setVerticalAlignment(TextVAlignment::CENTER);  //垂直
    label->setContentSize(_dialogue->getContentSize());
    _dialogue->addChild(label);
    label->setPosition(_dialogue->getContentSize().width / 2, _dialogue->getContentSize().height * 0.6);
}

void DialogueLayer::setLeftBtnText(const char *text) {
    _leftBtn->setTitleText(text);
    _leftBtn->setTitleFontSize(80);
}
void DialogueLayer::setRightBtnText(const char *text) {
    _rightBtn->setTitleText(text);
    _rightBtn->setTitleFontSize(80);
}