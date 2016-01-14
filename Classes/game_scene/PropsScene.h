//
//  PropsScene.h
//  DayDayUp
//
//  Created by LIUBO on 16/1/10.
//
//

#ifndef PropsScene_h
#define PropsScene_h

#include "NormalScene.h"
#include <stdio.h>
class PropsScene : public NormalModel {
public:
    static Scene* createScene();
    virtual bool init();
    virtual void update(float delta);
    virtual void moveObstacles(float delta);
    virtual bool onContactBegin(PhysicsContact& contact);

    void addProps(float delta);
    void delayRemveProps(Sprite* prop);
    void hitObstacle(PhysicsBody* body);

    int random_a_b(int min, int max);

    CREATE_FUNC(PropsScene);

private:
    Vector<Sprite*> _props;

    int _addApropMapScore = 0;
    int _random = -1;

    bool _isPropected = false;
    bool _isScaleSmal = false;
    bool _isScaleBig = false;
    bool _isAddPropected = false;
    bool _isAttached = false;
    bool _isBegin = true;

    Sequence* _scaleAction;
    Sequence* _protetedAcTion;
};

#endif /* PropsScene_h */
