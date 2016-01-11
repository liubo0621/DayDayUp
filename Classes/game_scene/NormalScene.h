//
//  GameLayer.h
//  DayDayUp
//
//  Created by LIUBO on 15/8/7.
//  正常玩法
//

#ifndef __DayDayUp__GameLayer__
#define __DayDayUp__GameLayer__

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "math.h"
#include <stdio.h>

#define PI 3.141592653
#define SORT_OBSTACLE 5          //短障碍物
#define LONG_OBSTACLE 6          //长障碍物
#define SIDESLIPPING_OBSTACLE 7  // 侧滑障碍物
#define STACK 8                  // 两边的桩
#define FLOOR 9

USING_NS_CC;
using namespace CocosDenshion;

class NormalModel : public Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();

    // 检测碰撞
    virtual void collisionDetection(float delta);
    virtual bool onContactBegin(PhysicsContact& contact);

    virtual void update(float delta);
    //移动障碍物
    virtual void moveObstacles(float delta);
    //添加障碍物
    void addObstacle(float delta);
    Sprite* addStack(std::string spriteName, Vec2 anchorPoint, Vec2 position);
    Sprite* addSlideObstacle(std::string spriteName, Vec2 position);
    Sprite* addObstacle(std::string spriteName, Sprite* stack, Vec2 position);

    //移除障碍物
    void removeObstacle(float delta);

    //障碍物形状 height 高度 ；t 旋转时间
    void obstacle1(double height, float t);  //两侧滑动
    void obstacle2(double height, float t);  //三个短的 120度
    void obstacle3(double height, float t);  //两个长的 交叉
    void obstacle4(double height, float t);  //两个长的，60度
    void obstacle5(double height, float t);  //两个长的 90度
    void obstacle6(double height, float t);  //两个长的 顺时针旋转
    void obstacle7(double height, float t);  //两个长的 一顺一逆旋转
    void obstacle8(double height, float t);  //两个长的 旋转速度不一
    void obstacle9(double height, float t);  //两个长的 一顺一逆旋转 速度不一

    //计算分数
    virtual void addScore(float delta);

    void gameOver();

    //    double getLength(Vec2 p1,Vec2 p2);//计算两点间的距离

    CREATE_FUNC(NormalModel);

protected:
    Sprite* _ball;          //小球
    Sprite* _fasterSprite;  //最远

    PhysicsBody* _ballBody;

    Vector<Sprite*> _obstacles;  //障碍物
    Sprite* _currentSprite;
    Sprite* _ready_go;
    Sprite* _tip;

    LabelBMFont* _scoreLabel;

    Size _visibleSize;

    bool _isMove;
    bool _firstTouch;
    bool _bestScoreIsShow;
    bool _bestScoreOnShow;

    int _currentObstacle;  //当前障碍物 防止产生重复的障碍物
    int _score;
    int _count;  //当前帧数
    int _rank;   //难度等级
};

#endif /* defined(__DayDayUp__GameLayer__) */
