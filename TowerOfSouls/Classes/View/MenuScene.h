#pragma once
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "View/BattleScene.h"
#include "View/LeaderboardScene.h"
#include "View/ShopScene.h"
#include "View/TeamScene.h"
#include "View/AuthScene.h"
#include "View/CampaignScene.h"
#include "View/SettingScene.h"
#include "Controller/PlayerController.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

using namespace std;

class MenuScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	enum class ZOrder
	{
		BG, BGButton, Button, Lable, Info
	};
	virtual bool init() override;
private:
	PlayerController _controller;

	void createButtons();
	void createUI();
	CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__