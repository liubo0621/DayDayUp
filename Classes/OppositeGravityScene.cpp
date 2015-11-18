//
//  OppositeGravityScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/28.
//
//

#include "OppositeGravityScene.h"

Scene * OppositeGravityModel::createScene(){
    auto scene = Scene::createWithPhysics();
    //    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, 2000));
    
    auto layer = OppositeGravityModel::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OppositeGravityModel::init(){
    if (!Game::init()) {
        return false;
    }
    
    return true;
}

void OppositeGravityModel::onEnter(){
    Layer::onEnter();
    
    _firstTouch=true;
    _onTouch=false;
    _moveStep=20;
    
    auto listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=[=](Touch *touch, Event *event){
        _onTouch=true;
        
        if (!_ballBody->isGravityEnabled()) {
            _ballBody->setGravityEnable(true);
        }
        if(_firstTouch){
            _firstTouch=false;
            
            auto moveOut=MoveTo::create(0.2, Vec2(_visibleSize.width/2,_visibleSize.height+_ready_go->getContentSize().height/2));
            auto function=CallFunc::create([=]{
                this->removeChild(_ready_go);
                this->removeChild(_tip);
                _scoreLabel->setString("0");
            });
            
            _ready_go->runAction(Sequence::create(moveOut,function, NULL));
        }
        SimpleAudioEngine::getInstance()->playEffect("flapSFX.mp3");
        _ballBody->setVelocity(Vec2(0, -500));
        _moveStep=20;
       
        return true;
    };
    listener->onTouchEnded=[=](Touch *touch, Event *event){
        _onTouch=false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//移动障碍物
void OppositeGravityModel::moveObstacles(float delta){
    //如果球的高度到大于屏幕中心位置，则障碍物下移
    if (_ball->getPosition().y>_visibleSize.height/2) {
        if (!_onTouch) {
            //设置球为静态  不让他的位置超过屏幕的一半
            _ballBody->setGravityEnable(false);
            _ballBody->setVelocity(Vec2(0, 0));
        }
        
        //障碍物下移
        for (auto obstacle:_obstacles ) {
            obstacle->setPositionY(obstacle->getPositionY()-_moveStep);
        }
        
        //移动faster
        if (_bestScoreOnShow) {
            _fasterSprite->setPositionY(_fasterSprite->getPositionY()-_moveStep);
        }
        _moveStep+=2;
    }
}
