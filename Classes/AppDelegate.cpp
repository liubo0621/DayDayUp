#include "AppDelegate.h"
#include "MenuScene.h"
#include "MobClickCpp.h"
#include "SimpleAudioEngine.h"
#include <ctime>

USING_NS_CC;
using namespace CocosDenshion;

#define MAX_LIFE 15
#define RECOVER_TIME 120

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

// if you want a different context,just modify the value of glContextAttrs
// it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    // set OpenGL context attributions,now can only set six attributions:
    // red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // umeng 数据统计
    const char* umengAppKey = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umengAppKey = "564c9fa1e0f55ad188000889";
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    umengAppKey = "560f92fe67e58e56b3001f9d";
#endif
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(umengAppKey, "xiaomi");

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::FIXED_WIDTH);
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // res
    FileUtils::getInstance()->addSearchPath("fonts");
    FileUtils::getInstance()->addSearchPath("gameover");
    FileUtils::getInstance()->addSearchPath("game");
    FileUtils::getInstance()->addSearchPath("menu");
    FileUtils::getInstance()->addSearchPath("sound");
    FileUtils::getInstance()->addSearchPath("utils");
    FileUtils::getInstance()->addSearchPath("game/skin_blue");
    FileUtils::getInstance()->addSearchPath("game/skin_pink");
    FileUtils::getInstance()->addSearchPath("game/skin_green");

    // create a scene. it's an autorelease object
    auto scene = MenuScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    umeng::MobClickCpp::applicationDidEnterBackground();

    Director::getInstance()->stopAnimation();
    log("applicationDidEnterBackground");
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    umeng::MobClickCpp::applicationWillEnterForeground();
    Director::getInstance()->startAnimation();
    log("applicationWillEnterForeground");

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
