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

	struct Slot {
		cocos2d::Rect area;
		CardNode* card;
		bool isEmpty;
		cocos2d::Vec2 pos;
		BattleCardData data;
	};

	vector<Slot> teamSlots;
	vector<Slot> deckSlots;
	virtual bool init() override;
private:
	PlayerController _pController;
	BattleController _cController;

	cocos2d::Node* _inventoryLayer;
	std::vector<BattleCardData> _tempSelectedCards;

	void createOpenInventoryButton();
	void showInventoryPopup();
	void showCardDetailPopup(BattleCardData data, Node* cardNode);
	void confirmSelection();

	void createDeck();
	void createTeam();

	void backHome();

	void loadCardOwned();
	CREATE_FUNC(TeamScene);
};

#endif // __TEAM_SCENE_H__