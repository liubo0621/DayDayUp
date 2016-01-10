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
    _leftTime = 10;

    _timeLable = Label::createWithSystemFont(StringUtils::format("时间：%d", _leftTime), "", 60);
    _timeLable->setAnchorPoint(Vec2(1, 1));
    _timeLable->setPosition(_visibleSize.width * 0.99, _visibleSize.height * 0.99);
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
    _timeLable->setString(StringUtils::format("时间：%d", _leftTime));

    if (_leftTime <= 0) {
        gameOver();
    }
}

//计算分数
void TimeModel::addScore(float delta) {
    auto obstacleHeight = _currentSprite->getPosition().y;
    auto ballHeight = _ball->getPosition().y;
    if (ballHeight > obstacleHeight) {
        unschedule(schedule_selector(NormalModel::addScore));
        SimpleAudioEngine::getInstance()->playEffect("point.mp3");
        ++_score;
        _leftTime += 6;

        _scoreLabel->setString(StringUtils::format("%d", _score));

        //计算是否超过最高分 显示faster图片
        auto bestScore = UserDefault::getInstance()->getIntegerForKey("bestScore", 0);
        if (bestScore != 0 && _score > bestScore && !_bestScoreIsShow) {
            _bestScoreIsShow = true;
            _bestScoreOnShow = true;

            _fasterSprite = Sprite::create("faster.png");
            _fasterSprite->setPosition(_visibleSize.width / 2, obstacleHeight);
            this->addChild(_fasterSprite, -1);

            auto show = ScaleTo::create(0.5, 2);
            _fasterSprite->runAction(show);
        }
    }
}
