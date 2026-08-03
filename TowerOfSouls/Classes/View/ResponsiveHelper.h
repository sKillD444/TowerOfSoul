#ifndef _RESPONSIVE_HELPER_H_
#define _RESPONSIVE_HELPER_H_

#include "cocos2d.h"

class Responsive {
public:
    static cocos2d::Vec2 getPos(float xPercent, float yPercent) {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
        return cocos2d::Vec2(origin.x + visibleSize.width * xPercent,
            origin.y + visibleSize.height * yPercent);
    }

    static float getScale(float baseScale = 1.0f) {
        auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        return baseScale * (visibleSize.height / 320.0f);
    }

    static float getSize(float basePixel) {
        return basePixel * getScale(1.0f);
    }
};

#endif // _RESPONSIVE_HELPER_H_