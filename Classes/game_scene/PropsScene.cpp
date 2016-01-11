//
//  PropsScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 16/1/10.
//
//

#include "PropsScene.h"

#define SCALE_SMAL "scale_smal"
#define PROTECTION "protection"
#define INVINCIBLE "invincible"  //无敌

#define BLINK_TIME 1
#define PROP_ACTIVE_TIME 10

Scene* PropsScene::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

void PropsScene::update(float delta) {
    collisionDetection(delta);  //碰撞检测
    moveObstacles(delta);       //移动障碍物
    removeObstacle(delta);      //删除障碍物
    addObstacle(delta);         //添加障碍物
    addProps(delta);            //添加道具
}

void PropsScene::addProps(float delta) {
    auto random = arc4random() % 1 + 1;
    if (_score > 0 && _addApropMapScore != _score && _score % random == 0) {
        Sprite* prop = nullptr;
        auto rand = arc4random() % 2;
        if (rand == 0) {
            prop = Sprite::create("capsule.png");
            prop->setName(SCALE_SMAL);
        } else if (rand == 1) {
            prop = Sprite::create("shield.png");
            prop->setName(PROTECTION);
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

        _addApropMapScore = _score;
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
        for (auto obstacle : _obstacles) {
            //            log("v %f, g %f, v/g=%f",v,g,v/g);
            auto moveBy = MoveBy::create(v / g, Vec2(0, -h));
            auto easeOut = EaseOut::create(moveBy, 2);
            obstacle->runAction(easeOut);
        }

        //移动道具
        for (auto prop : _props) {
            auto moveBy = MoveBy::create(v / g, Vec2(0, -h));
            auto easeOut = EaseOut::create(moveBy, 2);
            prop->runAction(easeOut);
        }

        //移动faster
        if (_bestScoreOnShow) {
            auto moveBy = MoveBy::create(v / g, Vec2(0, -h));
            auto easeOut = EaseOut::create(moveBy, 2);
            _fasterSprite->runAction(easeOut);
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
                auto blink = Blink::create(BLINK_TIME, 3);
                prop->setVisible(false);

                if (prop->getName() == PROTECTION) {
                    //                    prop->
                } else if (prop->getName() == SCALE_SMAL) {
                    auto scaleSmal = ScaleTo::create(0.2, 0.4);
                    auto scaleBack = ScaleTo::create(0.2, 1);
                    _ball->runAction(Sequence::create(scaleSmal, DelayTime::create(PROP_ACTIVE_TIME), blink, scaleBack, NULL));
                }
                prop->stopAllActions();
                _props.eraseObject(prop);
                prop->removeFromParent();
            }
        }
    } else {
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
