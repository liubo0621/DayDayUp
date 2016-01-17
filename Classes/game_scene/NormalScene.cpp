//
//  GameLayer.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/7.
//
//

#include "Constants.h"
#include "GameOverLayer.h"
#include "NormalScene.h"
#include "ui/cocosGUI.h"

using namespace ui;

#define OBSTACLE_NUM 11
const Vec2 OFFSET(30, -30);

Scene *NormalModel::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = NormalModel::create();

    scene->addChild(layer);

    return scene;
}

bool NormalModel::init() {
    if (!Layer::init()) {
        return false;
    }

    _visibleSize = Director::getInstance()->getVisibleSize();
    _originSize = Director::getInstance()->getVisibleOrigin();
    _isMove = false;
    _bestScoreIsShow = false;
    _bestScoreOnShow = false;
    _obstacles.clear();
    _rank = 0;

    //清除资源
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_skin_blue.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_skin_green.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_skin_pink.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_style1.plist");
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("game_style2.plist");

    //加载资源
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameOver.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_common.plist");
    //风格
    int style = UserDefault::getInstance()->getIntegerForKey("skin", 1);
    if (style == 1) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_style1.plist");
    } else if (style == 2) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_style2.plist");
    }
    //背景
    auto randomBg = arc4random() % 3;
    const char *bgColor = nullptr;
    if (randomBg == 0) {
        bgColor = "blue";
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_skin_blue.plist");
    } else if (randomBg == 1) {
        bgColor = "green";
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_skin_green.plist");

    } else if (randomBg == 2) {
        bgColor = "pink";
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game_skin_pink.plist");
    }

    std::string difficulty = UserDefault::getInstance()->getStringForKey("difficulty");
    if (difficulty == EASY) {
        _rank = 1;
    } else if (difficulty == MIDDLE) {
        _rank = 2;
    } else {
        _rank = 3;
    }

    //背景
    addBg(StringUtils::format("%s%s", bgColor, "_bg_bottom.jpg").c_str(), _originSize.height, -3);
    addBg("bg_top.png", _originSize.height, -1);
    auto bg = addBg(StringUtils::format("%s%s", bgColor, "_bg_middle.jpg").c_str(), _originSize.height, -2, true);
    bg->getTexture()->setAliasTexParameters();
    auto bg2 = addBg(StringUtils::format("%s%s", bgColor, "_bg_middle.jpg").c_str(), _originSize.height + bg->getContentSize().height, -2, true);
    bg2->getTexture()->setAliasTexParameters();
    auto bg3 = addBg(StringUtils::format("%s%s", bgColor, "_bg_middle.jpg").c_str(), _originSize.height + bg->getContentSize().height * 2, -2, true);
    bg3->getTexture()->setAliasTexParameters();
    //    //颜色
    //    auto random = arc4random() % 5;
    //    //    int random=3;
    //    //    log("bg   %d",random);
    //    switch (random) {
    //        case 0:
    //            bg->setColor(Color3B(160, 32, 240));  //肖贡土色
    //            break;
    //        case 1:
    //            bg->setColor(Color3B(255, 125, 64));  //肉黄
    //            break;
    //        case 2:
    //            bg->setColor(Color3B(255, 153, 18));  //镉黄
    //            break;
    //        case 3:
    //            bg->setColor(Color3B(138, 43, 226));  //紫罗兰
    //            break;
    //        case 4:
    //            bg->setColor(Color3B(255, 99, 71));  //番茄红
    //            break;
    //
    //        default:
    //            break;
    //    }

    //底部地面
    auto bottom = Sprite::createWithSpriteFrameName("bottom.png");
    _obstacles.pushBack(bottom);
    auto bottomBody = PhysicsBody::createEdgeBox(bottom->getContentSize());
    bottomBody->setCategoryBitmask(0x01);
    bottomBody->setContactTestBitmask(0x01);
    bottomBody->setCollisionBitmask(0x01);
    bottomBody->setGroup(0);
    bottom->setPhysicsBody(bottomBody);
    bottom->setPosition(Vec2(_visibleSize.width / 2, bottom->getContentSize().height / 2));
    bottom->setTag(FLOOR);
    this->addChild(bottom);

    //提示 点击
    _tip = Sprite::createWithSpriteFrameName("tip.png");
    _tip->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.3);
    _tip->setScale(2);
    this->addChild(_tip);

    // ready go
    _ready_go = Sprite::createWithSpriteFrameName("ready.png");
    _ready_go->setPosition(_visibleSize.width / 2, _visibleSize.height - _originSize.height + _ready_go->getContentSize().height / 2);
    _ready_go->setScale(2);
    this->addChild(_ready_go, 2);
    //动作
    auto moveTo = MoveTo::create(0.3, Vec2(_visibleSize.width / 2, _visibleSize.height * 0.92));
    auto easeIn = EaseIn::create(moveTo, 3);
    auto moveBy = MoveBy::create(0.2, Vec2(0, 30));
    _ready_go->runAction(Sequence::create(easeIn, moveBy, moveBy->reverse(), NULL));

    //小球
    _ball = Sprite::createWithSpriteFrameName("ball.png");
    _ballBody = PhysicsBody::createCircle(_ball->getContentSize().width / 2 - 10);
    _ballBody->setGravityEnable(false);
    _ballBody->setCategoryBitmask(0x01);
    _ballBody->setContactTestBitmask(0x01);
    _ballBody->setCollisionBitmask(0x01);
    _ballBody->setGroup(0);
    _ball->setPhysicsBody(_ballBody);
    _ball->setPosition(_visibleSize / 2);
    this->addChild(_ball, 10);

    auto ballShadow = Sprite::createWithSpriteFrameName("ball_shadow.png");
    ballShadow->setPosition(_ball->getContentSize().width / 2 + OFFSET.x, _ball->getContentSize().height / 2 + OFFSET.y);
    ballShadow->setName("ball_shadow");
    _ball->addChild(ballShadow, -1);

    //添加障碍物
    randomAddObstacle(_visibleSize.height * 0.85);

    //分数
    _scoreLabel = Label::createWithBMFont("stagelabel.fnt", "");
    _scoreLabel->enableShadow();
    _scoreLabel->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.92);
    this->addChild(_scoreLabel, 2);

    scheduleUpdate();

    //物理引擎碰撞检测
    _contactListener = EventListenerPhysicsContactWithGroup::create(0);
    _contactListener->onContactBegin = CC_CALLBACK_1(NormalModel::onContactBegin, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_contactListener, this);

    //    //暂停 开始
    //    ballVelocit = Vec2(0, 0);
    //    auto pause = Button::create();
    //    pause->loadTextureNormal("pause.png");
    //    pause->setTitleFontSize(50);
    //    pause->setAnchorPoint(Vec2(0, 1));
    //    pause->setPosition(Vec2(15, _visibleSize.height - _originSize.height - 10));
    //    pause->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type) {
    //        if (type == Widget::TouchEventType::ENDED) {
    //            if (Director::getInstance()->isPaused()) {  //开始
    //                pause->loadTextureNormal("pause.png");
    //                Director::getInstance()->resume();
    //                if (!_firstTouch) {
    //                    _ballBody->setVelocity(ballVelocit);
    //                    _ballBody->setGravityEnable(true);
    //                }
    //
    //            } else {  //暂停
    //                ballVelocit.y = _ballBody->getVelocity().y;
    //                pause->loadTextureNormal("resume.png");
    //
    //                _ballBody->setVelocity(Vec2(0, 0));
    //                _ballBody->setGravityEnable(false);
    //
    //                Director::getInstance()->pause();
    //            }
    //        }
    //
    //    });
    //
    //    addChild(pause);

    return true;
}

/**
 *  点击给小球初速度
 */
void NormalModel::onEnter() {
    Layer::onEnter();

    _firstTouch = true;

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event) {
        if (_firstTouch) {
            _firstTouch = false;
            auto moveOut = MoveTo::create(0.2, Vec2(_visibleSize.width / 2, _visibleSize.height + _ready_go->getContentSize().height / 2));
            auto function = CallFunc::create([=] {
                this->removeChild(_ready_go);
                this->removeChild(_tip);
                _scoreLabel->setString("0");
            });

            _ready_go->runAction(Sequence::create(moveOut, function, NULL));
        }
        if (!_ballBody->isGravityEnabled()) {
            _ballBody->setGravityEnable(true);
        }
        SimpleAudioEngine::getInstance()->playEffect("flapSFX.mp3");
        _ballBody->setVelocity(Vec2(0, 1000));
        _isMove = false;  //当达到屏幕1/2的位置 再点击屏幕 应该移动障碍物  _isMove 为移动障碍我的判断条件
        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void NormalModel::update(float delta) {
    collisionDetection(delta);  //碰撞检测
    moveObstacles(delta);       //移动障碍物
    removeObstacle(delta);      //删除障碍物
    addObstacle(delta);         //添加障碍物
    checkBgPosition(delta);
}

Sprite *NormalModel::addBg(const char *bgFileName, float positionY, int zOrder, bool isAddToBgVector) {
    auto bg = Sprite::create(bgFileName);
    bg->setAnchorPoint(Vec2::ZERO);
    bg->setPosition(Vec2(0, positionY));
    addChild(bg, zOrder);
    if (isAddToBgVector) {
        bg->setOpacity(150);
        _bgs.pushBack(bg);
    }

    return bg;
}

//**********************************添加障碍物************************************************

void NormalModel::addObstacle(float delta) {
    auto obstacle = _obstacles.back();
    if (obstacle->getPosition().y <= _visibleSize.height / 2) {
        _currentSprite = obstacle;  //储存当前障碍物，计算分数时用
        schedule(schedule_selector(NormalModel::addScore));
        randomAddObstacle(_visibleSize.height + 200);
    }
}

void NormalModel::randomAddObstacle(float height) {
    int random;  //随机障碍物
    //防止产生重复的障碍物
    do {
        random = arc4random() % OBSTACLE_NUM;
    } while (_currentObstacle != -1 && random == _currentObstacle);
    _currentObstacle = random;

    int randT = arc4random() % 3 - 1;

    //    random = 10;

    //两个长的 顺时针旋转
    if (random == 0) {
        obstacle6(height, 4 + randT);
    }
    //两个长的 一顺一逆旋转 速度不一
    else if (random == 1) {
        obstacle9(height, 4 + randT);
    }

    //两个长棍交叉转
    else if (random == 2) {
        obstacle3(height, 4 + randT);
    }

    //两个长棍成60度角同时转
    else if (random == 3) {
        obstacle4(height, 4 + randT);
    }
    //两个长的 90度
    else if (random == 4) {
        obstacle5(height, 4 + randT);
    }
    //侧滑
    else if (random == 5) {
        obstacle1(height, 1);
    }
    //两个长的一顺一逆旋转
    else if (random == 6) {
        obstacle7(height, 4 + randT);
    }
    //两个长的，旋转速度不一
    else if (random == 7) {
        obstacle8(height, 6);
    }
    //小圆 一个扇页
    else if (random == 8) {
        obstacle10(height, 6);
    }
    //大圆 两个短扇页
    else if (random == 9) {
        obstacle11(height, 6);
    }
    //圆角矩形 两个长扇页
    else if (random == 10) {
        obstacle12(height, 4 + randT);
    }
}

/**
 *  添加侧滑障碍物
 *
 *  @param spriteName 障碍物file
 *  @param position   位置
 *
 *  @return 障碍物
 */
Sprite *NormalModel::addSlideObstacle(std::string spriteName, Vec2 position) {
    auto obstacle = Sprite::createWithSpriteFrameName(spriteName);
    //储存到障碍物集合中
    _obstacles.pushBack(obstacle);
    obstacle->setPosition(position);
    this->addChild(obstacle);

    auto body = PhysicsBody::createEdgeBox(obstacle->getContentSize());
    body->setRotationEnable(true);

    body->setCategoryBitmask(0x01);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(-1);
    body->setDynamic(true);
    body->setGravityEnable(false);

    _obstaclesBody.pushBack(body);
    obstacle->setPhysicsBody(body);

    return obstacle;
}

/**
 *  添加 桩
 *
 *  @param spriteName  精灵名
 *  @param anchorPoint 锚点
 *  @param position    位置
 *
 *  @return 返回精灵
 */
Sprite *NormalModel::addStack(std::string spriteName, Vec2 anchorPoint, Vec2 position) {
    auto obstacle = Sprite::createWithSpriteFrameName(spriteName);
    //储存到障碍物集合中
    _obstacles.pushBack(obstacle);
    obstacle->setAnchorPoint(anchorPoint);
    obstacle->setPosition(position);
    this->addChild(obstacle);

    //旋转桩 难度  困难
    if (_rank >= 2) {
        int i = arc4random() % 3 - 1;  // 正负1  或者是0  控制旋转方向
        auto randomT = arc4random() % 3;
        obstacle->setRotation(60 * i);

        auto rotateBack = RotateBy::create(2 + randomT, 120 * i * (-1));
        auto repeatRotate = RepeatForever::create(Sequence::create(rotateBack, rotateBack->clone()->reverse(), NULL));
        obstacle->runAction(repeatRotate);
    }

    return obstacle;
}

/**
 *  添加桩上的障碍物
 *
 *  @param spriteName  精灵名
 *  @param stack 父节点
 *  @param position    位置
 *
 *  @return 返回精灵
 */
Sprite *NormalModel::addObstacle(std::string spriteName, Sprite *stack, cocos2d::Vec2 position) {
    auto obstacle = Sprite::createWithSpriteFrameName(spriteName);
    stack->addChild(obstacle);
    obstacle->setPosition(position);

    auto body = PhysicsBody::createEdgeBox(obstacle->getContentSize());
    body->setRotationEnable(true);

    body->setCategoryBitmask(0x01);
    body->setContactTestBitmask(0x01);
    body->setCollisionBitmask(0x01);
    body->setGroup(-1);
    body->setDynamic(true);
    body->setGravityEnable(false);

    _obstaclesBody.pushBack(body);
    obstacle->setPhysicsBody(body);

    //闪烁 障碍物 难度 超难
    if (_rank == 3) {
        auto isFade = arc4random() % 5;
        if (!isFade) {
            auto fadeOut = FadeOut::create(2);
            auto fadeIn = FadeIn::create(2);

            auto delay = DelayTime::create(2);
            auto repeat = RepeatForever::create(Sequence::create(fadeOut, delay, fadeIn, delay, NULL));
            obstacle->runAction(repeat);
        }
    }

    return obstacle;
}

/**
 *  添加轨道
 *
 *  @param fileName 轨道图片名
 *  @param height   添加高度
 *
 *  @return 轨道精灵
 */
Sprite *NormalModel::addRail(const char *fileName, double height) {
    auto rail = Sprite::createWithSpriteFrameName(fileName);
    double x = 0;
    if (arc4random() % 2) {
        x = _visibleSize.width * 0.6;
    } else {
        x = _visibleSize.width * 0.4;
    }
    rail->setPosition(x, height);
    _obstacles.pushBack(rail);
    addChild(rail);
    return rail;
}

//*******************具体障碍物************************
//两侧滑动
void NormalModel::obstacle1(double height, float t) {
    auto delay = DelayTime::create(0.2);
    //左边  270为obstacle.png宽的一半
    auto leftObstacle = addSlideObstacle("obstacle.png", Vec2(270, height));
    //添加动作
    auto leftMove = MoveBy::create(t, Vec2(-leftObstacle->getContentSize().width / 2, 0));
    auto leftMoveBack = leftMove->reverse();
    auto leftMoveRepeat = RepeatForever::create(Sequence::create(leftMove, leftMoveBack, delay, NULL));
    leftObstacle->runAction(leftMoveRepeat);

    //右边
    auto rightObstacle = addSlideObstacle("obstacle.png", Vec2(_visibleSize.width - 270, height));
    //添加动作
    auto rightMove = MoveBy::create(t, Vec2(rightObstacle->getContentSize().width / 2, 0));
    auto rightMoveBack = rightMove->reverse();
    auto rightMoveRepeat = RepeatForever::create(Sequence::create(rightMove, rightMoveBack, delay, NULL));
    rightObstacle->runAction(rightMoveRepeat);

    leftObstacle->setTag(SIDESLIPPING_OBSTACLE);
    rightObstacle->setTag(SIDESLIPPING_OBSTACLE);
}

//两个长的 交叉
void NormalModel::obstacle3(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    //左
    auto leftObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    auto leftObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    //右
    auto rightObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    auto rightObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, 360));

    leftObstacleRotationLong1->runAction(rotation);
    leftObstacleRotationLong2->runAction(rotation->reverse());

    rightObstacleRotationLong1->runAction(rotation->clone()->reverse());
    rightObstacleRotationLong2->runAction(rotation->clone());

    leftObstacleRotationLong1->setTag(LONG_OBSTACLE);
    leftObstacleRotationLong2->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong1->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong2->setTag(LONG_OBSTACLE);
}

//两个长的，60度
void NormalModel::obstacle4(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    //左
    auto leftObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong1->setRotation(30);
    auto leftObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong2->setRotation(-30);
    //右
    auto rightObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong1->setRotation(30);
    auto rightObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong2->setRotation(-30);
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, -360));

    leftObstacleRotationLong1->runAction(rotation);
    leftObstacleRotationLong2->runAction(rotation->clone());

    rightObstacleRotationLong1->runAction(rotation->clone()->reverse());
    rightObstacleRotationLong2->runAction(rotation->clone()->reverse());

    leftObstacleRotationLong1->setTag(LONG_OBSTACLE);
    leftObstacleRotationLong2->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong1->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong2->setTag(LONG_OBSTACLE);
}

//两个长的 90度
void NormalModel::obstacle5(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    //左
    auto leftObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong1->setRotation(45);
    auto leftObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong2->setRotation(-45);
    //右
    auto rightObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong1->setRotation(45);
    auto rightObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong2->setRotation(-45);
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, -360));

    leftObstacleRotationLong1->runAction(rotation);
    leftObstacleRotationLong2->runAction(rotation->clone());

    rightObstacleRotationLong1->runAction(rotation->clone()->reverse());
    rightObstacleRotationLong2->runAction(rotation->clone()->reverse());

    leftObstacleRotationLong1->setTag(LONG_OBSTACLE);
    leftObstacleRotationLong2->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong1->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong2->setTag(LONG_OBSTACLE);
}

//两个长的 顺时针旋转
void NormalModel::obstacle6(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    auto leftObstacleRotationLong = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    auto rightObstacleRotationLong = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    //    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, 360));
    leftObstacleRotationLong->runAction(rotation);
    rightObstacleRotationLong->runAction(rotation->clone()->reverse());

    leftObstacleRotationLong->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong->setTag(LONG_OBSTACLE);
}

//两个长的 一顺一逆旋转
void NormalModel::obstacle7(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    auto leftObstacleRotationLong = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    auto rightObstacleRotationLong = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, 360));
    leftObstacleRotationLong->runAction(rotation);
    rightObstacleRotationLong->runAction(rotation->clone());

    leftObstacleRotationLong->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong->setTag(LONG_OBSTACLE);
}

//两个长的，60度 旋转速度不一
void NormalModel::obstacle8(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    //左
    auto leftObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong1->setRotation(30);
    auto leftObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    leftObstacleRotationLong2->setRotation(-30);
    //右
    auto rightObstacleRotationLong1 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong1->setRotation(30);
    auto rightObstacleRotationLong2 = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    rightObstacleRotationLong2->setRotation(-30);
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, -360));
    auto rotation2 = RepeatForever::create(RotateBy::create(t + 2, -360));

    leftObstacleRotationLong1->runAction(rotation);
    leftObstacleRotationLong2->runAction(rotation2);

    rightObstacleRotationLong1->runAction(rotation->clone()->reverse());
    rightObstacleRotationLong2->runAction(rotation2->clone()->reverse());

    leftObstacleRotationLong1->setTag(LONG_OBSTACLE);
    leftObstacleRotationLong2->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong1->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong2->setTag(LONG_OBSTACLE);
}

//两个长的 一顺一逆旋转 速度不一
void NormalModel::obstacle9(double height, float t) {
    //两根桩
    auto leftObstacle = addStack("obstacle_bar.png", Vec2(0.3, 0.5), Vec2(0, height));
    auto rightObstacle = addStack("obstacle_bar.png", Vec2(0.7, 0.5), Vec2(_visibleSize.width, height));
    leftObstacle->setTag(STACK);
    rightObstacle->setTag(STACK);
    //旋转精灵
    auto leftObstacleRotationLong = addObstacle("obstacle_rotation_long.png", leftObstacle, Vec2(leftObstacle->getContentSize().width - 15, leftObstacle->getContentSize().height / 2));
    auto rightObstacleRotationLong = addObstacle("obstacle_rotation_long.png", rightObstacle, Vec2(15, rightObstacle->getContentSize().height / 2));
    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, 360));
    auto rotation2 = RepeatForever::create(RotateBy::create(t + 3, 360));

    leftObstacleRotationLong->runAction(rotation);
    rightObstacleRotationLong->runAction(rotation2);

    leftObstacleRotationLong->setTag(LONG_OBSTACLE);
    rightObstacleRotationLong->setTag(LONG_OBSTACLE);
}

//小圆 一个扇页
void NormalModel::obstacle10(double height, float t) {
    auto smalRail = addRail("rail_small.png", height);
    auto obstacleRotationLong = addObstacle("obstacle_rotation_long.png", smalRail, Vec2(smalRail->getContentSize().width / 2, smalRail->getContentSize().height - 5));

    int i = 1;
    if (smalRail->getPositionX() < _visibleSize.width / 2) {
        i = -1;
    }
    auto rotation = RepeatForever::create(RotateBy::create(t, 360 * i));
    obstacleRotationLong->runAction(rotation);
    obstacleRotationLong->setTag(LONG_OBSTACLE);

    smalRail->runAction(rotation->clone());
}
//大圆 两个短扇页
void NormalModel::obstacle11(double height, float t) {
    auto bigRail = addRail("rail_big.png", height);
    auto leftObstacleTotationSort = addObstacle("obstacle_rotation_short.png", bigRail, Vec2(bigRail->getContentSize().width / 2, bigRail->getContentSize().height - 5));
    auto rightObstacleTotationSort = addObstacle("obstacle_rotation_short.png", bigRail, Vec2(bigRail->getContentSize().width / 2, bigRail->getContentSize().height - 5));
    rightObstacleTotationSort->setRotation(90);

    //旋转
    auto rotation = RepeatForever::create(RotateBy::create(t, 360));

    bigRail->runAction(rotation->clone());
    leftObstacleTotationSort->runAction(rotation);
    rightObstacleTotationSort->runAction(rotation->clone());

    leftObstacleTotationSort->setTag(LONG_OBSTACLE);
    rightObstacleTotationSort->setTag(LONG_OBSTACLE);
}
//圆角矩形 两个长扇页
void NormalModel::obstacle12(double height, float t) {
    auto rectRail = addRail("rail_rect.png", height);
    rectRail->setPosition(_visibleSize.width / 2, height);

    //圆角矩形的长 宽 半径
    auto rectRailWidth = rectRail->getContentSize().width;
    auto rectRailHeight = rectRail->getContentSize().height;
    auto r = rectRailHeight / 2;
    int rectWidth = 9;

    //顺时针 圆角矩形一周
    //上边中间点
    auto p0 = Vec2(rectRailWidth / 2, rectRailHeight - rectWidth);
    //右半弧
    auto p1 = Vec2(rectRailWidth - r, rectRailHeight - rectWidth);
    auto p2 = Vec2(rectRailWidth - rectWidth, rectRailHeight / 2);
    auto p3 = Vec2(rectRailWidth - r, rectWidth);
    //下边中间点
    auto p4 = Vec2(rectRailWidth / 2, rectWidth);
    //左半弧
    auto p5 = Vec2(r, rectWidth);
    auto p6 = Vec2(rectWidth, rectRailHeight / 2);
    auto p7 = Vec2(r, rectRailHeight - rectWidth);

    auto obstaclePosition = p0;
    auto leftObstacleTotationLong = addObstacle("obstacle_rotation_long.png", rectRail, obstaclePosition);
    auto rightObstacleTotationLong = addObstacle("obstacle_rotation_long.png", rectRail, obstaclePosition);
    rightObstacleTotationLong->setRotation(90);

    auto array = PointArray::create(20);
    array->addControlPoint(p0);
    array->addControlPoint(p1);
    array->addControlPoint(p2);
    array->addControlPoint(p3);
    array->addControlPoint(p4);
    array->addControlPoint(p5);
    array->addControlPoint(p6);
    array->addControlPoint(p7);
    array->addControlPoint(p0);

    //    作用：创建一个样条曲线轨迹的动作
    //    参数1：完成轨迹所需的时间
    //    参数2：控制点的坐标数组
    //    拟合度  其值= 0 路径最柔和
    auto moveAction = CardinalSplineTo::create(10, array, 0);
    auto rotation = RotateBy::create(t, 360);

    auto repeatMove = RepeatForever::create(moveAction);
    auto repeatRotation = RepeatForever::create(rotation);

    leftObstacleTotationLong->runAction(repeatMove);
    leftObstacleTotationLong->runAction(repeatRotation);
    rightObstacleTotationLong->runAction(repeatMove->clone());
    rightObstacleTotationLong->runAction(repeatRotation->clone());
}

//**********************************************************************************

//移动障碍物
void NormalModel::moveObstacles(float delta) {
    //如果球的高度到大于屏幕中心位置，则障碍物下移
    if (_ball->getPosition().y > _visibleSize.height / 2 && !_isMove) {
        _isMove = true;

        //球到中点的速度v 重力g 球运动的高度h
        auto v = _ballBody->getVelocity().y;
        auto g = Director::getInstance()->getRunningScene()->getPhysicsWorld()->getGravity().y * -1;
        //    h=v^2/2g
        auto h = (v * v) / (2 * g);
        //设置球为静态  不让他的位置超过屏幕的一半
        _ballBody->setGravityEnable(false);
        _ballBody->setVelocity(Vec2(0, 0));

        //障碍物下移
        // log("v %f, g %f, v/g=%f",v,g,v/g);
        auto moveBy = MoveBy::create(v / g, Vec2(0, -h));
        auto easeOut = EaseOut::create(moveBy, 2);
        for (auto obstacle : _obstacles) {
            obstacle->runAction(easeOut->clone());
        }

        //移动faster
        if (_bestScoreOnShow) {
            _fasterSprite->runAction(easeOut->clone());
        }

        //移动背景
        for (auto bg : _bgs) {
            bg->runAction(easeOut->clone());
        }

        //障碍物移动完，设置小球受重力
        auto delay = DelayTime::create(v / g);
        auto func = CallFunc::create([=]() { _ballBody->setGravityEnable(true); });
        this->runAction(Sequence::create(delay, func, NULL));
    }
}

//移除障碍物
void NormalModel::removeObstacle(float delta) {
    for (auto obstacle : _obstacles) {
        if (obstacle->getPosition().y < -_visibleSize.height / 2) {
            this->removeChild(obstacle);
            _obstacles.eraseObject(obstacle);
        }
    }
    // faster
    if (_bestScoreOnShow && _fasterSprite->getPosition().y < -20) {
        this->removeChild(_fasterSprite);
        _bestScoreOnShow = false;
    }
}

void NormalModel::checkBgPosition(float delta) {
    // bg
    for (auto bg : _bgs) {
        if (bg->getPositionY() < _originSize.height - bg->getContentSize().height) {
            bg->setPositionY(bg->getPositionY() + bg->getContentSize().height * _bgs.size());
        }
    }
}

//碰撞检测
void NormalModel::collisionDetection(float delta) {
    if (_ball->getPosition().y < _ball->getContentSize().height / 2) {
        gameOver();
    }
}

bool NormalModel::onContactBegin(PhysicsContact &contact) {
    log("onContactbegin....");
    gameOver();
    return false;
}

//计算分数
void NormalModel::addScore(float delta) {
    auto obstacleHeight = _currentSprite->getPosition().y;
    auto ballHeight = _ball->getPosition().y;
    if (ballHeight > obstacleHeight) {
        unschedule(schedule_selector(NormalModel::addScore));
        SimpleAudioEngine::getInstance()->playEffect("point.mp3");
        ++_score;
        _scoreLabel->setString(StringUtils::format("%d", _score));

        //计算是否超过最高分 显示faster图片
        auto model = UserDefault::getInstance()->getStringForKey("model");
        auto difficuty = UserDefault::getInstance()->getStringForKey("difficulty", "简单");
        std::string model_bestScore = model + difficuty;
        auto bestScore = UserDefault::getInstance()->getIntegerForKey(model_bestScore.c_str(), 0);

        if (bestScore != 0 && _score >= bestScore && !_bestScoreIsShow) {
            _bestScoreIsShow = true;
            _bestScoreOnShow = true;

            _fasterSprite = Sprite::createWithSpriteFrameName("faster.png");
            _fasterSprite->setPosition(_visibleSize.width / 2, obstacleHeight);
            this->addChild(_fasterSprite, -1);

            auto show = ScaleTo::create(0.5, 2);
            _fasterSprite->runAction(show);
        }
    }
}

void NormalModel::gameOver() {
    this->unscheduleAllSelectors();  //停止所有定时器

    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

    SimpleAudioEngine::getInstance()->playEffect("die1.mp3");
    auto blood = ParticleSystemQuad::create("bloodSpurts.plist");
    blood->setTotalParticles(700);
    blood->setAutoRemoveOnFinish(true);  //自动删除
    blood->setLifeVar(3);
    blood->setStartSize(15);
    blood->setPosition(_ball->getPosition());
    this->addChild(blood);

    _ball->stopAllActions();
    _ball->setVisible(false);
    _ballBody->removeFromWorld();

    UserDefault::getInstance()->setIntegerForKey("score", _score);

    //创建结束层
    auto func = CallFunc::create([=] {
        auto gameoverLayer = GameOver::create();
        this->getScene()->addChild(gameoverLayer);

        gameoverLayer->setPosition(0, _visibleSize.height);
        auto moveTo = MoveTo::create(0.2, Vec2(0, 0));
        gameoverLayer->runAction(moveTo);
    });
    this->runAction(Sequence::create(DelayTime::create(0.1), func, NULL));
}
