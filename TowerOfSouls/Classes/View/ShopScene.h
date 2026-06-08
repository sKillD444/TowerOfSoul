#pragma once
#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "ui/CocosGUI.h"
#include "Controller/PlayerController.h"
#include "Controller/BattleController.h"
#include "CardNode.h"

using namespace std;

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
	std::vector<std::vector<cocos2d::Sprite*>> _spriteGrid;
	cocos2d::Node* _cardLayer = nullptr;

	void createGacha();
	void backHome();
	int rndCost(bool isGem);
	void RndCard(bool isGem, bool rollOne);
	void drawCard(vector<BattleCardData> allCard);
	CREATE_FUNC(ShopScene);
};

#endif // __SHOP_SCENE_H__