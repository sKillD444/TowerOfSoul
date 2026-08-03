#pragma once
#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__
#include "cocos2d.h"
#include <iostream>
#include "CardNode.h"
#include "ui/CocosGUI.h"
#include "Controller/BattleController.h"
#include "Controller/PlayerController.h"
#include "Controller/LeaderboardController.h"
#include "View/ResponsiveHelper.h"
#include "View/MenuScene.h"
#include "View/CampaignScene.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
using namespace std;
using namespace cocos2d::ui;

class BattleScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene(bool isCampaign = false, int stageId = 1);
	virtual bool init(bool isCampaign = false, int stageId = 1);

	enum class ZOrder { BackGround, Slot, Deck, Character, UI, Shop, Card, Notification};

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
		bool isPlayerSide = true;
	};

	vector<Slot> boardSlots;
	vector<Slot> deckSlots;
	vector<Slot> ShopSlots;
	vector<Slot> reinforcementSlots;

	struct CombatAction {
		Slot* attacker;
		Slot* target;
		float damage;
		bool isPlayerAttacking;
		bool targetDied;
	};

	struct CombatCluster {
		vector<Slot*> allies;
		vector<Slot*> enemies;
	};

	virtual void update(float dt) override;

private:

	BattleController _controller;
	PlayerController _pController;
	LeaderboardController _lController;

	//Layers
	cocos2d::Node* _uiLayer;
	cocos2d::Node* _shopLayer = nullptr;
	cocos2d::Node* _settingLayer = nullptr;
	cocos2d::Node* _logBattleLayer = nullptr;
	cocos2d::Node* _buyDialog = nullptr;
	
	
	//Label
	cocos2d::Label* _infoLabel = nullptr;
	cocos2d::Label* _logLabel = nullptr;
	cocos2d::Label* _coinLabel = nullptr;
	cocos2d::Label* _resultLabel = nullptr;
	cocos2d::Label* _quantityLabel = nullptr;
	
	CardNode* selectedCard = nullptr;
	BattleCardData selectedData;
	vector<cocos2d::Sprite*> arrIconCoin;
	vector<CombatAction> _actionQueue;
	vector<cocos2d::Label*> arrLabelCoin;

	int _coins = 0;
	int _round =5;
	int _currentAttackerIndex = 0;
	int _turnBuffElement = 0;
	bool _isBattle = false;
	bool _isPlayerTurn = true;
	bool _isCalculatingCombat = false;
	string _logBattle = "";
	string _buffElement = "";
	string _selectedBuffElement = "";
	float _turnTimer = 0.5f;

	mutex _mtx;
	
	int _buyQuantity = 1;
	BattleCardData _tempCardData;

	//Create UI
	void createUI();
	void createArena(cocos2d::Vec2 ignoredPos);
	void createDeck();
	void createShop();
	void createSetting();
	void createLogBattle();
	void createBuyDialog();
	void createReinforcementSlots();
	void highlightElementBtn(cocos2d::Sprite* selectedBtn);
	void loadMap();

	//Game Logic
	void startNewRound();
	void startBattle();
	void endBattle();
	void endGame();
	
	//Card Management
	int countUnitsOnBoard();
	int countCard(BattleCardData data);
	void mergeCard(Slot& card);
	bool deleteCard(CardNode* select, BattleCardData data);
	void swapCard(Slot& selectCard, Slot& Card);

	//Battle Logic
	void loadCardShop();
	void highLightLine(bool flag);
	void spawnEnemies();
	void spawnHiddenEnemy();
	Slot* findTarget(vector<Slot*>& targetSlots, Slot* attacker);
	float synergyAtkMult(string role);
	float culateAllDamage(Slot& attacker, Slot& target, bool isPlayerAttacking);
	void doAttack(Slot& attacker, Slot& target, bool isPlayerAttacking);
	void buffElement(string type);
	void resetElementButtons();
	vector<CombatCluster> groupUnits();
	void startCombatCalculation();
	void playCombatAnimations(const vector<CombatAction>& actions);
	
	//Event Touch
	void setUpTouchListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event);

	//Campaign
	bool _isCampaign = false;
	int _currentStageId = 1;
	int _currentWave = 1;
	int _maxWaves = 1;

	void loadTeam();
	void spawnCampaignEnemies(int stageId, int wave);

	//Test

	CREATE_FUNC(BattleScene);
};

#endif // __BATTLE_SCENE_H__