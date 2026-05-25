#ifndef __CARD_NODE_H__
#define __CARD_NODE_H__

#include "cocos2d.h"
#include "../GameData.h"
#include <iostream>
using namespace std;

class CardNode : public cocos2d::Node {
public:
    static CardNode* createNode(const BattleCardData& data, float cellW, float cellH);
    virtual bool init(const BattleCardData& data, float cellW, float cellH);

    void updateStats(float currentHp, float currentAtk);

    void playDeathAnimation(float cellW, float cellH);
    void playLungeAnimation(cocos2d::Vec2 targetPos, cocos2d::Vec2 basePos);
    void playHitAnimation(cocos2d::Vec2 basePos);

    void upStar(int newStar);

    cocos2d::Sprite* getSprite() const { return _bgSprite; }

private:
    cocos2d::Sprite* _bgSprite;
    cocos2d::Label* _hpLabel;
    cocos2d::Label* _atkLabel;
    cocos2d::Label* _starLabel;
    float _baseScale;
    void resetStateAndZOrder(cocos2d::Vec2 basePos);
};

#endif // __CARD_NODE_H__