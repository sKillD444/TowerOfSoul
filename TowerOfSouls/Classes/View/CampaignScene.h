#pragma once
#ifndef __CAMPAIGN_SCENE_H__
#define __CAMPAIGN_SCENE_H__

#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "View/BattleScene.h"
#include "View/MenuScene.h"
#include "View/ResponsiveHelper.h"
#include "Controller/PlayerController.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class CampaignScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
private:
    void createStageNodes();
    void backHome();
    CREATE_FUNC(CampaignScene);
};
#endif // __CAMPAIGN_SCENE_H__