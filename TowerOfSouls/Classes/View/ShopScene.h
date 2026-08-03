#pragma once
#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Controller/PlayerController.h"
#include "Controller/BattleController.h"
#include "View/ResponsiveHelper.h"
#include "View/PlayerHUD.h"
#include "View/MenuScene.h"
#include "CardNode.h"

using namespace std;
using namespace cocos2d::ui;

class ShopScene : public cocos2d::Scene
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
	BattleController _cController;
	vector<vector<cocos2d::Sprite*>> _spriteGrid;
	cocos2d::Node* _cardLayer = nullptr;

	void createGacha();
	void backHome();
	int rndCost(bool isGem);
	void RndCard(bool isGem, int rollCount);
	void drawCard(vector<BattleCardData> allCard);
	CREATE_FUNC(ShopScene);
};

#endif // __SHOP_SCENE_H__