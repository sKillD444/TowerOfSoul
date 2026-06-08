#pragma once
#ifndef __TEAM_SCENE_H__
#define __TEAM_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Controller/PlayerController.h"
#include "Controller/BattleController.h"
#include "CardNode.h"

using namespace std;

class TeamScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	enum class ZOrder
	{
		BG, Slot, Button, Card, Info
	};

	virtual bool init() override;
private:
	PlayerController _pController;

	void backHome();
	CREATE_FUNC(TeamScene);
};

#endif // __TEAM_SCENE_H__