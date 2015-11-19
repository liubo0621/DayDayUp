//
//  SettingScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/9/3.
//
//

#include "SettingScene.h"
#include "ui/CocosGui.h"
#include "SimpleAudioEngine.h"
#include "MobClickCpp.h"

using namespace ui;
using namespace CocosDenshion;

bool Setting::init() {
    if (!Layer::init()) {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();

    //***************音效*******************
    auto effect = Sprite::create("menu_yinxiao.png");
    //    auto effect=Label::createWithSystemFont("音效", "", 60);
    effect->setPosition(visibleSize.width * 0.28, visibleSize.height * 0.5);
    effect->setScale(1.2);
    this->addChild(effect);

    //音效滑动条
    auto effectSlider = Slider::create();
    effectSlider->loadBarTexture("menu_effect_bg.png");
    effectSlider->loadSlidBallTextures("sliderThumb.png");
    effectSlider->loadProgressBarTexture("menu_effect.png");
    effectSlider->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height * 0.5));
    this->addChild(effectSlider);

    //获取之前设置的音效值
    auto effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent", 100);

    effectSlider->setPercent(effectPercent);

    effectSlider->addEventListener([=](Ref* sender, Slider::EventType type) {
        if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
            int percent = effectSlider->getPercent();
            SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent) / 100);

            UserDefault::getInstance()->setFloatForKey("effectPercent", percent);
        }
    });

    //*******************难度***************
    auto difficulty = Sprite::create("menu_difficulty.png");
    difficulty->setScale(1.2);
    difficulty->setPosition(visibleSize.width * 0.28, visibleSize.height * 0.4);
    this->addChild(difficulty);

    //难度滑动条
    auto difficultySlider = Slider::create();
    difficultySlider->loadBarTexture("menu_deffect.png");
    difficultySlider->loadSlidBallTextures("menu_difficult_sellect.png");
    difficultySlider->loadProgressBarTexture("");
    difficultySlider->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height * 0.4));
    this->addChild(difficultySlider);

    //获取难度等级
    rank = UserDefault::getInstance()->getIntegerForKey("rank", 1);
    switch (rank) {
        case 1: {
            difficultySlider->setPercent(17);
            break;
        }
        case 2: {
            difficultySlider->setPercent(50);
            break;
        }
        case 3: {
            difficultySlider->setPercent(83);
            break;
        }
        default:
            break;
    }

    difficultySlider->addEventListener([=](Ref* sender, Slider::EventType type) {
        if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
            auto percent = difficultySlider->getPercent();
            if (percent < 33) {
                rank = 1;
                //                 difficultySlider->setPercent(17);
            }
            if (33 <= percent && percent < 67) {
                rank = 2;
                //                 difficultySlider->setPercent(50);
            }
            if (67 <= percent && percent <= 100) {
                rank = 3;
                //                difficultySlider->setPercent(83);
            }
        }

    });

    difficultySlider->addTouchEventListener([=](cocos2d::Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (rank == 1) {
                difficultySlider->setPercent(17);
                UserDefault::getInstance()->setStringForKey("difficulty", "简单");
            }
            if (rank == 2) {
                difficultySlider->setPercent(50);
                UserDefault::getInstance()->setStringForKey("difficulty", "一般");
            }
            if (rank == 3) {
                difficultySlider->setPercent(83);
                UserDefault::getInstance()->setStringForKey("difficulty", "困难");
            }

            UserDefault::getInstance()->setIntegerForKey("rank", rank);
        }
    });

    //*******************退出***************
    auto exitBtn = Button::create("menu_exit_btn.png");
    exitBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.12));
    this->addChild(exitBtn);

    auto exit = Sprite::create("menu_exit.png");
    exitBtn->addChild(exit);
    exit->setPosition(exitBtn->getContentSize() / 2);

    //事件
    exitBtn->addTouchEventListener(CC_CALLBACK_2(Setting::exit, this));

    return true;
}

void Setting::exit(cocos2d::Ref* pSender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
        return;
#endif
        umeng::MobClickCpp::end();
        Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ::exit(0);
#endif
    }
}
