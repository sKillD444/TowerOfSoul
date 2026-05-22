#ifndef __PLAYER_HUD_H__
#define __PLAYER_HUD_H__

#include "cocos2d.h"
#include "GameData.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include <iostream>

using namespace std;
class PlayerHUD : public cocos2d::Node {
private:
    cocos2d::Label* nameLabel;
    cocos2d::Label* goldLabel;
    cocos2d::Label* gemLabel;

public:
    virtual bool init() override;
    CREATE_FUNC(PlayerHUD);

    void updatePlayerData(const PlayerData& p);
};

#endif