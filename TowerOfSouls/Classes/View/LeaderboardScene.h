#pragma once
#ifndef __LEADERBOARD_SCENE_H__
#define __LEADERBOARD_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Controller/PlayerController.h"
#include "Controller/LeaderboardController.h"
#include "CardNode.h"

using namespace std;

class LeaderboardScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	enum class ZOrder
	{
		BG, Button, Label,Card, Info 
	};
	virtual bool init() override;
private:
	PlayerController _pController;
	LeaderboardController _lController;

	void createLeaderboardUI();

	void backHome();
	CREATE_FUNC(LeaderboardScene);
};

#endif // __LEADERBOARD_SCENE_H__