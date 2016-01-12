//
//  SecondGame.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/27.
//  第二种玩法 小球变大变小
//

#include "ElasticityScene.h"

Scene* ElasticityModel::createScene() {
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -2000));

    auto layer = ElasticityModel::create();

    scene->addChild(layer);

    return scene;
}

bool ElasticityModel::init() {
    if (!NormalModel::init()) {
        return false;
    }

    //小球变大变小
    auto scaleBig = ScaleTo::create(2, 1.5, 1.5);
    auto scaleSmall = ScaleTo::create(2, 0.5, 0.5);

    auto repeat = RepeatForever::create(Sequence::create(scaleBig, scaleSmall, NULL));
    _ball->runAction(repeat);

    return true;
}