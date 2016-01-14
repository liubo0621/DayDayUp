//
//  TimeModel.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#include "TimeScene.h"

Scene *TimeModel::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = TimeModel::create();

    scene->addChild(layer);

    return scene;
}

bool TimeModel::init() {
    if (!NormalModel::init()) {
        return false;
    }

    //时间
    _leftTime = 60;

    _timeLable = Label::createWithTTF("倒计时", "jiancuyuan.ttf", 150);
//    _timeLable->setColor(Color3B(8, 61, 90));
    _timeLable->enableShadow();
    _timeLable->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height * 0.65));
    this->addChild(_timeLable, 3);

    return true;
}

void TimeModel::onEnter() {
    Layer::onEnter();

    _firstTouch = true;

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event) {
        if (!_ballBody->isGravityEnabled()) {
            _ballBody->setGravityEnable(true);
        }
        if (_firstTouch) {
            _firstTouch = false;
            _timeLable->setString("60");
            schedule(schedule_selector(TimeModel::reduceTime), 1);  //启动倒计时

            auto moveOut = MoveTo::create(0.2, Vec2(_visibleSize.width / 2, _visibleSize.height + _ready_go->getContentSize().height / 2));
            auto function = CallFunc::create([=] {
                this->removeChild(_ready_go);
                this->removeChild(_tip);
                _scoreLabel->setString("0");
            });

            _ready_go->runAction(Sequence::create(moveOut, function, NULL));
        }
        SimpleAudioEngine::getInstance()->playEffect("flapSFX.mp3");
        _ballBody->setVelocity(Vec2(0, 1000));
        _isMove = false;  //当达到屏幕1/2的位置 再点击屏幕 应该移动障碍物  _isMove 为移动障碍我的判断条件
        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void TimeModel::reduceTime(float delta) {
    _leftTime--;
    _timeLable->setString(StringUtils::format("%d", _leftTime));

    if (_leftTime <= 0) {
        gameOver();
    }
}
