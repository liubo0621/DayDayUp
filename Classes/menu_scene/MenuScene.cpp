//
//  MenuScene.cpp
//  DayDayUp
//
//  Created by LIUBO on 15/8/10.
//
//

#include "AdManagerProtocol.h"
#include "Constants.h"
#include "DarkScene.h"
#include "ElasticityScene.h"  //弹力模式
#include "MenuScene.h"
#include "MobClickCpp.h"
#include "NormalScene.h"
#include "OppositeGravityScene.h"
#include "PropsScene.h"
#include "TimeScene.h"
#include "TwinkleScene.h"  //闪烁模式

#define MOVE_TIME 0.2
#define SHOW_DIFFICULTY_MENU_NUM 3
#define SHOW_MODEL_MENU_NUM 7

Scene* MenuScene::createScene() {
    auto scene = Scene::createWithPhysics();
    auto layer = MenuScene::create();
    scene->addChild(layer);

    return scene;
}

bool MenuScene::init() {
    if (!Layer::init()) {
        return false;
    }

    //广告
    AdManagerProtocol::getInstance()->showAD(-3);  //清除横幅

    _visibleSize = Director::getInstance()->getVisibleSize();
    _originSize = Director::getInstance()->getVisibleOrigin();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");

    //背景
    int random = arc4random() % 3;
    switch (random) {
        case 0: {
            auto bg1 = Sprite::create("menu_bg1.png");
            bg1->setPosition(_visibleSize / 2);
            this->addChild(bg1, -2);
            _btnTextColor = Color4B(167, 132, 191, -27);
            break;
        }
        case 1: {
            auto bg2 = Sprite::create("menu_bg2.png");
            bg2->setPosition(_visibleSize / 2);
            this->addChild(bg2, -2);
            _btnTextColor = Color4B(84, 229, 189, -48);
            break;
        }
        case 2: {
            auto bg3 = Sprite::create("menu_bg3.png");
            bg3->setPosition(_visibleSize / 2);
            this->addChild(bg3, -2);
            _btnTextColor = Color4B(151, 208, 165, -26);
            break;
        }
    }

    //天天向上
    auto day1 = Sprite::createWithSpriteFrameName("menu_0000.png");
    day1->setPosition(_visibleSize.width * 0.2, _visibleSize.height * 0.78);
    this->addChild(day1);

    auto day2 = Sprite::createWithSpriteFrameName("menu_0001.png");
    day2->setPosition(_visibleSize.width * 0.4, _visibleSize.height * 0.8);
    this->addChild(day2);

    auto go = Sprite::createWithSpriteFrameName("menu_0002.png");
    go->setPosition(_visibleSize.width * 0.6, _visibleSize.height * 0.78);
    this->addChild(go);

    auto up = Sprite::createWithSpriteFrameName("menu_0003.png");
    up->setPosition(_visibleSize.width * 0.8, _visibleSize.height * 0.8);
    this->addChild(up);

    //开始按钮
    addStartBtn();

    //选择模式btn
    _selectModelBtn = Button::create("menu_btn.png", "", "", TextureResType::PLIST);
    _selectModelBtn->setPosition(Vec2(_visibleSize.width / 5, _visibleSize.height * 0.5));
    auto model = UserDefault::getInstance()->getStringForKey("model", "模式");
    addChild(setBtntext(model.c_str(), _selectModelBtn));
    _selectModelBtn->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (!_isShow) {
                MenuScene::showMenuBtn(MenuBtnType::MODEL, 108, 36, false, _visibleSize.width * 0.36, SHOW_MODEL_MENU_NUM);
            } else {
                MenuScene::dissmissMenuBtn(MenuBtnType::MODEL, SHOW_MODEL_MENU_NUM);
            }
        }
    });

    //选择难度btn
    _selectDifficultyBtn = Button::create("menu_btn.png", "", "", TextureResType::PLIST);
    _selectDifficultyBtn->setPosition(Vec2(_visibleSize.width * 4 / 5, _visibleSize.height * 0.54));
    auto difficulty = UserDefault::getInstance()->getStringForKey("difficulty", "难度");
    addChild(setBtntext(difficulty.c_str(), _selectDifficultyBtn));

    _selectDifficultyBtn->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (!_isShow) {
                MenuScene::showMenuBtn(MenuBtnType::DIFFICULTY, 135, 45, true, _visibleSize.width * 0.36, SHOW_DIFFICULTY_MENU_NUM);
            } else {
                MenuScene::dissmissMenuBtn(MenuBtnType::DIFFICULTY, SHOW_DIFFICULTY_MENU_NUM);
            }
        }
    });

    addMenuBtn(_selectModelBtn, NORMAL_MODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, ELASTICITY_MODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, TIME_MODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, TWINKLE_MODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, DARK_MODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, OPPOSITE_GRAVITY_DMODEL, MenuBtnType::MODEL);
    addMenuBtn(_selectModelBtn, PROPS_MODEL, MenuBtnType::MODEL, true);

    addMenuBtn(_selectDifficultyBtn, EASY, MenuBtnType::DIFFICULTY);
    addMenuBtn(_selectDifficultyBtn, MIDDLE, MenuBtnType::DIFFICULTY);
    addMenuBtn(_selectDifficultyBtn, HARD, MenuBtnType::DIFFICULTY);

    //声音
    auto soundBtn = Button::create();
    soundBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    soundBtn->setPosition(Vec2(_visibleSize.width - 15, _originSize.height + 15));
    if (UserDefault::getInstance()->getBoolForKey("isSoundOn", true)) {
        soundBtn->loadTextureNormal("sound_on.png", TextureResType::PLIST);
    } else {
        soundBtn->loadTextureNormal("sound_off.png", TextureResType::PLIST);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
    soundBtn->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            if (UserDefault::getInstance()->getBoolForKey("isSoundOn", true)) {
                SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                SimpleAudioEngine::getInstance()->setEffectsVolume(0);
                soundBtn->loadTextureNormal("sound_off.png", TextureResType::PLIST);
                UserDefault::getInstance()->setBoolForKey("isSoundOn", false);
            } else {
                SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
                SimpleAudioEngine::getInstance()->setEffectsVolume(1);
                soundBtn->loadTextureNormal("sound_on.png", TextureResType::PLIST);
                UserDefault::getInstance()->setBoolForKey("isSoundOn", true);
            }
        }
    });
    addChild(soundBtn);

    //天天向上 破浪效果
    auto func01 = CallFunc::create([=] { addWave(day1); });

    auto func02 = CallFunc::create([=] { addWave(day2); });

    auto func03 = CallFunc::create([=] { addWave(go); });

    auto func04 = CallFunc::create([=] { addWave(up); });

    auto delay2 = DelayTime::create(0.5);
    this->runAction(Sequence::create(func01, delay2, func02, delay2, func03, delay2, func04, NULL));

    //烟花
    schedule(schedule_selector(MenuScene::addFireWorks), 3);

    //预加载声音资源
    SimpleAudioEngine::getInstance()->preloadEffect("die1.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("flapSFX.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("point.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("NewHigh.mp3");
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bg_country.mp3");

    SimpleAudioEngine::getInstance()->playBackgroundMusic("bg_country.mp3", true);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [](Touch* touch, Event* event) { return true; };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        if (_isShow) {
            if (_selectDifficultyBtn->getPositionX() > _visibleSize.width) {
                MenuScene::dissmissMenuBtn(MenuBtnType::MODEL, SHOW_MODEL_MENU_NUM);
            } else {
                MenuScene::dissmissMenuBtn(MenuBtnType::DIFFICULTY, SHOW_DIFFICULTY_MENU_NUM);
            }
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

//烟花
void MenuScene::addFireWorks(float delta) {
    auto x = arc4random() % (int)_visibleSize.width * 0.8 + _visibleSize.width * 0.1;
    auto y = arc4random() % (int)_visibleSize.height * 0.4 + _visibleSize.height * 0.5;

    auto fireworks = ParticleSystemQuad::create("fireworks.plist");
    fireworks->setLife(2);
    fireworks->setLifeVar(2);
    fireworks->setStartSize(30);
    fireworks->setEmissionRate(fireworks->getTotalParticles() / fireworks->getLife());
    fireworks->setAutoRemoveOnFinish(true);
    fireworks->setPosition(Vec2(x, y));
    this->addChild(fireworks, -1);
}

void MenuScene::addStartBtn() {
    _startBtn = Button::create("start.png", "", "", TextureResType::PLIST);
    _startBtn->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height * 0.3));
    addChild(_startBtn);

    _startBtn->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            log("start game");
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            MenuScene::replaceScene();
        }

    });
}

void MenuScene::addMenuBtn(Button* parent, const char* text, MenuBtnType type, bool isNew) {
    auto btn = Button::create("menu_btn.png", "", "", TextureResType::PLIST);
    btn->setPosition(parent->getContentSize() / 2);
    btn->setVisible(false);
    if (isNew) {
        auto newTip = Sprite::createWithSpriteFrameName("menu_new.png");
        btn->addChild(newTip);
        newTip->setPosition(btn->getContentSize());
    }

    parent->addChild(setBtntext(text, btn));
    if (type == MenuBtnType::DIFFICULTY) {
        _difficultyBtns.pushBack(btn);
        btn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::ENDED) {
                MenuScene::dissmissMenuBtn(MenuBtnType::DIFFICULTY, SHOW_DIFFICULTY_MENU_NUM);
                UserDefault::getInstance()->setStringForKey("difficulty", text);
                setBtntext(text, _selectDifficultyBtn);
            }

        });

    } else if (type == MenuBtnType::MODEL) {
        _modelBtns.pushBack(btn);
        btn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::ENDED) {
                MenuScene::dissmissMenuBtn(MenuBtnType::MODEL, SHOW_MODEL_MENU_NUM);
                UserDefault::getInstance()->setStringForKey("model", text);
                setBtntext(text, _selectModelBtn);
            }
        });
    }
}

void MenuScene::addWave(Sprite* sender) {
    auto moveUp = MoveBy::create(0.5, Vec2(0, 150));
    auto moveDown = moveUp->reverse();
    auto action = RepeatForever::create(Sequence::create(moveUp, moveDown, DelayTime::create(5), NULL));
    sender->runAction(action);
}

Button* MenuScene::setBtntext(const char* text, cocos2d::ui::Button* btn) {
    if (btn->getChildByTag(10) != nullptr) {
        static_cast<Label*>(btn->getChildByTag(10))->setString(text);
        return nullptr;
    }

    auto label = Label::createWithTTF("", "jiancuyuan.ttf", 50);
    label->setString(text);
    label->setSystemFontSize(50);
    label->setPosition(btn->getContentSize() / 2);
    label->setTag(10);
    label->setTextColor(_btnTextColor);

    btn->addChild(label);

    //    auto clippingNode = ClippingNode::create();
    //    clippingNode->setStencil(label);
    //    clippingNode->setInverted(true);
    //    clippingNode->setAlphaThreshold(0.04);
    //    clippingNode->setTag(20);
    //    clippingNode->addChild(btn);
    return btn;
}

void MenuScene::showMenuBtn(MenuBtnType type, int startAngle, int rotateAngle, bool isClockwise, int r, int num) {
    _isShow = true;
    //移动开始按钮
    auto startBtnMoveX = _startBtn->getPositionX();
    auto startBtnMoveY = _originSize.height - _startBtn->getContentSize().height;
    auto moveTo = MoveTo::create(MOVE_TIME, Vec2(startBtnMoveX, startBtnMoveY));
    _startBtn->runAction(moveTo);

    //移动难度或者模式选择按钮
    if (type == MenuBtnType::MODEL) {
        auto moveTo = MoveTo::create(MOVE_TIME, Vec2(_visibleSize.width + _selectDifficultyBtn->getContentSize().width, _selectDifficultyBtn->getPositionY()));
        _selectDifficultyBtn->runAction(moveTo);
    } else if (type == MenuBtnType::DIFFICULTY) {
        auto moveTo = MoveTo::create(MOVE_TIME, Vec2(-_selectModelBtn->getContentSize().width, _selectModelBtn->getPositionY()));
        _selectModelBtn->runAction(moveTo);
    }

    //移动菜单按钮
    for (int i = 0; i < num; i++) {
        auto moveX = r * cosf(startAngle * M_PI / 180);
        auto moveY = r * sinf(startAngle * M_PI / 180);
        auto moveBy = MoveBy::create(MOVE_TIME, Vec2(moveX, moveY));
        if (type == MenuBtnType::DIFFICULTY) {
            if (i >= _difficultyBtns.size()) {
                return;
            }
            _difficultyBtns.at(i)->setVisible(true);
            _difficultyBtns.at(i)->runAction(moveBy);
        } else if (type == MenuBtnType::MODEL) {
            if (i >= _modelBtns.size()) {
                return;
            }
            _modelBtns.at(i)->setVisible(true);
            _modelBtns.at(i)->runAction(moveBy);
        }

        if (isClockwise) {
            startAngle += rotateAngle;
        } else {
            startAngle -= rotateAngle;
        }
    }
}

void MenuScene::dissmissMenuBtn(MenuBtnType type, int num) {
    _isShow = false;
    //移回开始按钮
    _startBtn->stopAllActions();
    auto moveTo = MoveTo::create(MOVE_TIME, Vec2(_visibleSize.width / 2, _visibleSize.height * 0.3));
    _startBtn->runAction(moveTo);

    //移回难度或者模式选择按钮
    if (type == MenuBtnType::MODEL) {
        _selectDifficultyBtn->stopAllActions();
        auto moveTo = MoveTo::create(MOVE_TIME, Vec2(_visibleSize.width * 4 / 5, _visibleSize.height * 0.54));
        _selectDifficultyBtn->runAction(moveTo);
    } else if (type == MenuBtnType::DIFFICULTY) {
        _selectModelBtn->stopAllActions();
        auto moveTo = MoveTo::create(MOVE_TIME, Vec2(_visibleSize.width / 5, _visibleSize.height * 0.5));
        _selectModelBtn->runAction(moveTo);
    }

    //移回菜单按钮
    for (int i = 0; i < num; i++) {
        if (type == MenuBtnType::DIFFICULTY) {
            if (i >= _difficultyBtns.size()) {
                return;
            }
            _difficultyBtns.at(i)->stopAllActions();
            auto moveTo = MoveTo::create(MOVE_TIME, _selectDifficultyBtn->getContentSize() / 2);
            _difficultyBtns.at(i)->runAction(Sequence::create(moveTo, CallFunc::create([=] { _difficultyBtns.at(i)->setVisible(false); }), NULL));
        } else if (type == MenuBtnType::MODEL) {
            if (i >= _modelBtns.size()) {
                return;
            }
            _modelBtns.at(i)->stopAllActions();
            auto moveTo = MoveTo::create(MOVE_TIME, _selectModelBtn->getContentSize() / 2);
            _modelBtns.at(i)->runAction(Sequence::create(moveTo, CallFunc::create([=] { _modelBtns.at(i)->setVisible(false); }), NULL));
        }
    }
}

void MenuScene::replaceScene() {
    auto model = UserDefault::getInstance()->getStringForKey("model", "null");

    Scene* nextScene = nullptr;
    if (model == NORMAL_MODEL) {
        nextScene = NormalModel::createScene();
    } else if (model == DARK_MODEL) {
        nextScene = DarkModel::createScene();
    } else if (model == ELASTICITY_MODEL) {
        nextScene = ElasticityModel::createScene();
    } else if (model == OPPOSITE_GRAVITY_DMODEL) {
        nextScene = OppositeGravityModel::createScene();
    } else if (model == TIME_MODEL) {
        nextScene = TimeModel::createScene();
    } else if (model == TWINKLE_MODEL) {
        nextScene = TwinkleModel::createScene();
    } else if (model == PROPS_MODEL) {
        nextScene = PropsScene::createScene();
    } else {
        AdManagerProtocol::getInstance()->showAD(-11);
        return;
    }

    auto difficulty = UserDefault::getInstance()->getStringForKey("difficulty", "null");
    if (difficulty == "null") {
        AdManagerProtocol::getInstance()->showAD(-12);
        return;
    }

    auto reScene = TransitionMoveInR::create(0.2, nextScene);
    Director::getInstance()->replaceScene(reScene);
}
