#ifndef __MANUAL_H__
#define __MANUAL_H__

#include "cocos2d.h"

class manual: public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(manual);
};

#endif  // __MANUAL_H__