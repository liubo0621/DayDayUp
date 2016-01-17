//
//  TwinkleScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#include "TwinkleScene.h"

Scene* TwinkleModel::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = TwinkleModel::create();

    scene->addChild(layer);

    return scene;
}

bool TwinkleModel::init() {
    if (!NormalModel::init()) {
        return false;
    }

    //闪烁
    auto fadeOut = FadeOut::create(2);
    auto fadeIn = FadeIn::create(2);

    auto delay = DelayTime::create(3);
    auto repeat = RepeatForever::create(Sequence::create(fadeOut, delay, fadeIn, delay, NULL));
    _ball->runAction(repeat);
    _ball->getChildByName("ball_shadow")->runAction(repeat->clone());

    return true;
}