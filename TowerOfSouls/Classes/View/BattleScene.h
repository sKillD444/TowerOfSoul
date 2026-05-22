#pragma once
#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "CardNode.h"
#include "ui/CocosGUI.h"
#include "Controller/BattleController.h"
#include "MenuScene.h"
using namespace std;

class BattleScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	enum class ZOrder { BackGround, Slot, Deck, Character, UI, Shop, Card, Notification};

	virtual bool init() override;

	cocos2d::Vec2 originalPos;
	bool _btnShopEnabled = true;
	bool _btnPlayEnabled = true;
	bool _btnSettingEnabled = true;

	//Slot Battle
	struct Slot {
		cocos2d::Rect area;
		CardNode* card;
		bool isEmpty;
		cocos2d::Vec2 pos;
		BattleCardData data;
	};

	vector<Slot> playerSlots;
	vector<Slot> enemySlots;
	vector<Slot> deckSlots;
	vector<Slot> ShopSlots;

	virtual void update(float dt) override;

private:
	BattleController _controller;
	cocos2d::Node* _uiLayer;
	cocos2d::Node* _shopLayer = nullptr;
	cocos2d::Node* _settingLayer = nullptr;
	cocos2d::Node* _logBattleLayer = nullptr;
	CardNode* selectedCard = nullptr;
	BattleCardData selectedData;
	cocos2d::Label* _infoLabel = nullptr;
	cocos2d::Label* _logLabel = nullptr;
	cocos2d::Label* _coinLabel = nullptr;
	cocos2d::Label* _resultLabel = nullptr;
	std::vector<cocos2d::Label*> arrLabelCoin;
	std::vector<cocos2d::Sprite*> arrIconCoin;

	int _coins = 0;
	int _round =0;
	bool _isBattle = false;
	int _currentAttackerIndex = 0;
	bool _isPlayerTurn = true;
	string _logBattle = "";
	float _turnTimer = 0.5f;

	void createUI();
	void createGrid3x3(cocos2d::Vec2 startPos, bool isPlayer);
	void createDeck();
	void createShop();
	void createSetting();
	void createLogBattle();


	void startNewRound();
	void startBattle();
	void endBattle();
	void endGame();

	void loadCardShop();
	void spawnEnemies();
	bool deleteCard(CardNode* select, BattleCardData data);
	void highLightLine(bool flag);
	int countCard(BattleCardData data);
	void mergeCard(Slot& card);
	void swapCard(Slot& selectCard, Slot& Card);
	int countUnitsOnBoard();

	Slot* findTarget(std::vector<Slot>& targetSlots, Slot& attacker);
	void doAttack(Slot& attacker, Slot& target, bool isPlayerAttacking);
	float synergyAtkMult(std::string role);
	float culateAllDamage(Slot& attacker, Slot& target, bool isPlayerAttacking);
	

	void setUpTouchListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(BattleScene);
};

#endif // __BATTLE_SCENE_H__