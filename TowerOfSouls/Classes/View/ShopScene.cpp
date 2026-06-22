#include "ShopScene.h"
#include "PlayerHUD.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace cocos2d::ui;
namespace L
{

}
Scene *ShopScene::createScene()
{
	return ShopScene::create();
}

bool ShopScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// ==========================================

	auto bg = LayerColor::create(Color4B(25, 35, 50, 255));
	this->addChild(bg, static_cast<int>(ZOrder::BG));

	auto hud = PlayerHUD::create();
	this->addChild(hud, static_cast<int>(ZOrder::Info), "HUD");

	PlayerData p = _pController.loadPlayer();
	hud->updatePlayerData(p);

	_cardLayer = Node::create();
	this->addChild(_cardLayer, static_cast<int>(ZOrder::Card));
	_cardLayer->setVisible(false);

	this->createGacha();
	this->backHome();

	// ==========================================
	return true;
}

void ShopScene::backHome()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto btnHome = Button::create("UI/btnBack.png");
	btnHome->setScale(0.2f);
	btnHome->setPosition(Vec2(origin.x + 40.0f, visibleSize.height - 10.0f));
	btnHome->addClickEventListener([](Ref*)
		{
			auto scene = MenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0))); });
	this->addChild(btnHome, static_cast<int>(ZOrder::Button));
}

void ShopScene::createGacha()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x = origin.x + visibleSize.width * 0.3f;
	float y = origin.y + visibleSize.height * 0.6f;
	float btn_x = origin.x + visibleSize.width * 0.2f;
	float btn_y = origin.y + visibleSize.height * 0.3f;

	auto chest_golds = Sprite::create("Items/Chest_Gold.png");
	chest_golds->setPosition(Vec2(x, y));
	chest_golds->setScale(6.0f);
	this->addChild(chest_golds, static_cast<int>(ZOrder::Button));

	auto chest_gems = Sprite::create("Items/Chest_Gem.png");
	chest_gems->setPosition(Vec2(x + 200.0f, y));
	chest_gems->setScale(5.0f);
	this->addChild(chest_gems, static_cast<int>(ZOrder::Button));

	// Label

	auto goldLabel = Label::createWithTTF("Gold", "fonts/alagard.ttf", 20);
	goldLabel->setTextColor(Color4B::YELLOW);
	goldLabel->enableBold();
	goldLabel->setPosition(Vec2(btn_x + 40.0f, y + 60.0f));
	this->addChild(goldLabel, static_cast<int>(ZOrder::Button));

	auto roll1goldLabel = Label::createWithTTF("100 Gold", "fonts/04B_03__.ttf", 8);
	roll1goldLabel->setTextColor(Color4B::YELLOW);
	roll1goldLabel->enableBold();
	roll1goldLabel->setPosition(Vec2(btn_x, btn_y - 15.0f));
	this->addChild(roll1goldLabel, static_cast<int>(ZOrder::Button));

	auto roll10goldLabel = Label::createWithTTF("900 Gold", "fonts/04B_03__.ttf", 8);
	roll10goldLabel->setTextColor(Color4B::YELLOW);
	roll10goldLabel->enableBold();
	roll10goldLabel->setPosition(Vec2(btn_x + 80.0f, btn_y - 15.0f));
	this->addChild(roll10goldLabel, static_cast<int>(ZOrder::Button));

	auto gemLabel = Label::createWithTTF("Gem", "fonts/alagard.ttf", 20);
	gemLabel->setTextColor(Color4B(255, 150, 255, 255));
	gemLabel->enableBold();
	gemLabel->setPosition(Vec2(btn_x + 250.0f, y + 60.0f));
	this->addChild(gemLabel, static_cast<int>(ZOrder::Button));

	auto roll1gemLabel = Label::createWithTTF("10 Gems", "fonts/04B_03__.ttf", 8);
	roll1gemLabel->setTextColor(Color4B(255, 150, 255, 255));
	roll1gemLabel->enableBold();
	roll1gemLabel->setPosition(Vec2(btn_x + 210.0f, btn_y - 15.0f));
	this->addChild(roll1gemLabel, static_cast<int>(ZOrder::Button));

	auto roll10gemLabel = Label::createWithTTF("90 Gems", "fonts/04B_03__.ttf", 8);
	roll10gemLabel->setTextColor(Color4B(255, 150, 255, 255));
	roll10gemLabel->enableBold();
	roll10gemLabel->setPosition(Vec2(btn_x + 290.0f, btn_y - 15.0f));
	this->addChild(roll10gemLabel, static_cast<int>(ZOrder::Button));

	// Button

	auto btnRoll1Gold = Button::create("UI/BTN.png", "UI/BTNHover.png");
	btnRoll1Gold->setTitleText("1");
	btnRoll1Gold->setPosition(Vec2(btn_x, btn_y));
	btnRoll1Gold->addClickEventListener([this](Ref *)
	{
		this->RndCard(false,true);
		_cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll1Gold, static_cast<int>(ZOrder::Button));

	auto btnRoll10Gold = Button::create("UI/BTN.png", "UI/BTNHover.png");
	btnRoll10Gold->setTitleText("10");
	btnRoll10Gold->setPosition(Vec2(btn_x + 80.0f, btn_y));
	btnRoll10Gold->addClickEventListener([this](Ref *)
										 {
		this->RndCard(false, false);
		_cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll10Gold, static_cast<int>(ZOrder::Button));

	auto btnRoll1Gem = Button::create("UI/BTN.png", "UI/BTNHover.png");
	btnRoll1Gem->setTitleText("1");
	btnRoll1Gem->setPosition(Vec2(btn_x + 210.0f, btn_y));
	btnRoll1Gem->addClickEventListener([this](Ref *)
									   {
		this->RndCard(true,true);
		_cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll1Gem, static_cast<int>(ZOrder::Button));

	auto btnRoll10Gem = Button::create("UI/BTN.png", "UI/BTNHover.png");
	btnRoll10Gem->setTitleText("10");
	btnRoll10Gem->setPosition(Vec2(btn_x + 290.0f, btn_y));
	btnRoll10Gem->addClickEventListener([this](Ref *)
										{
		this->RndCard(true, false);
		_cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll10Gem, static_cast<int>(ZOrder::Button));
}

int ShopScene::rndCost(bool isGem)
{
	int rnd = cocos2d::RandomHelper::random_int(1, 100);
	int cost = 1;
	if (isGem)
	{
		if (rnd > 85)
			cost = 5;
		else if (rnd > 60)
			cost = 4;
		else if (rnd > 30)
			cost = 3;
		else if (rnd > 10)
			cost = 2;
		else
			cost = 1;
	}
	else
	{
		if (rnd > 95)
			cost = 5;
		else if (rnd > 75)
			cost = 4;
		else if (rnd > 60)
			cost = 3;
		else if (rnd > 40)
			cost = 2;
		else
			cost = 1;
	}
	return cost;
}

void ShopScene::RndCard(bool isGem, bool rollOne)
{
	PlayerData p = _pController.loadPlayer();
	auto hud = dynamic_cast<PlayerHUD *>(this->getChildByName("HUD"));
	bool flag = false;
	int roll = 1;
	int size = 1;
	if (!hud)
	{
		CCLOG("Ko ket noi HUB .drawOneCard");
		return;
	}
	if (!rollOne)
	{
		roll = 9;
		size = 10;
	}
	if (isGem)
	{
		if (p.gems >= (10 * roll))
		{
			p.gems -= 10 * roll;
			_pController.updateGems(p.id, p.gems);
			hud->updatePlayerData(p);
			flag = true;
		}
		else
		{
			CCLOG("Ko du gem .drawOneCard");
			return;
		}
	}
	else
	{
		if (p.gold >= (100 * roll))
		{
			p.gold -= 100 * roll;
			_pController.updateGold(p.id, p.gold);
			hud->updatePlayerData(p);
			flag = true;
		}
		else
		{
			CCLOG("Ko du gold .drawOneCard");
			return;
		}
	}
	if (flag)
	{
		BattleCardData card;
		vector<BattleCardData> allCard;
		int cost;

		for (int i = 0; i < size; i++)
		{
			cost = rndCost(isGem);
			card = _cController.getRNDCard(cost);
			_cController.buyCard(p.id, card);
			allCard.push_back(card);
		}
		drawCard(allCard);
	}
}

void ShopScene::drawCard(vector<BattleCardData> allCard)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float CELLSHOP_W = 40.0f;
	float CELLSHOP_H = 55.0f;
	float padding = 10.0f;
	float x = origin.x + visibleSize.width * 0.12f;
	float y = origin.y + visibleSize.height * 0.6f;
	float firstCardX = x + 5.0f;
	float lastCardX = x + 5.0f + (CELLSHOP_W * (10 - 1));
	Vec2 originCard = Vec2(firstCardX - CELLSHOP_W / 2 - padding, y - CELLSHOP_H / 2 - padding);
	Vec2 destination = Vec2(lastCardX + CELLSHOP_W / 2 + padding, y + CELLSHOP_H / 2 + padding);

	auto bg = DrawNode::create();

	bg->drawSolidRect(originCard, destination, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
	_cardLayer->addChild(bg, static_cast<int>(ZOrder::Card));

	for (int i = 0; i < allCard.size(); i++)
	{
		auto card = CardNode::createNode(allCard[i], CELLSHOP_W, CELLSHOP_H);
		if (card)
		{
			if (allCard.size() == 1)
			{
				card->setPosition(Vec2((originCard.x + destination.x) / 2, (originCard.y + destination.y) / 2));
			}
			else
			{
				card->setPosition(Vec2(x + 5.0f + (CELLSHOP_W * i), y));
			}
			_cardLayer->addChild(card, static_cast<int>(ZOrder::Card));
		}
	}
	auto btnOk = Button::create("UI/Ok.png", "UI/OkHover.png");
	btnOk->setPosition(Vec2((originCard.x + destination.x) / 2, (originCard.y + destination.y) / 2 - 48.0f));
	btnOk->addClickEventListener([this](Ref *)
								 {
		_cardLayer->removeAllChildren();
		_cardLayer->setVisible(!_cardLayer->isVisible()); });
	_cardLayer->addChild(btnOk, static_cast<int>(ZOrder::Card));
}