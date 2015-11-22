//
//  Timer.h
//  DayDayUp
//
//  Created by LIUBO on 15/11/21.
//
//

#ifndef Life_h
#define Life_h

#include <stdio.h>
#include "cocos2d.h"
#include "DialogueLayer.h"

USING_NS_CC;

class Life : public Layer {
public:
    CREATE_FUNC(Life);
    virtual bool init();

    int getLife();

    void beginTimer();
    void endTimer();

    void addLife(int liftValue);
    void reduceLife();

    void showDialogue();
    //    void dissMissDialogue();
    void showCountdown(float dt);

protected:
    void recoverLife(float dt);

    // private:
    int _life;
    int _recoverTime;

    Label* _lifeLabel;
    Label* _recoverTimeLabel;

    Sprite* _lifeSprite;

    DialogueLayer* _dialogue;

    bool _isBeginTimer;
    bool _isRecoverTime;
};

#endif /* Life_h */
