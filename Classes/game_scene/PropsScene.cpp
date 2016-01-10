//
//  PropsScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 16/1/10.
//
//

#include "PropsScene.h"

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