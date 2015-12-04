//
//  GameOverLayer.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#include "GameOverLayer.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "NormalScene.h"
#include "ElasticityScene.h"  //弹力模式
#include "TimeScene.h"
#include "TwinkleScene.h"  //闪烁模式
#include "DarkScene.h"
#include "OppositeGravityScene.h"
#include "MobClickCpp.h"
#include "CustomerJniHelper.h"

#include <iostream>

using namespace ui;
using namespace std;

bool GameOver::init() {
    if (!Layer::init()) {
        return false;
    }

//广告
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (arc4random() % 2 == 0) {
        CustomerJniHelper::getInstance()->callJavaMethod(-2);  //插屏
    }
#endif

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = Sprite::create("gameoverBg.png");
    bg->setScale(visibleSize.width, visibleSize.height);
    bg->setOpacity(95);
    this->addChild(bg);

    //游戏结束
    auto gameover = Sprite::create("gameover.png");
    gameover->setPosition(visibleSize.width / 2, visibleSize.height * 0.9);
    this->addChild(gameover);

    // board(承载分数 模式 最佳)
    auto scoreBoard = Sprite::create("scoreBoard.png");
    scoreBoard->setPosition(visibleSize.width / 2, visibleSize.height * 0.7);
    this->addChild(scoreBoard);

    //模式
    auto model = UserDefault::getInstance()->getStringForKey("model");
    auto difficuty = UserDefault::getInstance()->getStringForKey("difficulty", "简单");

    auto type = Label::createWithSystemFont(model, "", 40);
    type->setColor(Color3B(0, 0, 0));
    scoreBoard->addChild(type);
    type->setPosition(scoreBoard->getContentSize().width * 0.27, scoreBoard->getContentSize().height * 0.18);

    //分数
    string model_bestScore = model + difficuty;
    auto score = UserDefault::getInstance()->getIntegerForKey("score", 0);
    auto bestScore = UserDefault::getInstance()->getIntegerForKey(model_bestScore.c_str(), 0);
    log("%s", model_bestScore.c_str());

    //分数label
    auto scoreLabel = Label::createWithSystemFont(StringUtils::format("%d", score), "", 70);
    scoreLabel->setColor(Color3B(0, 0, 0));
    scoreBoard->addChild(scoreLabel);
    scoreLabel->setPosition(scoreBoard->getContentSize().width * 0.27, scoreBoard->getContentSize().height * 0.64);

    //新纪录
    if (score > bestScore) {
        auto newBest = Sprite::create("new.png");
        scoreBoard->addChild(newBest);
        newBest->setPosition(Vec2(scoreBoard->getContentSize().width * 0.6, scoreBoard->getContentSize().height * 0.64));

        UserDefault::getInstance()->setIntegerForKey(model_bestScore.c_str(), score);
        bestScore = score;
    }

    //最佳
    auto bestScoreLabel = Label::createWithSystemFont(StringUtils::format("%d", bestScore), "arial.ttf", 70);
    bestScoreLabel->setColor(Color3B(0, 0, 0));
    scoreBoard->addChild(bestScoreLabel);
    bestScoreLabel->setPosition(scoreBoard->getContentSize().width * 0.695, scoreBoard->getContentSize().height * 0.64);

    //难度
    auto difficutyLable = Label::createWithSystemFont(difficuty, "", 40);
    difficutyLable->setColor(Color3B(0, 0, 0));
    scoreBoard->addChild(difficutyLable);
    difficutyLable->setPosition(scoreBoard->getContentSize().width * 0.695, scoreBoard->getContentSize().height * 0.18);

    // umeng 数据统计
    const char* gameModel = "";
    if (model == "正常模式") {
        gameModel = "normal";
    } else if (model == "弹力模式") {
        gameModel = "elasticity";
    } else if (model == "时间模式") {
        gameModel = "time";
    } else if (model == "闪烁模式") {
        gameModel = "blink";
    } else if (model == "暗黑模式") {
        gameModel = "dark";
    } else {
        gameModel = "opposite_gravity";
    }

    umeng::eventDict dict;
    dict["score"] = StringUtils::format("%d", score);
    dict["best_score"] = StringUtils::format("%d", bestScore);
    dict["difficuty"] = difficuty;
    umeng::MobClickCpp::event(gameModel, &dict);

    //按钮
    auto shareBtn = Button::create("gameoverbtn.png");
    shareBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4));
    shareBtn->setScale(1.5);
    shareBtn->setTitleText("炫耀");
    shareBtn->setTitleFontSize(40);
    this->addChild(shareBtn);

    auto restarBtn = Button::create("gameoverbtn.png");
    restarBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3));
    restarBtn->setScale(1.5);
    restarBtn->setTitleText("再试一次");
    restarBtn->setTitleFontSize(40);
    this->addChild(restarBtn);

    auto gohomeBtn = Button::create("gameoverbtn.png");
    gohomeBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.2));
    gohomeBtn->setScale(1.5);
    gohomeBtn->setTitleText("返回首页");
    gohomeBtn->setTitleFontSize(40);
    this->addChild(gohomeBtn);

    //添加按钮点击事件
    shareBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            log("share...");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CustomerJniHelper::getInstance()->callJavaMethod(bestScore);
#endif
        }
    });

    restarBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (model == "正常模式") {
                Director::getInstance()->replaceScene(Game::createScene());
            }
            if (model == "弹力模式") {
                Director::getInstance()->replaceScene(ElasticityModel::createScene());
            }
            if (model == "时间模式") {
                Director::getInstance()->replaceScene(TimeModel::createScene());
            }
            if (model == "闪烁模式") {
                Director::getInstance()->replaceScene(TwinkleModel::createScene());
            }
            if (model == "暗黑模式") {
                Director::getInstance()->replaceScene(DarkModel::createScene());
            }
            if (model == "反重力模式") {
                Director::getInstance()->replaceScene(OppositeGravityModel::createScene());
            }
        }
    });

    gohomeBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            log("离开%s", model.c_str());
            umeng::MobClickCpp::endLogPageView(gameModel);
            Director::getInstance()->replaceScene(MenuScene::createScene());
        }
    });

    return true;
}