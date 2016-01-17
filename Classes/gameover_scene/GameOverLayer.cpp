//
//  GameOverLayer.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#include "AdManagerProtocol.h"
#include "Constants.h"
#include "DarkScene.h"
#include "ElasticityScene.h"  //弹力模式
#include "GameOverLayer.h"
#include "MenuScene.h"
#include "MobClickCpp.h"
#include "NormalScene.h"
#include "OppositeGravityScene.h"
#include "PropsScene.h"
#include "TimeScene.h"
#include "TwinkleScene.h"  //闪烁模式
#include "ui/CocosGUI.h"
#include <string.h>

#include <iostream>

using namespace ui;
using namespace std;

bool GameOver::init() {
    if (!Layer::init()) {
        return false;
    }

    //广告
    if (arc4random() % 5 == 0) {
        AdManagerProtocol::getInstance()->showAD(-2);  //插屏
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B::BLACK);
    bg->setOpacity(95);
    this->addChild(bg);

    //游戏结束
    auto gameover = Sprite::createWithSpriteFrameName("gameover.png");
    gameover->setPosition(visibleSize.width / 2, visibleSize.height * 0.9);
    this->addChild(gameover);

    // board(承载分数 模式 最佳)
    auto scoreBoard = Sprite::createWithSpriteFrameName("scoreBoard.png");
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
        auto newBest = Sprite::createWithSpriteFrameName("new.png");
        scoreBoard->addChild(newBest);
        newBest->setPosition(Vec2(scoreBoard->getContentSize().width * 0.6, scoreBoard->getContentSize().height * 0.64));

        UserDefault::getInstance()->setIntegerForKey(model_bestScore.c_str(), score);
        bestScore = score;
    }

    //最佳
    auto bestScoreLabel = Label::createWithSystemFont(StringUtils::format("%d", bestScore), "", 70);
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
    if (model == NORMAL_MODEL) {
        gameModel = "normal";
    } else if (model == ELASTICITY_MODEL) {
        gameModel = "elasticity";
    } else if (model == TIME_MODEL) {
        gameModel = "time";
    } else if (model == TWINKLE_MODEL) {
        gameModel = "blink";
    } else if (model == DARK_MODEL) {
        gameModel = "dark";
    } else if (model == OPPOSITE_GRAVITY_DMODEL) {
        gameModel = "opposite_gravity";
    } else if (model == PROPS_MODEL) {
        gameModel = "props";
    }

    umeng::eventDict dict;
    dict["score"] = StringUtils::format("%s, %d分", difficuty.c_str(), score);
    dict["best_score"] = StringUtils::format("%s, %d分", difficuty.c_str(), bestScore);
    dict["difficuty"] = difficuty;
    umeng::MobClickCpp::event(gameModel, &dict);

    //按钮
    auto shareBtn = Button::create("gameoverbtn.png", "", "", TextureResType::PLIST);
    shareBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.4));
    shareBtn->setScale(1.5);
    shareBtn->setTitleText("炫耀");
    shareBtn->setTitleFontSize(40);
    this->addChild(shareBtn);

    auto restarBtn = Button::create("gameoverbtn.png", "", "", TextureResType::PLIST);
    restarBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3));
    restarBtn->setScale(1.5);
    restarBtn->setTitleText("再试一次");
    restarBtn->setTitleFontSize(40);
    this->addChild(restarBtn);

    auto gohomeBtn = Button::create("gameoverbtn.png", "", "", TextureResType::PLIST);
    gohomeBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.2));
    gohomeBtn->setScale(1.5);
    gohomeBtn->setTitleText("返回首页");
    gohomeBtn->setTitleFontSize(40);
    this->addChild(gohomeBtn);

    //添加按钮点击事件
    shareBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            log("share...");
            const char* path = screenShoot();
            auto imgFile = StringUtils::format("%scannotstop.jpg", path);
            log("imgPath %s", imgFile.c_str());
            AdManagerProtocol::getInstance()->share(imgFile.c_str(), score);
        }
    });

    restarBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            Scene* nextScene = nullptr;
            if (model == NORMAL_MODEL) {
                nextScene = NormalModel::createScene();
            } else if (model == ELASTICITY_MODEL) {
                nextScene = ElasticityModel::createScene();
            } else if (model == TIME_MODEL) {
                nextScene = TimeModel::createScene();
            } else if (model == TWINKLE_MODEL) {
                nextScene = TwinkleModel::createScene();
            } else if (model == DARK_MODEL) {
                nextScene = DarkModel::createScene();
            } else if (model == OPPOSITE_GRAVITY_DMODEL) {
                nextScene = OppositeGravityModel::createScene();
            } else if (model == PROPS_MODEL) {
                nextScene = PropsScene::createScene();
            }
            Director::getInstance()->replaceScene(nextScene);
        }
    });

    gohomeBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            log("离开%s", model.c_str());
            auto reScene = TransitionMoveInL::create(0.2, MenuScene::createScene());
            Director::getInstance()->replaceScene(reScene);
        }
    });

    return true;
}

const char* GameOver::screenShoot() {
    //定义一个屏幕大小的渲染纹理
    RenderTexture* pScreen = RenderTexture::create(visibleSize.width, visibleSize.height, kCCTexture2DPixelFormat_RGBA8888);
    //获得当前的场景指针
    Scene* pCurScene = Director::getInstance()->getRunningScene();
    //渲染纹理开始捕捉
    pScreen->begin();
    //当前场景参与绘制
    pCurScene->visit();
    //结束捕捉
    pScreen->end();
    //保存为png
    pScreen->saveToFile("cannotstop.png", kCCImageFormatPNG);  //图片质量不好
    //保存为jpg
    //    pScreen->saveToFile("cannotstop.jpg", kCCImageFormatJPEG);

    return FileUtils::getInstance()->getWritablePath().c_str();
}