//
//  DarkScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//
//

#include "DarkScene.h"

Scene* DarkModel::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = DarkModel::create();

    scene->addChild(layer);

    return scene;
}

bool DarkModel::init() {
    if (!NormalModel::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    //黑布
    auto dark = Sprite::create("bg.png");
    dark->setColor(Color3B::BLACK);
    dark->setPosition(visibleSize / 2);
    dark->setOpacity(0);
    this->addChild(dark, 10);

    //闪烁
    auto fadeOut = FadeOut::create(2);
    auto fadeIn = FadeIn::create(2);

    auto delay = DelayTime::create(5);
    auto delay2 = DelayTime::create(1.5);
    auto repeat = RepeatForever::create(Sequence::create(fadeOut, delay, fadeIn, delay2, NULL));
    dark->runAction(repeat);

    return true;
}