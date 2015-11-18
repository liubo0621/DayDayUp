//
//  MenuScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#include "MenuScene.h"
#include "NormalScene.h"
#include "ElasticityScene.h"//弹力模式
#include "TimeScene.h"
#include "TwinkleScene.h"//闪烁模式
#include "DarkScene.h"
#include "OppositeGravityScene.h"
#include "SettingScene.h"


Scene * MenuScene::createScene(){
    auto scene = Scene::createWithPhysics();
    auto layer=MenuScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool MenuScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    visibleSize=Director::getInstance()->getVisibleSize();
    
    //背景
    int random=arc4random()%3;
    switch (random) {
        case 0:
        {
            auto bg1=Sprite::create("menu_bg1.png");
            bg1->setPosition(visibleSize/2);
            this->addChild(bg1,-2);
            break;
        }
        case 1:
        {
            auto bg2=Sprite::create("menu_bg2.png");
            bg2->setPosition(visibleSize/2);
            this->addChild(bg2,-2);
            break;
        }
        case 2:
        {
            auto bg3=Sprite::create("menu_bg3.png");
            bg3->setPosition(visibleSize/2);
            this->addChild(bg3,-2);
            break;
        }
    }
 
    
    //天天向上
    auto day1=Sprite::create("menu_0000.png");
    day1->setPosition(visibleSize.width*0.2,visibleSize.height*0.8);
    this->addChild(day1);
    
    auto day2=Sprite::create("menu_0001.png");
    day2->setPosition(visibleSize.width*0.38,visibleSize.height*0.8);
    this->addChild(day2);
    
    auto go=Sprite::create("menu_0002.png");
    go->setPosition(visibleSize.width*0.6,visibleSize.height*0.8);
    this->addChild(go);
    
    auto up=Sprite::create("menu_0003.png");
    up->setPosition(visibleSize.width*0.8,visibleSize.height*0.8);
    this->addChild(up);
    
    //********************菜单按钮***********************************
    auto btn_width=310; //按钮宽的一半
    
    //正常模式
    auto func1=CallFunc::create([=]{
        auto position1=Vec2(-btn_width,visibleSize.height*0.6);
        addBtn(position1, "menu_btn_bg.png", "menu_0008.png");
    });
    
    //弹力模式
    auto func2=CallFunc::create([=]{
        auto position2=Vec2(-btn_width,visibleSize.height*0.5);
        addBtn(position2, "menu_btn_bg.png", "menu_0012.png");
    });
    
    //时间模式
    auto func3=CallFunc::create([=]{
        auto position3=Vec2(-btn_width,visibleSize.height*0.4);
        addBtn(position3, "menu_btn_bg.png", "menu_0004.png");
    });
    
    //闪烁模式
    auto func4=CallFunc::create([=]{
        auto position4=Vec2(-btn_width,visibleSize.height*0.3);
        addBtn(position4, "menu_btn_bg.png", "menu_0006.png");
    });
    
    //暗黑模式
    auto func5=CallFunc::create([=]{
        auto position5=Vec2(-btn_width,visibleSize.height*0.2);
        addBtn(position5, "menu_btn_bg.png", "menu_0010.png");
    });
    
    //反重力模式
    auto func6=CallFunc::create([=]{
        auto position6=Vec2(-btn_width,visibleSize.height*0.1);
        addBtn(position6, "menu_btn_bg.png", "menu_0013.png");
    });
    
    //设置按钮
    _isShowSetting=false;
    
    auto func7=CallFunc::create([=]{
        //按钮btn
        _settingBtn=Button::create("setting.png");
        _settingBtn->setPosition(Vec2(visibleSize.width-_settingBtn->getContentSize().width/2-30,-_settingBtn->getContentSize().height/2));
        _settingBtn->setOpacity(190);
        this->addChild(_settingBtn);
        
        auto moveTo=MoveTo::create(1, Vec2(visibleSize.width-_settingBtn->getContentSize().width/2-30,_settingBtn->getContentSize().height/2-10));
        _settingBtn->runAction(moveTo);
        
        //设置层
        settingLayer=Setting::create();
        settingLayer->setPosition(visibleSize.width,0);
        this->getScene()->addChild(settingLayer);
        
        //返回首页按钮
        //*******************返回首页***************
        goHomeBtn=Button::create("menu_exit_btn.png");
        goHomeBtn->setPosition(Vec2(visibleSize.width+goHomeBtn->getContentSize().width/2,visibleSize.height*0.25));
        this->addChild(goHomeBtn);
        
        auto gohome=Sprite::create("menu_gohome.png");
        goHomeBtn->addChild(gohome);
        gohome->setPosition(goHomeBtn->getContentSize()/2);
        
        //返回首页点击事件
        goHomeBtn->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
            if (type==Widget::TouchEventType::ENDED) {
                moveBack();
            }
            
        });

        
        //添加按钮点击事件
        _settingBtn->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
            if (type==Widget::TouchEventType::ENDED) {
                //弹出设置页面
                if (!_isShowSetting) {
                    _isShowSetting=true;
                    //设置按钮不可点 移除按钮
                    for(auto btn:_buttons){
                        btn->setTouchEnabled(false);
                        
                        auto moveTo=MoveTo::create(0.2, Vec2(-btn_width,btn->getPositionY()));
                        btn->runAction(moveTo);
                    }
                    //将设置层移入
                    auto moveIn=MoveTo::create(0.2, Vec2(0, 0));
                    settingLayer->runAction(moveIn);
                    
                    //移入返回首页按钮
                    auto goHomeMoveIn=MoveTo::create(0.2, Vec2(visibleSize.width/2, visibleSize.height*0.25));
                    goHomeBtn->runAction(goHomeMoveIn);
                }
                else{
                    moveBack();
                }
                
            }
        });
        
        //设置文本
        auto setting=Sprite::create("setting_text.png");
        _settingBtn->addChild(setting);
        setting->setScale(0.8);
        setting->setPosition(Vec2(_settingBtn->getContentSize().width/2,_settingBtn->getContentSize().height*0.5));
        
    });
    

    //天天向上 破浪效果
    auto func01=CallFunc::create([=]{
        addWave(day1);
    });
    
    auto func02=CallFunc::create([=]{
        addWave(day2);
    });

    auto func03=CallFunc::create([=]{
        addWave(go);
    });

    auto func04=CallFunc::create([=]{
        addWave(up);
    });

    auto delay=DelayTime::create(0.3);
    auto delay2=DelayTime::create(0.5);
    this->runAction(Sequence::create(func1,delay,func2,delay,func3,delay,func4,delay,func5,delay,func6,func7,
                                     DelayTime::create(0.5),
                                     func01,delay2,func02,delay2,func03,delay2,func04,
                                     NULL));

    //烟花
    schedule(schedule_selector(MenuScene::addFireWorks),3);
    
    
    //预加载声音资源
    SimpleAudioEngine::getInstance()->preloadEffect("die1.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("flapSFX.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("point.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("NewHigh.mp3");
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bg_country.mp3");
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bg_country.mp3",true);

    
    return true;
}

//烟花
void MenuScene::addFireWorks(float delta){
    auto x=arc4random()%(int)visibleSize.width*0.8+visibleSize.width*0.1;
    auto y=arc4random()%(int)visibleSize.height*0.4+visibleSize.height*0.5;
    
    auto fireworks=ParticleSystemQuad::create("fireworks.plist");
    fireworks->setLife(2);
    fireworks->setLifeVar(2);
    fireworks->setStartSize(30);
    fireworks->setEmissionRate(fireworks->getTotalParticles()/fireworks->getLife());
    fireworks->setAutoRemoveOnFinish(true);
    fireworks->setPosition(Vec2(x,y));
    this->addChild(fireworks,-1);
}

void MenuScene::addBtn(cocos2d::Vec2 position, string btnName, string textName){
    //按钮
    auto btn=Button::create(btnName);
    btn->setPosition(position);
    this->addChild(btn);
    _buttons.pushBack(btn);
    
    //文字
    auto text=Sprite::create(textName);
    btn->addChild(text);
    text->setPosition(btn->getContentSize()/2);
    
    //移动
    auto moveTo=MoveTo::create(0.5, Vec2(visibleSize.width/2, position.y));
    btn->runAction(moveTo);
    
    //事件
    btn->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type){
        if (type==Widget::TouchEventType::ENDED) {
            if (textName=="menu_0008.png") {
                UserDefault::getInstance()->setStringForKey("model", "正常模式");
                Director::getInstance()->replaceScene(Game::createScene());
            }
            if (textName=="menu_0012.png") {
                UserDefault::getInstance()->setStringForKey("model", "弹力模式");
                Director::getInstance()->replaceScene(ElasticityModel::createScene());
            }
            if (textName=="menu_0004.png") {
                UserDefault::getInstance()->setStringForKey("model", "时间模式");
                Director::getInstance()->replaceScene(TimeModel::createScene());
            }
            if (textName=="menu_0006.png") {
                UserDefault::getInstance()->setStringForKey("model", "闪烁模式");
                Director::getInstance()->replaceScene(TwinkleModel::createScene());
            }
            if (textName=="menu_0010.png") {
                UserDefault::getInstance()->setStringForKey("model", "暗黑模式");
                Director::getInstance()->replaceScene(DarkModel::createScene());
            }
            if (textName=="menu_0013.png") {
                UserDefault::getInstance()->setStringForKey("model", "反重力模式");
                Director::getInstance()->replaceScene(OppositeGravityModel::createScene());
            }
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            
        }
    
    });
}

void MenuScene::addWave(Sprite *sender){
    auto moveUp=MoveBy::create(0.5, Vec2(0, 150));
    auto moveDown=moveUp->reverse();
    auto action = RepeatForever::create(Sequence::create(moveUp,moveDown,DelayTime::create(5), NULL));
    sender->runAction(action);

}

void MenuScene::moveBack(){
    //移回按钮
    for(auto btn:_buttons){
        btn->setTouchEnabled(true);
        
        auto moveTo=MoveTo::create(0.2, Vec2(visibleSize.width/2,btn->getPositionY()));
        btn->runAction(moveTo);
        _isShowSetting=false;
    }
    //将设置层移出
    auto moveOut=MoveTo::create(0.2, Vec2(visibleSize.width, 0));
    settingLayer->runAction(moveOut);
    
    //移出返回首页按钮
    auto goHomeMoveOut=MoveTo::create(0.2, Vec2(visibleSize.width+goHomeBtn->getContentSize().width/2, visibleSize.height*0.25));
    goHomeBtn->runAction(goHomeMoveOut);
}