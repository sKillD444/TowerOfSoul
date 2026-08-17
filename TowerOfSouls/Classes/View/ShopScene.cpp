#include "ShopScene.h"

USING_NS_CC;
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
	auto btnHome = Button::create("UI/BTN/BTN_Back.png");
	btnHome->setScale(Responsive::getScale(0.2f));
	btnHome->setAnchorPoint(Vec2(0.0f, 1.0f));
	btnHome->setPosition(Responsive::getPos(0.03f, 0.9f));
	btnHome->addClickEventListener([](Ref*) {
		auto scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
		});
	this->addChild(btnHome, static_cast<int>(ZOrder::Button));
}

void ShopScene::createGacha() {
	Vec2 goldCenter = Responsive::getPos(0.30f, 0.60f);
	Vec2 gemCenter = Responsive::getPos(0.70f, 0.60f);

	float btnY = Responsive::getPos(0.0f, 0.25f).y;
	float gapBtn = Responsive::getSize(55.0f);

	// GOLD
	auto chest_golds = Sprite::create("Items/Chest_Gold.png");
	chest_golds->setPosition(goldCenter);
	chest_golds->setScale(Responsive::getScale(6.0f));
	this->addChild(chest_golds, static_cast<int>(ZOrder::Button));

	auto goldLabel = Label::createWithTTF("Gold", "fonts/alagard.ttf", Responsive::getSize(20));
	goldLabel->setTextColor(Color4B::YELLOW);
	goldLabel->setPosition(Vec2(goldCenter.x, goldCenter.y + Responsive::getSize(60.0f)));
	this->addChild(goldLabel, static_cast<int>(ZOrder::Button));

	float goldX_1 = goldCenter.x - gapBtn;
	float goldX_5 = goldCenter.x;
	float goldX_10 = goldCenter.x + gapBtn;

	auto btnRoll1Gold = Button::create("UI/BTN/BTN1.png");
	btnRoll1Gold->setPosition(Vec2(goldX_1, btnY));
	btnRoll1Gold->setScale(Responsive::getScale(2.0f));
	btnRoll1Gold->addClickEventListener([this](Ref*) { this->RndCard(false, 1); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll1Gold, static_cast<int>(ZOrder::Button));
	auto roll1goldLabel = Label::createWithTTF("100 Gold", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll1goldLabel->setPosition(Vec2(goldX_1, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll1goldLabel, static_cast<int>(ZOrder::Button));

	auto btnRoll5Gold = Button::create("UI/BTN/BTN5.png");
	btnRoll5Gold->setPosition(Vec2(goldX_5, btnY));
	btnRoll5Gold->setScale(Responsive::getScale(2.0f));
	btnRoll5Gold->addClickEventListener([this](Ref*) { this->RndCard(false, 5); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll5Gold, static_cast<int>(ZOrder::Button));
	auto roll5goldLabel = Label::createWithTTF("450 Gold", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll5goldLabel->setPosition(Vec2(goldX_5, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll5goldLabel, static_cast<int>(ZOrder::Button));

	auto btnRoll10Gold = Button::create("UI/BTN/BTN10.png");
	btnRoll10Gold->setPosition(Vec2(goldX_10, btnY));
	btnRoll10Gold->setScale(Responsive::getScale(2.0f));
	btnRoll10Gold->addClickEventListener([this](Ref*) { this->RndCard(false, 10); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll10Gold, static_cast<int>(ZOrder::Button));
	auto roll10goldLabel = Label::createWithTTF("900 Gold", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll10goldLabel->setPosition(Vec2(goldX_10, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll10goldLabel, static_cast<int>(ZOrder::Button));

	// GEM
	auto chest_gems = Sprite::create("Items/Chest_Gem.png");
	chest_gems->setPosition(gemCenter);
	chest_gems->setScale(Responsive::getScale(5.0f));
	this->addChild(chest_gems, static_cast<int>(ZOrder::Button));

	auto gemLabel = Label::createWithTTF("Gem", "fonts/alagard.ttf", Responsive::getSize(20));
	gemLabel->setTextColor(Color4B(255, 150, 255, 255));
	gemLabel->setPosition(Vec2(gemCenter.x, gemCenter.y + Responsive::getSize(60.0f)));
	this->addChild(gemLabel, static_cast<int>(ZOrder::Button));

	float gemX_1 = gemCenter.x - gapBtn;
	float gemX_5 = gemCenter.x;
	float gemX_10 = gemCenter.x + gapBtn;

	auto btnRoll1Gem = Button::create("UI/BTN/BTN1.png");
	btnRoll1Gem->setPosition(Vec2(gemX_1, btnY));
	btnRoll1Gem->setScale(Responsive::getScale(2.0f));
	btnRoll1Gem->addClickEventListener([this](Ref*) { this->RndCard(true, 1); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll1Gem, static_cast<int>(ZOrder::Button));
	auto roll1gemLabel = Label::createWithTTF("10 Gems", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll1gemLabel->setPosition(Vec2(gemX_1, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll1gemLabel, static_cast<int>(ZOrder::Button));

	auto btnRoll5Gem = Button::create("UI/BTN/BTN5.png");
	btnRoll5Gem->setPosition(Vec2(gemX_5, btnY));
	btnRoll5Gem->setScale(Responsive::getScale(2.0f));
	btnRoll5Gem->addClickEventListener([this](Ref*) { this->RndCard(true, 5); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll5Gem, static_cast<int>(ZOrder::Button));
	auto roll5gemLabel = Label::createWithTTF("45 Gems", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll5gemLabel->setPosition(Vec2(gemX_5, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll5gemLabel, static_cast<int>(ZOrder::Button));

	auto btnRoll10Gem = Button::create("UI/BTN/BTN10.png");
	btnRoll10Gem->setPosition(Vec2(gemX_10, btnY));
	btnRoll10Gem->setScale(Responsive::getScale(2.0f));
	btnRoll10Gem->addClickEventListener([this](Ref*) { this->RndCard(true, 10); _cardLayer->setVisible(!_cardLayer->isVisible()); });
	this->addChild(btnRoll10Gem, static_cast<int>(ZOrder::Button));
	auto roll10gemLabel = Label::createWithTTF("90 Gems", "fonts/04B_03__.ttf", Responsive::getSize(8));
	roll10gemLabel->setPosition(Vec2(gemX_10, btnY - Responsive::getSize(25.0f)));
	this->addChild(roll10gemLabel, static_cast<int>(ZOrder::Button));
}

void ShopScene::drawCard(vector<BattleCardData> allCard) {
	float CELLSHOP_W = Responsive::getSize(40.0f);
	float CELLSHOP_H = Responsive::getSize(55.0f);
	float padding = Responsive::getSize(10.0f);
	float gap = Responsive::getSize(5.0f);

	int size = allCard.size();
	float totalWidth = (size * CELLSHOP_W) + ((size - 1) * gap);

	Vec2 centerPos = Responsive::getPos(0.5f, 0.6f);
	float startX = centerPos.x - totalWidth / 2.0f + CELLSHOP_W / 2.0f;
	float y = centerPos.y;

	Vec2 originCard = Vec2(startX - CELLSHOP_W / 2 - padding, y - CELLSHOP_H / 2 - padding);
	Vec2 destination = Vec2(startX + totalWidth - CELLSHOP_W / 2 + padding, y + CELLSHOP_H / 2 + padding);

	auto bg = DrawNode::create();
	bg->drawSolidRect(originCard, destination, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
	_cardLayer->addChild(bg, static_cast<int>(ZOrder::Card));

	for (int i = 0; i < size; i++) {
		auto card = CardNode::createNode(allCard[i], CELLSHOP_W, CELLSHOP_H);
		if (card) {
			card->setPosition(Vec2(startX + i * (CELLSHOP_W + gap), y));
			_cardLayer->addChild(card, static_cast<int>(ZOrder::Card));
		}
	}

	auto btnOk = Button::create("UI/BTN/BG_BTN.png");
	btnOk->setTitleText("OK");
	btnOk->setTitleFontName("fonts/04B_03__.ttf");
	btnOk->setTitleFontSize(Responsive::getSize(40));
	btnOk->setScale(Responsive::getScale(0.15f));
	btnOk->setPosition(Vec2((originCard.x + destination.x) / 2, (originCard.y + destination.y) / 2 - Responsive::getSize(48.0f)));
	btnOk->addClickEventListener([this](Ref*) {
		_cardLayer->removeAllChildren();
		_cardLayer->setVisible(false);
		});
	_cardLayer->addChild(btnOk, static_cast<int>(ZOrder::Card));
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

void ShopScene::RndCard(bool isGem, int rollCount)
{
	PlayerData p = _pController.loadPlayer();
	auto hud = dynamic_cast<PlayerHUD*>(this->getChildByName("HUD"));
	if (!hud)
	{
		CCLOG("Ko ket noi HUB");
		return;
	}

	int totalCost = 0;
	if (isGem) {
		if (rollCount == 1) totalCost = 10;
		else if (rollCount == 5) totalCost = 45;
		else if (rollCount == 10) totalCost = 90;
	}
	else {
		if (rollCount == 1) totalCost = 100;
		else if (rollCount == 5) totalCost = 450;
		else if (rollCount == 10) totalCost = 900;
	}

	if (isGem)
	{
		if (p.gems >= totalCost)
		{
			p.gems -= totalCost;
			_pController.updateGems(p.id, p.gems);
			hud->updatePlayerData(p);
		}
		else
		{
			CCLOG("Ko du gem");
			_cardLayer->setVisible(false);
			return;
		}
	}
	else
	{
		if (p.gold >= totalCost)
		{
			p.gold -= totalCost;
			_pController.updateGold(p.id, p.gold);
			hud->updatePlayerData(p);
		}
		else
		{
			CCLOG("Ko du gold");
			_cardLayer->setVisible(false);
			return;
		}
	}

	vector<int> costs;
	for (int i = 0; i < rollCount; i++)
	{
		costs.push_back(rndCost(isGem));
	}

	vector<BattleCardData> allCard = _cController.getRNDCards(costs);

	if (!allCard.empty()) {
		_cController.buyCards(p.id, allCard);
	}

	_cardLayer->removeAllChildren();
	drawCard(allCard);
}
