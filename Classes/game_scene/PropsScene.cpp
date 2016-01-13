//
//  PropsScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 16/1/10.
//
//

#include "PropsScene.h"
#include <cmath>

#define SCALE_SMAL "scale_smal"
#define PROTECTION "protection"
#define INVINCIBLE "invincible"  //无敌

#define PROTECTION_TAG 11
#define BLINK_TIME 1
#define PROP_ACTIVE_TIME 10
#define ADD_PROP_RANDOM_MAX 8
#define ADD_PROP_RANDOM_MIN 3

Scene* PropsScene::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = PropsScene::create();

    scene->addChild(layer);

    return scene;
}

bool PropsScene::init() {
    if (!NormalModel::init()) {
        return false;
    }

    return true;
}

// a <= n < b
int PropsScene::random_a_b(int min, int max) {
    return arc4random() % (max - min) + min;
}

void PropsScene::update(float delta) {
    collisionDetection(delta);  //碰撞检测
    moveObstacles(delta);       //移动障碍物
    removeObstacle(delta);      //删除障碍物
    addObstacle(delta);         //添加障碍物
    checkBgPosition(delta);
    addProps(delta);  //添加道具
}

//间隔random 个障碍物 添加一个道具
void PropsScene::addProps(float delta) {
    if (_score > 0 && _addApropMapScore != _score) {
        _addApropMapScore = _score;

        if (_random == -1) {
            _random = random_a_b(ADD_PROP_RANDOM_MIN, ADD_PROP_RANDOM_MAX);
        }

        if (_score % _random > 0) {
            return;
        }
        _random = -1;

        Sprite* prop = nullptr;
        auto rand = arc4random() % 2;
        if (rand == 0) {
            prop = Sprite::create("capsule.png");
            prop->setName(SCALE_SMAL);
        } else if (rand == 1) {
            if (_isAddPropected) {
                return;
            }
            prop = Sprite::create("shield.png");
            prop->setName(PROTECTION);
            _isAddPropected = true;
        }

        prop->setPosition(_visibleSize.width / 2, _visibleSize.height * 1.2);
        _props.pushBack(prop);
        delayRemveProps(prop);

        auto physicsBody = PhysicsBody::createEdgeBox(prop->getContentSize());
        physicsBody->setCategoryBitmask(0x01);
        physicsBody->setContactTestBitmask(0x01);
        physicsBody->setCollisionBitmask(0x01);
        physicsBody->setGroup(1);

        prop->setPhysicsBody(physicsBody);

        addChild(prop);
    }
}

//移动障碍物
void PropsScene::moveObstacles(float delta) {
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

        //移动道具
        for (auto prop : _props) {
            prop->runAction(easeOut->clone());
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

bool PropsScene::onContactBegin(PhysicsContact& contact) {
    log("onContactbegin....");
    auto body = contact.getShapeB()->getBody();  //道具 group ＝ 1
    if (body->getGroup() == 1) {
        for (Sprite* prop : _props) {
            if (prop->getPhysicsBody() == body) {
                prop->setVisible(false);

                //保护罩道具
                if (prop->getName() == PROTECTION) {
                    _isPropected = true;
                    //添加保护罩
                    auto protection = Sprite::create("shield.png");
                    protection->setPosition(_ball->getContentSize() / 2);
                    protection->setTag(PROTECTION_TAG);
                    _ball->addChild(protection);

                }
                //变小道具
                else if (prop->getName() == SCALE_SMAL) {
                    if (_isScaleSmal) {
                        _ball->stopAction(_scaleAction);
                    }
                    auto blink = Blink::create(BLINK_TIME, 3);
                    auto scaleSmal = ScaleTo::create(0.2, 0.4, 0.4);
                    auto scaleBack = ScaleTo::create(0.2, 1, 1);
                    _scaleAction = Sequence::create(scaleSmal, DelayTime::create(PROP_ACTIVE_TIME), blink, scaleBack, CallFunc::create([=] { _isScaleSmal = false; }), NULL);
                    _isScaleSmal = true;
                    _ball->runAction(_scaleAction);
                }

                //删除道具图片
                prop->stopAllActions();
                _props.eraseObject(prop);
                prop->removeFromParent();
            }
        }
    }
    //去掉保护罩功能
    else if (_isPropected) {
        if (!_isAttached) {
            _isAttached = true;
            auto blink = Blink::create(BLINK_TIME, 3);
            _ball->runAction(Sequence::create(blink, CallFunc::create([=] {
                                                  _ball->removeChildByTag(PROTECTION_TAG);
                                                  _isPropected = false;
                                                  _isAddPropected = false;
                                                  _isAttached = false;
                                              }),
                                              NULL));
        }

    } else if (!_isPropected) {
        gameOver();
    }

    return false;
}

void PropsScene::delayRemveProps(cocos2d::Sprite* prop) {
    auto blink = Blink::create(BLINK_TIME, 3);
    prop->runAction(Sequence::create(DelayTime::create(PROP_ACTIVE_TIME), blink, CallFunc::create([=] {
                                         prop->stopAllActions();
                                         _props.eraseObject(prop);
                                         prop->removeFromParent();

                                     }),
                                     NULL));
}
