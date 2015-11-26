//
//  Timer.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/11/21.
//
//

#include "Life.h"
#include "CustomerJniHelper.h"
#include "MobClickCpp.h"

#define MAX_LIFE 15
#define RECOVER_TIME 120

bool Life::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    _isBeginTimer = false;
    _isRecoverTime = false;

    //体力
    _lifeSprite = Sprite::create("life.png");
    _lifeSprite->setAnchorPoint(Vec2(0, 1));
    _lifeSprite->setOpacity(200);
    _lifeSprite->setScale(0.2);
    _lifeSprite->setPosition(5, visibleSize.height - 5);
    this->addChild(_lifeSprite, 20);
    //体力值
    _life = UserDefault::getInstance()->getIntegerForKey("life", MAX_LIFE);
    _lifeLabel = Label::createWithSystemFont(StringUtils::format("%d", _life), "", 200);
    _lifeLabel->setAnchorPoint(Vec2::ZERO);
    _lifeLabel->setPosition(_lifeSprite->getContentSize().width + 2, 2);
    _lifeSprite->addChild(_lifeLabel);

    if (_life < MAX_LIFE && !_isBeginTimer) {
        beginTimer();
    }
    if (_life <= 0) {
        _recoverTime = RECOVER_TIME;
        _isRecoverTime = true;
        schedule(schedule_selector(Life::showCountdown), 1);
        endTimer();
    }

    return true;
}

void Life::beginTimer() {
    this->schedule(schedule_selector(Life::recoverLife), RECOVER_TIME);
    _isBeginTimer = true;
}

void Life::endTimer() {
    unschedule(schedule_selector(Life::recoverLife));
    _isBeginTimer = false;
    UserDefault::getInstance()->setIntegerForKey("life", _life);
}

int Life::getLife() {
    return _life;
}

void Life::recoverLife(float dt) {
    _life++;
    log("life %d", _life);
    _lifeLabel->setString(StringUtils::format("%d", _life));
    UserDefault::getInstance()->setIntegerForKey("life", _life);

    if (_life >= MAX_LIFE) {
        unschedule(schedule_selector(Life::recoverLife));
    }
}

void Life::addLife(int lifeValue) {
    _life += lifeValue;
    log("life %d", _life);
    _lifeLabel->setString(StringUtils::format("%d", _life));
    UserDefault::getInstance()->setIntegerForKey("life", _life);
}

void Life::reduceLife() {
    if (_life > 0) {
        _life--;
        _lifeLabel->setString(StringUtils::format("%d", _life));
        UserDefault::getInstance()->setIntegerForKey("life", _life);

        if (_life < MAX_LIFE && !_isBeginTimer) {
            beginTimer();
        }
    } else {
        if (!_isRecoverTime) {
            _recoverTime = RECOVER_TIME;
            _isRecoverTime = true;
            schedule(schedule_selector(Life::showCountdown), 1);
            endTimer();
        }
    }
}

void Life::showDialogue() {
    _dialogue = DialogueLayer::create();
    addChild(_dialogue);

    _dialogue->setTitle("您已经没有体力了，想快速恢复体力可点击分享或广告", 90);
    _dialogue->setLeftBtnText("分享");
    _dialogue->setRightBtnText("广告");

    _dialogue->leftCallback = [=]() {
        umeng::MobClickCpp::event("click_dialogue", "分享");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto model = UserDefault::getInstance()->getStringForKey("model");
        auto difficuty = UserDefault::getInstance()->getStringForKey("difficulty", "简单");
        std::string model_bestScore = model + difficuty;
        auto bestScore = UserDefault::getInstance()->getIntegerForKey(model_bestScore.c_str(), 0);
        CustomerJniHelper::getInstance()->callJavaMethod(bestScore);
#endif

    };

    _dialogue->rightCallback = []() { umeng::MobClickCpp::event("click_dialogue", "广告"); };
}

void Life::showCountdown(float dt) {
    _lifeLabel->setString(StringUtils::format("倒计时: %ds", _recoverTime--));

    if (_recoverTime < 0) {
        unschedule(schedule_selector(Life::showCountdown));
        _isRecoverTime = false;
        _dialogue->removeFromParent();

        _life++;
        _lifeLabel->setString("1");
        beginTimer();
    }
}
