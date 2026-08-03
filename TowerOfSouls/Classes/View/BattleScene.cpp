#include "BattleScene.h"

USING_NS_CC;

namespace L {
	const Vec2 PlayerPos(200.0f, 230.0f);

	constexpr float CELL_W = 40.0f; // Chiều rộng slot
	constexpr float CELL_H = 40.0f; // Chiều cao slot
	constexpr float GAP = 0.5f;    // Khoảng cách giữa các slot

	constexpr float CELLSHOP_W = 40.0f;
	constexpr float CELLSHOP_H = 40.0f;
	constexpr float CELLSHOP_X = 150.0f;
	constexpr float CELLSHOP_Y = 165.0f;
	constexpr float GAPSHOP = 5.0f;
	const Vec2 OriginShop(100.0f, 115.0f);
	const Vec2 DestinationShop(390.0f, 200.0f);

	constexpr float BtnStart_X = 240.0f;
	constexpr float BtnStart_Y = 270.0f;

	constexpr float BtnShop_X = 450.0f;
	constexpr float BtnShop_Y = 50.0f;

	constexpr float BtnElement_X = 450.0f;
	constexpr float BtnElement_Y= 270.0f;
	constexpr float Element_Gap = 33.0f;

	constexpr float BtnConfirm_X = 450.0f;
	constexpr float BtnConfirm_Y = 130.0f;

	constexpr float SceneCenter_X = 240.0f;
	constexpr float SceneCenter_Y = 160.0f;

	const Vec2 OriginDeck(5.0f, 70.0f);
	const Vec2 DestinationDeck(100.0f, 320.0f);

	constexpr float Deck_X = 45.0f;
	constexpr float Deck_Y = 180.0f;
	constexpr float Deck_W = 160.0f;
	constexpr float Deck_H = 240.0f;

	const Vec2 OriginDelete(0.0f, 10.0f);
	const Vec2 DestinationDelete(100.0f, 70.0f);

	constexpr float CoinLabel_X = 440.0f;
	constexpr float CoinLabel_Y = 70.0f;
}

Scene* BattleScene::createScene(bool isCampaign, int stageId) {
	auto scene = new (nothrow) BattleScene();
	if (scene && scene->init(isCampaign, stageId)) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

bool BattleScene::init(bool isCampaign, int stageId) {
	if (!Scene::init()) return false;

	_isCampaign = isCampaign;
	_currentStageId = stageId;

	_maxWaves = (_currentStageId <= 4) ? 1 : 2;
	_currentWave = 1;

	int menuBgmId = UserDefault::getInstance()->getIntegerForKey("bgmId", -1);
	if (menuBgmId != -1) {
		cocos2d::experimental::AudioEngine::stop(menuBgmId);
		UserDefault::getInstance()->setIntegerForKey("bgmId", -1);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	_coins = 1;
	_uiLayer = Node::create();
	this->addChild(_uiLayer, static_cast<int>(ZOrder::UI), "uiLayer");
	this->createUI();

	_shopLayer = Node::create();
	this->addChild(_shopLayer, static_cast<int>(ZOrder::Shop), "shopLayer");
	this->createShop();
	_shopLayer->setVisible(false);

	if (_isCampaign) {
		_shopLayer->setVisible(false);
		if (auto btnShop = _uiLayer->getChildByName("btnShop")) btnShop->setVisible(false);
		if (auto iconCoin = _uiLayer->getChildByName("iconCoin")) iconCoin->setVisible(false);
		if (auto coinLabel = _uiLayer->getChildByName("coinLabel")) coinLabel->setVisible(false);
	}
	else {
		loadCardShop();
	}

	_settingLayer = Node::create();
	this->addChild(_settingLayer, static_cast<int>(ZOrder::Notification), "settingLayer");
	this->createSetting();
	_settingLayer->setVisible(false);

	_logBattleLayer = Node::create();
	this->addChild(_logBattleLayer, static_cast<int>(ZOrder::Notification), "logBattleLayer");
	this->createLogBattle();
	_logBattleLayer->setVisible(false);

	loadMap();

	this->createDeck();
	this->createBuyDialog();
	this->createArena(Vec2::ZERO);

	if (_isCampaign) {
		loadTeam();
		spawnCampaignEnemies(_currentStageId, _currentWave); 
	}
	else {
		this->createReinforcementSlots();
		this->spawnEnemies();
	}
	this->setUpTouchListener();
	return true;
}

void BattleScene::loadMap() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	vector<string> mapList = {
		"BG/Map/Forest1.png",    
		"BG/Map/Forest2.png",     
		"BG/Map/Forest3.png",     
		"BG/Map/Forest4.png",     
		"BG/Map/ElvenLand1.png",   
		"BG/Map/ElvenLand2.png",   
		"BG/Map/ElvenLand3.png",   
		"BG/Map/ElvenLand4.png",
		"BG/Map/Vampires1.png",
		"BG/Map/Vampires2.png",
		"BG/Map/Vampires3.png",
		"BG/Map/Vampires4.png"
	};

	string bgPath = mapList[0]; 

	if (_isCampaign) {
		int index = _currentStageId - 1;
		if (index >= 0 && index < mapList.size()) {
			bgPath = mapList[index];
		}
	}
	else {
		int currentFloor = (_round <= 0) ? 1 : _round;
		int mapIndex = ((currentFloor - 1) / 3) % mapList.size();
		bgPath = mapList[mapIndex];
	}

	auto bg = dynamic_cast<Sprite*>(this->getChildByName("bg"));

	if (bg) {
		bg->setTexture(bgPath);
	}
	else {
		bg = Sprite::create(bgPath);
		if (bg) {
			bg->setPosition(Vec2(centerX, centerY));
			this->addChild(bg, static_cast<int>(ZOrder::BackGround), "bg");
		}
	}

	if (bg) {
		float scaleX = visibleSize.width / bg->getContentSize().width;
		float scaleY = visibleSize.height / bg->getContentSize().height;
		bg->setScale(MAX(scaleX, scaleY));
	}
}

// Create UI

void BattleScene::createUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float topY = origin.y + visibleSize.height *0.5f;
	float botY = origin.y + visibleSize.height * 0.2f;
	float rightX = origin.x + visibleSize.width*0.9f;

	//Info
	_infoLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 16);
	_infoLabel->setPosition(Vec2(centerX, topY));
	_infoLabel->setColor(Color3B::WHITE);
	this->addChild(_infoLabel, static_cast<int>(ZOrder::Notification), "infoLabel");

	//Coin
	auto IconCoin = Sprite::create("Items/Silver.png");
	IconCoin->setPosition(Vec2(rightX - 15.0f, botY));
	IconCoin->setScale(0.05f);
	_uiLayer->addChild(IconCoin, static_cast<int>(ZOrder::UI), "iconCoin");

	_coinLabel = Label::createWithTTF(to_string(_coins), "fonts/Marker Felt.ttf", 7);
	_coinLabel->setPosition(Vec2(rightX, botY - 3.0f));
	_coinLabel->setColor(Color3B::YELLOW);
	_uiLayer->addChild(_coinLabel, static_cast<int>(ZOrder::UI), "coinLabel");

	//Start
	auto btnStart = Sprite::create("UI/BTN/btnPlay.png");
	btnStart->setScale(Responsive::getScale(0.15f));
	btnStart->setPosition(Responsive::getPos(0.45f, 0.90f));
	_uiLayer->addChild(btnStart, static_cast<int>(ZOrder::UI), "btnStart");

	//Setting
	auto btnSetting = Sprite::create("UI/BTN/btnSetting.png");
	btnSetting->setScale(Responsive::getScale(0.15f));
	btnSetting->setPosition(Responsive::getPos(0.5f, 0.90f));
	_uiLayer->addChild(btnSetting, static_cast<int>(ZOrder::UI), "btnSetting");

	//Shop
	auto btnShop = Sprite::create("UI/BTN/btnShop.png");
	btnShop->setPosition(Vec2(rightX, origin.y + visibleSize.height * 0.1f));
	btnShop->setScale(0.15f);
	_uiLayer->addChild(btnShop, static_cast<int>(ZOrder::Shop), "btnShop");

	//Element buff
	float elemStartY = origin.y + visibleSize.height * 0.9f;
	auto btnConfirmElement = Sprite::create("UI/BTN/BG_BTN2.png");
	btnConfirmElement->setPosition(Vec2(rightX, origin.y + visibleSize.height * 0.45f));
	btnConfirmElement->setScale(0.15f);
	auto labelOk = Label::createWithTTF("OK", "fonts/04B_03__.ttf", 80);
	auto btnSize = btnConfirmElement->getContentSize();
	labelOk->setPosition(Vec2(btnSize.width / 2.0f, btnSize.height / 2.0f));
	btnConfirmElement->addChild(labelOk);
	_uiLayer->addChild(btnConfirmElement, static_cast<int>(ZOrder::UI), "btnConfirmElement");

	vector<string> elements = { "Air", "Earth", "Water", "Fire" };
	for (size_t i = 0; i < elements.size(); ++i) {
		float yPos = elemStartY - i * L::Element_Gap;
		auto btnElem = Sprite::create("UI/BTN/" + elements[i] + ".png");
		btnElem->setPosition(Vec2(rightX, yPos));
		btnElem->setScale(0.06f);
		_uiLayer->addChild(btnElem, static_cast<int>(ZOrder::UI), "btn" + elements[i]);

		auto icon = Sprite::create("Items/Silver.png");
		icon->setPosition(Vec2(rightX - 30.0f, yPos));
		icon->setScale(0.05f);
		_uiLayer->addChild(icon, static_cast<int>(ZOrder::UI), "IconCoin" + elements[i]);

		auto coinLbl = Label::createWithTTF("7", "fonts/Marker Felt.ttf", 7);
		coinLbl->setPosition(Vec2(rightX - 20.0f, yPos));
		coinLbl->setColor(Color3B::YELLOW);
		_uiLayer->addChild(coinLbl, static_cast<int>(ZOrder::UI), "Coin" + elements[i]);
	}
}

void BattleScene::createArena(Vec2 ignoredPos) {
    boardSlots.clear();
    Vec2 arenaCenter = Responsive::getPos(0.55f, 0.40f);

    // Kích thước ô cờ phải được scale theo màn hình
    float scaledCellW = Responsive::getSize(L::CELL_W);
    float scaledCellH = Responsive::getSize(L::CELL_H);
    float scaledGap = Responsive::getSize(L::GAP);

    float totalW = 5 * scaledCellW + 4 * scaledGap;
    float totalH = 5 * scaledCellH + 4 * scaledGap;

    float startX = arenaCenter.x - totalW / 2.0f + scaledCellW / 2.0f;
    float startY = arenaCenter.y + totalH / 2.0f - scaledCellH / 2.0f;

    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            float x = startX + c * (scaledCellW + scaledGap);
            float y = startY - r * (scaledCellH + scaledGap);
            Vec2 center = Vec2(x, y);

            auto drawNode = DrawNode::create();
            Vec2 originSlot = Vec2(x - scaledCellW / 2.0f, y - scaledCellH / 2.0f);
            Vec2 destSlot = Vec2(x + scaledCellW / 2.0f, y + scaledCellH / 2.0f);

            drawNode->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.05f));
            drawNode->drawRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
            drawNode->setName("grid_draw_" + to_string(r) + "_" + to_string(c));
            this->addChild(drawNode, static_cast<int>(ZOrder::Slot));

            Slot slot;
            slot.area = Rect(originSlot.x, originSlot.y, scaledCellW, scaledCellH);
            slot.pos = center;
            slot.card = nullptr;
            slot.isEmpty = true;
            slot.isPlayerSide = false;
            boardSlots.push_back(slot);
        }
    }
}

void BattleScene::createDeck() {
	Vec2 deckCenter = Responsive::getPos(0.1f, 0.50f);

	float scaledCellW = Responsive::getSize(L::CELL_W);
	float scaledCellH = Responsive::getSize(L::CELL_H);

	float deckW = scaledCellW * 2.0f + Responsive::getSize(20.0f);
	float deckH = scaledCellH * 5.0f + Responsive::getSize(30.0f);

	auto drawBg = Sprite::create("BG/BGDeck.png");
	drawBg->setPosition(deckCenter);
	drawBg->setScaleX((deckW + Responsive::getSize(50.0f)) / drawBg->getContentSize().width);
	drawBg->setScaleY((deckH + Responsive::getSize(20.0f)) / drawBg->getContentSize().height);
	this->addChild(drawBg, static_cast<int>(ZOrder::Deck));

	auto drawDelete = DrawNode::create();
	drawDelete->setName("delete_draw");
	this->addChild(drawDelete, static_cast<int>(ZOrder::Deck));

	int index = 0;
	float startY = deckCenter.y + (deckH / 2.0f) - (scaledCellH / 2.0f) - Responsive::getSize(10.0f);

	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 2; c++) {
			float x = deckCenter.x + (c == 0 ? -1 : 1) * (scaledCellW / 2.0f + Responsive::getSize(2.0f));
			float y = startY - r * (scaledCellH + Responsive::getSize(2.0f));
			Vec2 center = Vec2(x, y);

			auto drawNode = DrawNode::create();
			Vec2 originSlot = Vec2(x - scaledCellW / 2.0f, y - scaledCellH / 2.0f);
			Vec2 destSlot = Vec2(x + scaledCellW / 2.0f, y + scaledCellH / 2.0f);

			drawNode->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
			drawNode->drawRect(originSlot, destSlot, Color4F(1, 1, 1, 0.3f));
			drawNode->setName("deck_draw_" + to_string(index));
			this->addChild(drawNode, static_cast<int>(ZOrder::Deck));

			Slot slot;
			slot.area = Rect(originSlot.x, originSlot.y, scaledCellW, scaledCellH);
			slot.card = nullptr;
			slot.isEmpty = true;
			slot.pos = center;
			deckSlots.push_back(slot);
			index++;
		}
	}
}

void BattleScene::createShop() {
	ShopSlots.clear();

	Vec2 originShop = Responsive::getPos(0.25f, 0.40f);
	float shopW = Responsive::getSize(300.0f);
	float shopH = Responsive::getSize(80.0f);
	Vec2 destShop(originShop.x + shopW, originShop.y + shopH);

	auto shopBg = DrawNode::create();
	shopBg->drawSolidRect(originShop, destShop, Color4F(0.0f, 0.0f, 0.0f, 0.95f));
	shopBg->drawRect(originShop, destShop, Color4F(0.8f, 0.6f, 0.2f, 1.0f));
	_shopLayer->addChild(shopBg, static_cast<int>(ZOrder::Shop));

	float availableW = shopW - Responsive::getSize(60.0f);
	float stepX = availableW / 5.0f;
	float startX = originShop.x + Responsive::getSize(20.0f) + stepX / 2.0f;
	float centerY = originShop.y + shopH / 2.0f;

	float cellShopW = Responsive::getSize(L::CELLSHOP_W);
	float cellShopH = Responsive::getSize(L::CELLSHOP_H);

	for (int i = 0; i < 5; i++) {
		float x = startX + i * stepX;
		Vec2 center = Vec2(x, centerY);

		auto drawNode = DrawNode::create();
		Vec2 originCell = Vec2(x - cellShopW / 2.0f, centerY - cellShopH / 2.0f);
		Vec2 destCell = Vec2(x + cellShopW / 2.0f, centerY + cellShopH / 2.0f);
		drawNode->drawSolidRect(originCell, destCell, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
		drawNode->drawRect(originCell, destCell, Color4F(1, 1, 1, 1));
		_shopLayer->addChild(drawNode, static_cast<int>(ZOrder::Shop));

		auto CostLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", Responsive::getSize(7));
		CostLabel->setPosition(Vec2(center.x, center.y - Responsive::getSize(30.0f)));
		CostLabel->setColor(Color3B::YELLOW);
		arrLabelCoin.push_back(CostLabel);
		_shopLayer->addChild(CostLabel, 10, "costLabel" + to_string(i));

		auto IconCoin = Sprite::create("Items/Silver.png");
		IconCoin->setPosition(Vec2(center.x - Responsive::getSize(12.0f), center.y - Responsive::getSize(28.0f)));
		IconCoin->setScale(Responsive::getScale(0.05f));
		arrIconCoin.push_back(IconCoin);
		_shopLayer->addChild(IconCoin, 10, "iconCoin" + to_string(i));

		Slot slot;
		slot.area = Rect(originCell.x, originCell.y, cellShopW, cellShopH);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		ShopSlots.push_back(slot);
	}

	float rollX = destShop.x - Responsive::getSize(10.0f);
	auto btnRoll = Sprite::create("UI/BTN/btnRoll.png");
	btnRoll->setPosition(Vec2(rollX, centerY - Responsive::getSize(25.0f)));
	btnRoll->setScale(Responsive::getScale(0.2f));
	_shopLayer->addChild(btnRoll, static_cast<int>(ZOrder::Shop), "btnRoll");

	auto RollCoinLabel = Label::createWithTTF("2", "fonts/Marker Felt.ttf", Responsive::getSize(7));
	RollCoinLabel->setPosition(Vec2(rollX, centerY - Responsive::getSize(10.0f)));
	RollCoinLabel->setColor(Color3B::YELLOW);
	_shopLayer->addChild(RollCoinLabel, static_cast<int>(ZOrder::Shop), "rollCoinLabel");

	auto IconCoinRoll = Sprite::create("Items/Silver.png");
	IconCoinRoll->setPosition(Vec2(rollX - Responsive::getSize(7.0f), centerY - Responsive::getSize(8.0f)));
	IconCoinRoll->setScale(Responsive::getScale(0.05f));
	_shopLayer->addChild(IconCoinRoll, static_cast<int>(ZOrder::Shop), "iconCoinRoll");
}

void BattleScene::createSetting() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	float boxW = 200.0f, boxH = 150.0f;
	Vec2 origSet(centerX - boxW / 2.0f, centerY - boxH / 2.0f);
	Vec2 destSet(centerX + boxW / 2.0f, centerY + boxH / 2.0f);

	auto settingBg = DrawNode::create();
	settingBg->drawSolidRect(origSet, destSet, Color4F(0.0f, 0.0f, 0.0f, 0.95f));
	settingBg->drawRect(origSet, destSet, Color4F(0.8f, 0.6f, 0.2f, 1.0f));
	_settingLayer->addChild(settingBg, static_cast<int>(ZOrder::Notification));

	auto resumeLabel = Label::createWithTTF("Resume", "fonts/alagard.ttf", 16);
	auto resumeItem = MenuItemLabel::create(resumeLabel, [this](Ref*) { _settingLayer->setVisible(false); });
	resumeItem->setPosition(Vec2(centerX, centerY + 30.0f));

	auto newGameLabel = Label::createWithTTF("New Game", "fonts/alagard.ttf", 16);
	auto newGameItem = MenuItemLabel::create(newGameLabel, [this](Ref*) { _settingLayer->setVisible(false); this->endGame(); });
	newGameItem->setPosition(Vec2(centerX, centerY));

	auto homeLabel = Label::createWithTTF("Home", "fonts/alagard.ttf", 16);
	auto homeItem = MenuItemLabel::create(homeLabel, [](Ref*) {
		auto scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		});
	homeItem->setPosition(Vec2(centerX, centerY - 30.0f));

	auto menu = Menu::create(resumeItem, newGameItem, homeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	_settingLayer->addChild(menu, static_cast<int>(ZOrder::Notification));
}

void BattleScene::createLogBattle() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setContentSize(Size(260, 120));
	scrollView->setPosition(Vec2(centerX, centerY));
	scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	scrollView->setBounceEnabled(true);
	scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	scrollView->setBackGroundColor(Color3B(20, 20, 20));
	scrollView->setBackGroundColorOpacity(230);

	float margin = 10.0f;
	_logLabel = Label::createWithTTF(_logBattle, "fonts/arial.ttf", 8);
	_logLabel->setColor(Color3B::WHITE);
	_logLabel->setDimensions(scrollView->getContentSize().width - margin * 2, 0);
	_logLabel->setAnchorPoint(Vec2(0.0f, 1.0f));

	_resultLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 16);
	_resultLabel->setColor(Color3B::WHITE);
	_resultLabel->setPosition(Vec2(centerX, centerY + 75.0f));
	_resultLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_logBattleLayer->addChild(_resultLabel, static_cast<int>(ZOrder::Notification));

	float innerHeight = MAX(_logLabel->getContentSize().height + margin * 2, scrollView->getContentSize().height);
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, innerHeight));
	_logLabel->setPosition(Vec2(margin, innerHeight - margin));
	scrollView->addChild(_logLabel);
	_logBattleLayer->addChild(scrollView, static_cast<int>(ZOrder::Notification), "infoScrollView");

	auto btnOk = Button::create("UI/BTN/BG_BTN.png");
	btnOk->setTitleText("OK");
	btnOk->setTitleFontName("fonts/04B_03__.ttf");
	btnOk->setTitleFontSize(30);
	btnOk->setScale(0.2f);
	btnOk->setPosition(Vec2(centerX, centerY - 75.0f));
	btnOk->setAnchorPoint(Vec2(0.5f, 0.5f));
	_logBattleLayer->addChild(btnOk, static_cast<int>(ZOrder::Notification), "btnOkLog");
}

void BattleScene::createBuyDialog() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	_buyDialog = Node::create();
	_buyDialog->setVisible(false);
	this->addChild(_buyDialog, (int)ZOrder::Notification);

	auto bg = DrawNode::create();
	bg->drawSolidRect(Vec2(centerX - 80, centerY - 60), Vec2(centerX + 80, centerY + 60), Color4F(0, 0, 0, 0.95f));
	bg->drawRect(Vec2(centerX - 80, centerY - 60), Vec2(centerX + 80, centerY + 60), Color4F(1, 0.8f, 0, 1));
	_buyDialog->addChild(bg);

	_quantityLabel = Label::createWithTTF("1", "fonts/Marker Felt.ttf", 18);
	_quantityLabel->setPosition(Vec2(centerX, centerY + 20));
	_buyDialog->addChild(_quantityLabel);

	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);

	auto plus = MenuItemLabel::create(Label::createWithTTF("+", "fonts/Marker Felt.ttf", 20), [this](Ref*) {
		if (_buyQuantity < 5) { _buyQuantity++; _quantityLabel->setString(to_string(_buyQuantity)); }
		});
	plus->setPosition(Vec2(centerX + 40, centerY + 20));
	menu->addChild(plus);

	auto minus = MenuItemLabel::create(Label::createWithTTF("-", "fonts/Marker Felt.ttf", 20), [this](Ref*) {
		if (_buyQuantity > 1) { _buyQuantity--; _quantityLabel->setString(to_string(_buyQuantity)); }
		});
	minus->setPosition(Vec2(centerX - 40, centerY + 20));
	menu->addChild(minus);

	auto buyBtn = MenuItemLabel::create(Label::createWithTTF("BUY", "fonts/Marker Felt.ttf", 18), [this](Ref*) {
		int totalCost = _buyQuantity * _tempCardData.cost;
		if (_coins >= totalCost) {
			bool isPlaced = false;
			for (auto& slotD : deckSlots) {
				if (slotD.isEmpty) {
					slotD.card = CardNode::createNode(_tempCardData, L::CELL_W, L::CELL_H);
					slotD.data = _tempCardData;
					slotD.data.star = _buyQuantity;
					slotD.card->setPosition(slotD.pos);
					this->addChild(slotD.card, static_cast<int>(ZOrder::Character));
					slotD.isEmpty = false;

					if (_buyQuantity > 1) {
						_controller.updateStar(slotD.data);
						slotD.card->upStar(slotD.data.star);
					}

					slotD.data.currentHp = slotD.data.hp;
					slotD.card->updateStats(slotD.data.currentHp, slotD.data.atk);

					isPlaced = true;
					break;
				}
			}
			if (isPlaced) {
				_coins -= totalCost;
				_coinLabel->setString(to_string(_coins));
				_buyDialog->setVisible(false);
			}
			else _infoLabel->setString("Deck da day!");
		}
		else _infoLabel->setString("Khong du coin!");
		_buyDialog->setVisible(false);
		});
	buyBtn->setPosition(Vec2(centerX, centerY - 25));
	menu->addChild(buyBtn);
	_buyDialog->addChild(menu);
}

void BattleScene::createReinforcementSlots() {
	Vec2 reinCenter = Responsive::getPos(0.27f, 0.2f);
	float scaledCellW = Responsive::getSize(L::CELL_W);
	float scaledCellH = Responsive::getSize(L::CELL_H);

	for (int i = 0; i < 2; i++) {
		float x = reinCenter.x + (i == 0 ? -1 : 1) * (scaledCellW / 2.0f + Responsive::getSize(1.5f));
		float y = reinCenter.y;
		Vec2 center = Vec2(x, y);

		auto drawNode = DrawNode::create();
		Vec2 originSlot = Vec2(x - scaledCellW / 2.0f, y - scaledCellH / 2.0f);
		Vec2 destSlot = Vec2(x + scaledCellW / 2.0f, y + scaledCellH / 2.0f);

		drawNode->drawSolidRect(originSlot, destSlot, Color4F(0.0f, 0.5f, 1.0f, 0.2f));
		drawNode->drawRect(originSlot, destSlot, Color4F(0.0f, 0.8f, 1.0f, 0.8f));
		drawNode->setName("reinforcement_draw_" + to_string(i));
		this->addChild(drawNode, static_cast<int>(ZOrder::Deck));

		Slot slot;
		slot.area = Rect(originSlot.x, originSlot.y, scaledCellW, scaledCellH);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		reinforcementSlots.push_back(slot);
	}
}

//Event Touch

void BattleScene::setUpTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMove, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool BattleScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 pos = touch->getLocation();

	if (!_shopLayer->isVisible() && !_settingLayer->isVisible() && !_buyDialog->isVisible()) {
		for (auto& slotR : reinforcementSlots) {
			if (!slotR.isEmpty && slotR.card && slotR.area.containsPoint(pos)) {
				selectedCard = slotR.card;
				selectedData = slotR.data;
				originalPos = slotR.pos;
				slotR.card = nullptr;
				slotR.isEmpty = true;
				selectedCard->setLocalZOrder(static_cast<int>(ZOrder::Card));
				selectedCard->setScale(selectedCard->getScale() * 1.2f);
				highLightLine(true);
				return true;
			}
		}
	}

	if (_isBattle) return false;

	auto btnStart = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnStart"));

	if (btnStart && btnStart->getBoundingBox().containsPoint(pos) && _btnPlayEnabled && !_shopLayer->isVisible() && !_settingLayer->isVisible() && !_buyDialog->isVisible()) {
		btnStart->setOpacity(200);
		if (countUnitsOnBoard() == 0) {
			_infoLabel->setString("Dat 1 card len san");
			auto delay = DelayTime::create(1.5f);
			auto sequence = Sequence::create(delay, CallFunc::create([this]() {
				_infoLabel->setString("");
				}), nullptr);
			this->runAction(sequence);
		}
		else {
			this->startNewRound();
			this->spawnHiddenEnemy();
			this->startBattle();
		}
		return true;
	}

	auto btnShop = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnShop"));
	if (btnShop && btnShop->getBoundingBox().containsPoint(pos) && _btnShopEnabled && !_settingLayer->isVisible() && !_buyDialog->isVisible()) {
		bool isOpening = !_shopLayer->isVisible();
		_shopLayer->setVisible(isOpening);
		btnShop->setOpacity(200);
		for (auto& slotS : ShopSlots) {
			if (!slotS.isEmpty && slotS.card != nullptr) {
				slotS.card->setVisible(isOpening);
				arrLabelCoin[&slotS - &ShopSlots[0]]->setVisible(isOpening);
				arrIconCoin[&slotS - &ShopSlots[0]]->setVisible(isOpening);
			}
		}
		return true;
	}

	auto btnSetting = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnSetting"));
	if (btnSetting && btnSetting->getBoundingBox().containsPoint(pos) && _btnSettingEnabled && !_shopLayer->isVisible() && !_buyDialog->isVisible()) {
		bool isOpening = !_settingLayer->isVisible();
		_settingLayer->setVisible(isOpening);
		btnSetting->setOpacity(200);
		return true;
	}

	auto handleElementSelect = [&](Sprite* btn, string elementStr) {
		if (btn && btn->getBoundingBox().containsPoint(pos) && _buffElement == "") {
			resetElementButtons();
			highlightElementBtn(btn);
			btn->setScale(0.07f);
			_selectedBuffElement = elementStr;

			if (elementStr == "Air") AudioEngine::play2d("Audio/Effect/DragonStudio_Air_Spell.mp3", false, 0.1f);
			else if (elementStr == "Earth") AudioEngine::play2d("Audio/Effect/DragonStudio_Earth_Spell.mp3", false, 0.1f);
			else if (elementStr == "Water") AudioEngine::play2d("Audio/Effect/DragonStudio_Water_Spell.mp3", false, 0.1f);
			else if (elementStr == "Fire") AudioEngine::play2d("Audio/Effect/DragonStudio_Fire_Spell.mp3", false, 0.1f);

			return true;
		}
		return false;
		};

	auto btnAir = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnAir"));
	if (handleElementSelect(btnAir, "Air")) return true;

	auto btnEarth = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnEarth"));
	if (handleElementSelect(btnEarth, "Earth")) return true;

	auto btnWater = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnWater"));
	if (handleElementSelect(btnWater, "Water")) return true;

	auto btnFire = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnFire"));
	if (handleElementSelect(btnFire, "Fire")) return true;

	auto btnConfirm = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnConfirmElement"));
	if (btnConfirm && btnConfirm->getBoundingBox().containsPoint(pos)) {
		AudioEngine::play2d("Audio/Effect/Click_2.mp3", false, 0.5f);
		if (_buffElement == "" && _selectedBuffElement != "") {

			bool isEnoughCurrency = false;
			int elementCost = 7;

			if (_isCampaign) {
				PlayerData p = _pController.loadPlayer();
				if (p.gold >= elementCost) {
					p.gold -= elementCost;
					_pController.updateGold(p.id, p.gold);
					isEnoughCurrency = true;
				}
			}
			else {
				if (_coins >= elementCost) {
					_coins -= elementCost;
					_coinLabel->setString(to_string(_coins));
					isEnoughCurrency = true;
				}
			}

			if (isEnoughCurrency) {
				_buffElement = _selectedBuffElement;

				if (_buffElement == "Air") _turnBuffElement = 1;
				else if (_buffElement == "Earth") _turnBuffElement = 1;
				else if (_buffElement == "Water") _turnBuffElement = 5;
				else if (_buffElement == "Fire") _turnBuffElement = 5;

				_infoLabel->setString("Da ap dung Element " + _buffElement);
				_selectedBuffElement = "";
				btnConfirm->setOpacity(150);

				auto delay = DelayTime::create(1.0f);
				this->runAction(Sequence::create(delay, CallFunc::create([this]() { _infoLabel->setString(""); }), nullptr));
			}
			else {
				_infoLabel->setString(_isCampaign ? "Khong du vang" : "Khong du bac");
				auto delay = DelayTime::create(1.0f);
				this->runAction(Sequence::create(delay, CallFunc::create([this]() { _infoLabel->setString(""); }), nullptr));
			}
		}
		return true;
	}

	//Shop
	if (_shopLayer->isVisible() && !_settingLayer->isVisible() && !_buyDialog->isVisible()) {
		//Roll
		auto btnRoll = dynamic_cast<Sprite*>(_shopLayer->getChildByName("btnRoll"));
		if (btnRoll && btnRoll->getBoundingBox().containsPoint(pos) && _btnShopEnabled) {
			if (_coins >= 2) {
				_coins -= 2;
				_coinLabel->setString(to_string(_coins));
				loadCardShop();
			}
			else {
				_infoLabel->setString("Khong du coin");
				auto delay = DelayTime::create(1.5f);
				auto squence = Sequence::create(delay, CallFunc::create([this]() {
					_infoLabel->setString("");
					}), nullptr);
				this->runAction(squence);
			}
			return true;
		}

		//Buy
		for (auto& slotShop : ShopSlots) {
			if (!slotShop.isEmpty && slotShop.card && slotShop.area.containsPoint(pos)) {
				_tempCardData = slotShop.data;
				_buyQuantity = 1;
				_quantityLabel->setString("1");
				_buyDialog->setVisible(true);
				return true;
			}
		}
	}

	if (!_shopLayer->isVisible() && !_settingLayer->isVisible() && !_buyDialog->isVisible()) {
		for (auto& slotD : deckSlots) {
			if (!slotD.isEmpty && slotD.card && slotD.area.containsPoint(pos)) {
				selectedCard = slotD.card;
				selectedData = slotD.data;
				originalPos = slotD.pos;
				slotD.card = nullptr;
				slotD.isEmpty = true;
				selectedCard->setLocalZOrder(static_cast<int>(ZOrder::Card));
				selectedCard->setScale(selectedCard->getScale() * 1.2f);
				highLightLine(true);
				return true;
			}
		}
	}

	for (auto& slot : boardSlots) {
		if (!slot.isEmpty && slot.card && slot.area.containsPoint(pos)) {
			if (slot.isPlayerSide == false) return false;
			selectedCard = slot.card;
			selectedData = slot.data;
			originalPos = slot.pos;
			slot.card = nullptr;
			slot.isEmpty = true;
			selectedCard->setLocalZOrder(static_cast<int>(ZOrder::Card));
			selectedCard->setScale(selectedCard->getScale() * 1.2f);
			highLightLine(true);
			return true;
		}
	}
	return false;
}

void BattleScene::onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (selectedCard) {
		selectedCard->setPosition(selectedCard->getPosition() + touch->getDelta());
	}
}

void BattleScene::onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto btnStart = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnStart"));
	if (btnStart) btnStart->setOpacity(255);

	auto btnShop = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnShop"));
	if (btnShop) btnShop->setOpacity(255);

	auto btnSetting = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnSetting"));
	if (btnSetting) btnSetting->setOpacity(255);

	auto btnAir = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnAir"));
	if (btnAir) btnAir->setScale(0.06f);
	auto btnEarth = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnEarth"));
	if (btnEarth) btnEarth->setScale(0.06f);
	auto btnWater = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnWater"));
	if (btnWater) btnWater->setScale(0.06f);
	auto btnFire = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnFire"));
	if (btnFire) btnFire->setScale(0.06f);

	if (!selectedCard) return;
	bool placed = false;
	highLightLine(false);

	if (!_isBattle && deleteCard(selectedCard, selectedData)) {
		selectedCard = nullptr;
		return;
	}

	Slot* select = nullptr;
	for (auto& slot : boardSlots) {
		if (originalPos == slot.pos) {
			select = &slot;
			break;
		}
	}
	if (select == nullptr) {
		for (auto& slot : deckSlots) {
			if (originalPos == slot.pos) {
				select = &slot;
				break;
			}
		}
	}

	if (select == nullptr) {
		for (auto& slotR : reinforcementSlots) {
			if (originalPos == slotR.pos) {
				select = &slotR;
				break;
			}
		}
	}

	if (select) {
		select->card = selectedCard;
		select->data = selectedData;
		select->isEmpty = false;
	}

	selectedCard->setScale(selectedCard->getScale() / 1.2f);
	Vec2 cardCenter = selectedCard->getPosition();

	for (auto& slot : boardSlots) {
		if (slot.area.containsPoint(cardCenter)) {
			if (select == &slot) {
				placed = false;
				break;
			}
			if (slot.isEmpty) {
				selectedCard->setPosition(slot.pos);
				slot.card = selectedCard;
				slot.data = selectedData;
				slot.isPlayerSide = true;
				if (select != nullptr && select != &slot) {
					select->card = nullptr;
					select->isEmpty = true;
				}
				slot.isEmpty = false;
				placed = true;

				if (_isBattle) {
					slot.card->updateStats(slot.data.currentHp, slot.data.atk + synergyAtkMult(slot.data.role));
				}
			}

			else if (slot.isPlayerSide == true && !_isBattle) {
				swapCard(*select, slot);
				slot.isEmpty = false;
				placed = true;
			}
			else {
				placed = false;
			}
			break;
		}
	}

	if (!placed && !_isBattle) {
		for (auto& slotD : deckSlots) {
			if (slotD.area.containsPoint(cardCenter)) {
				if (select == &slotD) {
					placed = false;
					break;
				}
				if (slotD.isEmpty) {
					selectedCard->setPosition(slotD.pos);
					slotD.card = selectedCard;
					slotD.data = selectedData;
					slotD.isPlayerSide = true;
					if (select != nullptr && select != &slotD) {
						select->card = nullptr;
						select->isEmpty = true;
					}
				}
				else {
					swapCard(*select, slotD);
				}
				slotD.isEmpty = false;
				placed = true;
				break;
			}
		}
	}

	if (!placed) {
		for (auto& slotR : reinforcementSlots) {
			if (slotR.area.containsPoint(cardCenter)) {
				if (select == &slotR) {
					placed = false;
					break;
				}
				if (slotR.isEmpty) {
					selectedCard->setPosition(slotR.pos);
					slotR.card = selectedCard;
					slotR.data = selectedData;
					slotR.isPlayerSide = true;
					if (select != nullptr && select != &slotR) {
						select->card = nullptr;
						select->isEmpty = true;
					}
				}
				else {
					swapCard(*select, slotR);
				}
				slotR.isEmpty = false;
				placed = true;
				break;
			}
		}
	}

	if (!placed) {
		auto moveBack = MoveTo::create(0.15f, originalPos);
		selectedCard->runAction(moveBack);

		bool isRestored = false;

		for (auto& slotD : deckSlots) {
			if (slotD.pos == originalPos) {
				slotD.card = selectedCard;
				slotD.isEmpty = false;
				slotD.isPlayerSide = true;
				isRestored = true;
				break;
			}
		}

		if (!isRestored) {
			for (auto& slotR : reinforcementSlots) {
				if (slotR.pos == originalPos) {
					slotR.card = selectedCard;
					slotR.isEmpty = false;
					slotR.isPlayerSide = true;
					isRestored = true;
					break;
				}
			}
		}

		if (!isRestored) {
			for (auto& slot : boardSlots) {
				if (slot.pos == originalPos) {
					slot.card = selectedCard;
					slot.isEmpty = false;
					slot.isPlayerSide = true;
					break;
				}
			}
		}
	}
	selectedCard->setLocalZOrder(static_cast<int>(ZOrder::Character));
	selectedCard = nullptr;
}

//Game Logic
void BattleScene::startNewRound() {
	if(!_isCampaign){
		_infoLabel->setString("Round " + to_string(_round));
		_round++;
		if ((_round - 1) % 3 == 0) {
			loadMap();
		}
	}
	auto delay = DelayTime::create(1.5f);
	auto squence = Sequence::create(delay, CallFunc::create([this]() {
		_infoLabel->setString("");
		}), nullptr);
	this->runAction(squence);
}

void BattleScene::startBattle() {
	_btnShopEnabled = false;
	_btnPlayEnabled = false;
	_isBattle = true;
	_shopLayer->setVisible(false);

	for (auto& slotS : ShopSlots) {
		if (!slotS.isEmpty && slotS.card != nullptr) {
			slotS.card->setVisible(false);
			arrLabelCoin[&slotS - &ShopSlots[0]]->setVisible(false);
			arrIconCoin[&slotS - &ShopSlots[0]]->setVisible(false);
		}
	}

	for (auto& p : boardSlots) {
		if (!p.isEmpty && p.card && p.isPlayerSide == true) {
			p.data.currentHp = p.data.hp;
			float dmg = p.data.atk;
			dmg += synergyAtkMult(p.data.role);
			p.card->updateStats(p.data.currentHp, dmg);
		}
	}

	for (auto& e : boardSlots) {
		if (!e.isEmpty && e.card && e.isPlayerSide == false) {
			e.data.currentHp = e.data.hp;;
			float dmg = e.data.atk;
			dmg += e.data.atk * (_round / 10);
			e.card->updateStats(e.data.currentHp, dmg);
		}
	}

	string elementBuff = "Element: " + _buffElement + "\n\n";
	_logBattle += elementBuff;
	this->scheduleUpdate();
}

void BattleScene::endBattle() {
	_btnShopEnabled = true;
	_btnPlayEnabled = true;
	_isBattle = false;
	_turnBuffElement = 0;
	_buffElement = "";

	_infoLabel->setString("");

	vector<string> btnNames = { "btnAir", "btnEarth", "btnWater", "btnFire" };
	for (auto name : btnNames) {
		auto btn = dynamic_cast<Sprite*>(_uiLayer->getChildByName(name));
		if (btn) {
			btn->removeChildByName("glowEffect");
		}
	}

	for (auto& slot : boardSlots) {
		if (slot.isPlayerSide == false) {
			if (slot.card != nullptr) {
				slot.card->removeFromParent();
				slot.card = nullptr;
			}
			slot.isEmpty = true;
			slot.data = BattleCardData();
		}
		else {
			if (slot.isEmpty || slot.data.currentHp <= 0) {
				if (slot.card != nullptr) {
					slot.card->removeFromParent();
					slot.card = nullptr;
				}
				slot.isEmpty = true;
				slot.data = BattleCardData();
			}
		}
	}

	for (auto& p : boardSlots) {
		if (!p.isEmpty && p.card && p.isPlayerSide == true) {
			p.card->updateStats(p.data.hp, p.data.atk);
		}
	}

	if (!_isCampaign) {
		_coins += 3;
		_coinLabel->setString(to_string(_coins));
		_shopLayer->setVisible(true);
		loadCardShop();
		spawnEnemies();
	}
}

void BattleScene::endGame() {
	if (selectedCard != nullptr) {
		selectedCard->setLocalZOrder(static_cast<int>(ZOrder::Character));
		selectedCard = nullptr;
	}

	_btnShopEnabled = true;
	_btnPlayEnabled = true;
	_isBattle = false;
	_turnBuffElement = 0;
	_buffElement = "";

	vector<string> btnNames = { "btnAir", "btnEarth", "btnWater", "btnFire" };
	for (auto name : btnNames) {
		auto btn = dynamic_cast<Sprite*>(_uiLayer->getChildByName(name));
		if (btn) {
			btn->removeChildByName("glowEffect");
		}
	}

	_infoLabel->setString("");

	if (_isCampaign) {
		auto scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		return;
	}
	else
	{
		_shopLayer->setVisible(true);
		this->unscheduleUpdate();
		PlayerData p = _pController.loadPlayer();
		int gold = 0;
		if (_round >= 30)
			gold = 100;
		else if (_round >= 25)
			gold = 70;
		else if (_round >= 20)
			gold = 50;
		else if (_round >= 15)
			gold = 25;
		else if (_round >= 10)
			gold = 20;
		else if (_round >= 5)
			gold = 10;
		p.gold += gold;
		_pController.updateGold(p.id, p.gold);
		_lController.updateLeaderboard(p.id, _round);

		_currentAttackerIndex = 0;
		_coins = 5;
		_coinLabel->setString(to_string(_coins));
		_round = 0;


		for (auto& slot : boardSlots) {
			slot.isEmpty = true;
			if (slot.card != nullptr) {
				slot.card->removeFromParent();
				slot.card = nullptr;
			}
			slot.data = BattleCardData();
		}
		for (auto& slot : deckSlots) {
			slot.isEmpty = true;
			if (slot.card != nullptr) {
				slot.card->removeFromParent();
				slot.card = nullptr;
			}
			slot.data = BattleCardData();
		}

		for (auto& slot : reinforcementSlots) {
			slot.isEmpty = true;
			if (slot.card != nullptr) {
				slot.card->removeFromParent();
				slot.card = nullptr;
			}
			slot.data = BattleCardData();
		}

		loadCardShop();
		spawnEnemies();
	}
}

//Card Management
int BattleScene::countUnitsOnBoard() {
	int count = 0;
	for (const auto& slot : boardSlots) {
		if (!slot.isEmpty && slot.isPlayerSide == true) count++;
	}
	return count;
}

int BattleScene::countCard(BattleCardData data) {
	int count = 0;
	for (auto slot : boardSlots) {
		if (!slot.isEmpty && slot.isPlayerSide == true && slot.card != nullptr && slot.data.card_id == data.card_id && slot.data.star == data.star)
			count++;
	}
	for (auto slot : deckSlots) {
		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == data.card_id && slot.data.star == data.star)
			count++;
	}
	return count;
}

//void BattleScene::mergeCard(Slot& card) {
//	string targetId = card.data.card_id;
//	int targetStar = card.data.star;
//	if (countCard(card.data) < 3)return;
//	bool flag = true;
//	int count = 0;
//	Slot* upgradedSlot = nullptr;
//	for (auto& slot : playerSlots) {
//		if (count > 2)break;
//		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == targetId && slot.data.star == targetStar) {
//			if (flag) {
//				slot.data.star++;
//				_controller.updateStar(slot.data);
//				slot.card->updateStats(slot.data.currentHp, slot.data.atk);
//				slot.card->upStar(slot.data.star);
//				count++;
//				upgradedSlot = &slot;
//				flag = false;
//			}
//			else {
//				slot.card->removeFromParentAndCleanup(true);
//				slot.card = nullptr;
//				slot.isEmpty = true;
//				count++;
//			}
//		}
//	}
//	for (auto& slot : deckSlots) {
//		if (count > 2)break;
//		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == targetId && slot.data.star == targetStar) {
//			if (flag) {
//				slot.data.star++;
//				_controller.updateStar(slot.data);
//				slot.card->updateStats(slot.data.currentHp, slot.data.atk);
//				slot.card->upStar(slot.data.star);
//				count++;
//				upgradedSlot = &slot;
//				flag = false;
//			}
//			else {
//				slot.card->removeFromParentAndCleanup(true);
//				slot.card = nullptr;
//				slot.isEmpty = true;
//				count++;
//			}
//		}
//	}
//	if (upgradedSlot != nullptr && upgradedSlot->data.star < 3) {
//		mergeCard(*upgradedSlot);
//	}
//}

bool BattleScene::deleteCard(CardNode* select, BattleCardData data) {
	Vec2 deckCenter = Responsive::getPos(0.12f, 0.0f);
	float zoneW = Responsive::getSize(90.0f);
	float zoneH = Responsive::getSize(45.0f);

	Rect deleteZone(deckCenter.x - zoneW / 2.0f, deckCenter.y, zoneW, zoneH);

	if (deleteZone.containsPoint(select->getPosition())) {
		_coins += data.cost;
		_coinLabel->setString(to_string(_coins));
		select->removeFromParentAndCleanup(true);
		return true;
	}
	return false;
}

void BattleScene::swapCard(Slot& selectCard, Slot& card) {
	swap(selectCard.data, card.data);
	swap(selectCard.card, card.card);
	auto selectCardMove = MoveTo::create(0.3f, selectCard.pos);
	auto cardMove = MoveTo::create(0.3f, card.pos);
	selectCard.card->runAction(selectCardMove);
	card.card->runAction(cardMove);
}

//Battle Logic
void BattleScene::loadCardShop() {
	for (auto& slotS : ShopSlots) {
		if (!slotS.isEmpty && slotS.card != nullptr) {
			slotS.card->removeFromParent();
			slotS.card = nullptr;
			slotS.isEmpty = true;
		}
	}

	auto shopData = _controller.getShopRoll(_round);
	for (size_t i = 0; i < shopData.size() && i < ShopSlots.size(); i++) {
		auto& slotS = ShopSlots[i];
		arrLabelCoin[i]->setString(to_string(shopData[i].cost));
		arrLabelCoin[i]->setVisible(_shopLayer->isVisible());
		arrIconCoin[i]->setVisible(_shopLayer->isVisible());
		slotS.card = CardNode::createNode(shopData[i], L::CELLSHOP_W, L::CELLSHOP_H);
		if (slotS.card != nullptr) {
			slotS.card->setPosition(slotS.pos);
			this->addChild(slotS.card, static_cast<int>(ZOrder::Shop));
			slotS.card->setVisible(_shopLayer->isVisible());
			slotS.isEmpty = false;
			slotS.data = shopData[i];
		}
	}
}

void BattleScene::highLightLine(bool flag) {
	for (size_t i = 0; i < deckSlots.size(); i++) {
		string str = "deck_draw_" + to_string(i);
		auto drawDeck = dynamic_cast<DrawNode*>(this->getChildByName(str));
		if (drawDeck != nullptr) {
			drawDeck->clear();

			Vec2 center = deckSlots[i].pos;
			Vec2 originSlot = Vec2(center.x - L::CELL_W / 2.0f, center.y - L::CELL_H / 2.0f);
			Vec2 destSlot = Vec2(center.x + L::CELL_W / 2.0f, center.y + L::CELL_H / 2.0f);

			if (flag) {
				drawDeck->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
				drawDeck->drawRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else {
				drawDeck->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
				drawDeck->drawRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
			}
		}
	}

	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			int idx = r * 5 + c;
			if (idx >= boardSlots.size()) continue;

			string str = "grid_draw_" + to_string(r) + "_" + to_string(c);
			auto drawGrid = dynamic_cast<DrawNode*>(this->getChildByName(str));
			if (drawGrid != nullptr) {
				drawGrid->clear();

				Vec2 center = boardSlots[idx].pos;
				Vec2 originSlot = Vec2(center.x - L::CELL_W / 2.0f, center.y - L::CELL_H / 2.0f);
				Vec2 destSlot = Vec2(center.x + L::CELL_W / 2.0f, center.y + L::CELL_H / 2.0f);

				if (flag) {
					drawGrid->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.05f));
					drawGrid->drawRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else {
					drawGrid->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.05f));
					drawGrid->drawRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
				}
			}
		}
	}

	auto drawDelete = dynamic_cast<DrawNode*>(this->getChildByName("delete_draw"));
	if (drawDelete != nullptr) {
		drawDelete->clear();
		if (flag) {
			auto visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();

			float deckCenterX = origin.x + visibleSize.width * 0.10f;
			Vec2 delOrig(deckCenterX - 45.0f, origin.y + 10.0f);
			Vec2 delDest(deckCenterX + 45.0f, origin.y + 45.0f);

			drawDelete->drawSolidRect(delOrig, delDest, Color4F(1.0f, 0.0f, 0.0f, 0.5f));
			drawDelete->drawRect(delOrig, delDest, Color4F(1.0f, 0.2f, 0.2f, 1.0f));
		}
	}
}

void BattleScene::spawnEnemies() {

	for (auto& slotE : boardSlots) {
		if (!slotE.isEmpty && slotE.card != nullptr && slotE.isPlayerSide == false) {
			slotE.card->removeFromParent();
			slotE.card = nullptr;
			slotE.isEmpty = true;
			slotE.data = BattleCardData();
		}
	}

	vector<int> emptySlotIndices;
	for (int i = 0; i < boardSlots.size(); i++) {
		if (boardSlots[i].isEmpty) {
			emptySlotIndices.push_back(i);
		}
	}

	int count = 1 + (_round / 2);
	int upStar = 1 + (_round / 7);
	if (count > 15)
		count = 15;
	auto enemies = _controller.spawnEnemies(count);
	for (size_t i = 0; i < enemies.size(); i++) {
		if (emptySlotIndices.empty()) break;
		int randomIndex = cocos2d::RandomHelper::random_int(0, (int)emptySlotIndices.size() - 1);

		int targetSlotIndex = emptySlotIndices[randomIndex];
		emptySlotIndices.erase(emptySlotIndices.begin() + randomIndex);

		auto& slotE = boardSlots[targetSlotIndex];
		slotE.card = CardNode::createNode(enemies[i], L::CELL_W, L::CELL_H);
		if (slotE.card != nullptr) {
			if (upStar > 1) {
				enemies[i].star = upStar;
				_controller.updateStar(enemies[i]);
				slotE.card->upStar(upStar);
				slotE.card->updateStats(enemies[i].currentHp, enemies[i].atk);
			}
			slotE.card->setPosition(slotE.pos);
			this->addChild(slotE.card, static_cast<int>(ZOrder::Character));
			slotE.isEmpty = false;
			slotE.isPlayerSide = false;
			slotE.data = enemies[i];
		}
	}
}

void BattleScene::spawnHiddenEnemy() {
	if (_isCampaign) return;

	vector<int> emptySlotIndices;
	for (int i = 0; i < boardSlots.size(); i++) {
		if (boardSlots[i].isEmpty) {
			emptySlotIndices.push_back(i);
		}
	}

	if (emptySlotIndices.empty()) return;

	int hiddenCount = 1 + (_round / 5);
	int spawnAmount = min(hiddenCount, (int)emptySlotIndices.size());

	auto enemies = _controller.spawnEnemies(spawnAmount);
	if (enemies.empty()) return;

	int upStar = 1 + (_round / 7);
	int actuallySpawned = 0;

	for (size_t i = 0; i < enemies.size(); i++) {
		if (emptySlotIndices.empty()) break;

		int randomIndex = cocos2d::RandomHelper::random_int(0, (int)emptySlotIndices.size() - 1);
		int targetSlotIndex = emptySlotIndices[randomIndex];
		emptySlotIndices.erase(emptySlotIndices.begin() + randomIndex);

		auto& slotE = boardSlots[targetSlotIndex];

		slotE.card = CardNode::createNode(enemies[i], L::CELL_W, L::CELL_H);
		if (slotE.card != nullptr) {
			if (upStar > 1) {
				enemies[i].star = upStar;
				_controller.updateStar(enemies[i]);
				slotE.card->upStar(upStar);
				slotE.card->updateStats(enemies[i].currentHp, enemies[i].atk);
			}

			slotE.card->setPosition(slotE.pos);
			slotE.card->setOpacity(0);
			auto fadeIn = FadeIn::create(1.0f);
			slotE.card->runAction(fadeIn);

			this->addChild(slotE.card, static_cast<int>(ZOrder::Character));
			slotE.isEmpty = false;
			slotE.isPlayerSide = false;
			slotE.data = enemies[i];

			actuallySpawned++;
		}
	}
	if (actuallySpawned > 0) {
		_infoLabel->setString("Spawn " + to_string(actuallySpawned) + " hidden enemies!");
	}
}

BattleScene::Slot* BattleScene::findTarget(vector<Slot*>& targetSlots, Slot* attacker) {
	Slot* bestTarget = nullptr;
	float bestDist = 99999.0f;

	if (!attacker || !attacker->card) return nullptr;
	Vec2 attackerPos = attacker->pos;

	for (auto* slot : targetSlots) {
		if (!slot->isEmpty && slot->card != nullptr && slot->data.currentHp > 0) {
			float dist = attackerPos.distance(slot->pos);
			if (dist < bestDist) {
				bestDist = dist;
				bestTarget = slot;
			}
		}
	}
	return bestTarget;
}

float BattleScene::synergyAtkMult(string role) {
	int dem = 0;
	for (auto& p : boardSlots) {
		if (p.data.role == role && p.isPlayerSide == true)
			dem++;
	}
	if (dem > 5)
		return 25;
	else if (dem > 4)
		return 15;
	else if (dem > 3)
		return 10;
	else if (dem > 2)
		return 5;
	return 0;
}

float BattleScene::culateAllDamage(Slot& attacker, Slot& target, bool isPlayerAttacking) {
	float dmg = _controller.culateDamage(attacker.data, target.data);

	int atkRow = -1, atkCol = -1;
	int tgtRow = -1, tgtCol = -1;

	for (int r = 0; r < 5; r++) {
		for (int c = 0; c < 5; c++) {
			int idx = r * 5 + c;
			if (&boardSlots[idx] == &attacker) { atkRow = r; atkCol = c; }
			if (&boardSlots[idx] == &target) { tgtRow = r; tgtCol = c; }
		}
	}

	int gridDistance = 1;
	if (atkRow != -1 && tgtRow != -1) {
		gridDistance = abs(atkRow - tgtRow) + abs(atkCol - tgtCol);
	}

	string role = attacker.data.role;

	bool check = (isPlayerAttacking && _buffElement == "Air");

	float penalty = 0.0f;
	if (role == "Ranged DPS") {
		dmg = dmg * 1.0f;
	}
	else if (role == "Assassin") {
		if (gridDistance <= 1) {
			dmg *= 1.10f;
		}
		else {
			if (!check) {
				penalty = (gridDistance - 1) * 0.25f;
				if (penalty > 0.75f) penalty = 0.75f;
				dmg *= (1.0f - penalty);
			}
		}
	}
	else {
		if (gridDistance > 1) {
			if (!check) {
				penalty = (gridDistance - 1) * 0.15f;
				if (penalty > 0.60f) penalty = 0.60f;
				dmg *= (1.0f - penalty);
			}
		}
	}

	if (isPlayerAttacking) {
		dmg += synergyAtkMult(attacker.data.role);
	}
	else {
		dmg += dmg * (_round / 10.0f);
	}

	return max(1.0f, dmg);
}

void BattleScene::doAttack(Slot& attacker, Slot& target, bool isPlayerAttacking) {
	if (!attacker.card || !target.card) return;

	float dmg = culateAllDamage(attacker, target, isPlayerAttacking);

	target.data.currentHp -= dmg;


	target.card->updateStats(target.data.currentHp, target.data.atk);

	attacker.card->playLungeAnimation(target.pos, attacker.pos);
	target.card->playHitAnimation(target.pos);

	auto dmgLabel = Label::createWithTTF("-" + to_string((int)dmg), "fonts/Marker Felt.ttf", 16);
	if (dmgLabel) {
		dmgLabel->setPosition(target.pos + Vec2(0, 20));
		dmgLabel->setTextColor(Color4B::RED);
		dmgLabel->enableOutline(Color4B::BLACK, 1);
		this->addChild(dmgLabel, static_cast<int>(ZOrder::Notification));

		auto floatUp = MoveBy::create(0.7f, Vec2(0, 24));
		auto fadeOut = FadeOut::create(0.7f);
		dmgLabel->runAction(Sequence::create(
			Spawn::create(floatUp, fadeOut, nullptr),
			RemoveSelf::create(),
			nullptr
		));
	}

	const char* side = isPlayerAttacking ? "PLAYER" : "ENEMY";
	string log = string("[") + side + "] "
		+ attacker.data.name + " danh "
		+ target.data.name + ": Mat "
		+ to_string((int)dmg) + " mau. Dich con "
		+ to_string((int)target.data.currentHp) + " mau!\n\n";
	_logBattle += log;
}

void BattleScene::buffElement(string type) {
	if (type == "Fire") {
		for (auto& e : boardSlots) {
			if (!e.isEmpty && e.card && e.isPlayerSide == false) {
				e.data.currentHp -= e.data.hp * 0.05f;
				e.card->updateStats(e.data.currentHp, e.data.atk);
			}
		}
	}
	else if (type == "Water") {
		for (auto& p : boardSlots) {
			if (!p.isEmpty && p.card && p.isPlayerSide == true) {
				p.data.currentHp += p.data.hp * 0.05f;
				p.card->updateStats(p.data.currentHp, p.data.atk);
			}
		}
	}
	else if (type == "Earth") {
		for (auto& p : boardSlots) {
			if (!p.isEmpty && p.card && p.isPlayerSide == true) {
				p.data.currentHp += p.data.hp * 0.20f;
				p.card->updateStats(p.data.currentHp, p.data.atk);
			}
		}
	}
}

void BattleScene::resetElementButtons() {
	vector<string> btnNames = { "btnAir", "btnEarth", "btnWater", "btnFire" };
	for (auto name : btnNames) {
		auto btn = dynamic_cast<Sprite*>(_uiLayer->getChildByName(name));
		if (btn) {
			btn->setScale(0.06f);
			btn->removeChildByName("glowEffect");
		}
	}
}

void BattleScene::update(float dt) {
	if (!_isBattle) return;

	int alivePlayers = 0;
	int aliveEnemys = 0;

	for (auto& p : boardSlots) {
		if (!p.isEmpty && p.isPlayerSide == true) {
			if (p.data.currentHp <= 0) {
				p.card->playDeathAnimation(L::CELL_W, L::CELL_H);
				p.card = nullptr;
				p.isEmpty = true;
			}
			else
				alivePlayers++;
		}
	}

	for (auto& e : boardSlots) {
		if (!e.isEmpty && e.isPlayerSide == false) {
			if (e.data.currentHp <= 0) {
				e.card->playDeathAnimation(L::CELL_W, L::CELL_H);
				e.card = nullptr;
				e.isEmpty = true;
			}
			else
				aliveEnemys++;
		}
	}

	auto state = _controller.checkBattleState(alivePlayers, aliveEnemys);
	if (state == BattleState::WIN) {
		this->unscheduleUpdate();

		if (_isCampaign && _currentWave < _maxWaves) {
			_currentWave++;
			_infoLabel->setString("WAVE " + to_string(_currentWave));

			auto delay = DelayTime::create(1.5f);
			auto sequence = Sequence::create(delay, CallFunc::create([this]() {
				_infoLabel->setString("");
				this->endBattle();
				this->spawnCampaignEnemies(_currentStageId, _currentWave);
				}), nullptr);
			this->runAction(sequence);
			return;
		}

		_currentAttackerIndex = 0;
		_logLabel->setString(_logBattle);
		_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
		_resultLabel->setString("WIN");
		auto btn = dynamic_cast<cocos2d::ui::Button*>(_logBattleLayer->getChildByName("btnOkLog"));
		if (btn) {
			btn->addClickEventListener([this](Ref*) {
				_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
				_logBattle = "";
				_infoLabel->setString("");
				_resultLabel->setString("");
				_logLabel->setString(_logBattle);

				if (_isCampaign) {
					PlayerData p = _pController.loadPlayer();

					int rewardGold = _controller.getStageGoldReward(_currentStageId);
					p.gold += rewardGold;
					_pController.updateGold(p.id, p.gold);

					int rewardGems = 0;
					if (_currentStageId == 4) rewardGems = 20;
					else if (_currentStageId == 8) rewardGems = 40;
					else if (_currentStageId == 12) rewardGems = 60;

					if (rewardGems > 0) {
						p.gems += rewardGems;
						_pController.updateGems(p.id, p.gems); 
					}

					if (p.current_stage <= _currentStageId) {
						_pController.updateCurrentStage(p.id, _currentStageId + 1);
					}

					auto scene = CampaignScene::createScene();
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
				}
				else {
					this->endBattle();
				}
				});
		}
		return;
	}
	else if (state == BattleState::LOSE) {
		this->unscheduleUpdate();
		_logLabel->setString(_logBattle);
		_resultLabel->setString("LOSE");
		_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
		auto btn = dynamic_cast<cocos2d::ui::Button*>(_logBattleLayer->getChildByName("btnOkLog"));
		if (btn) {
			btn->addClickEventListener([this](Ref*) {
				_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
				_logBattle = "";
				_infoLabel->setString("");
				_resultLabel->setString("");
				_logLabel->setString(_logBattle);
				this->endGame();
				});
		}
		return;
	}

	_turnTimer -= dt;
	if (_turnTimer > 0) return;

	if (_turnBuffElement > 0) {
		buffElement(_buffElement);
		_turnBuffElement--;
	}
	else if (_buffElement != "Air") {
		_buffElement = "";
	}

	vector<CombatAction> tempQueue;
	{
		lock_guard<mutex> lock(_mtx);
		if (!_actionQueue.empty()) {
			tempQueue = _actionQueue;
			_actionQueue.clear();
		}
	}

	if (!tempQueue.empty()) {
		playCombatAnimations(tempQueue);
		_turnTimer = 1.2f;
	}
	else {
		startCombatCalculation();
		_turnTimer = 0.1f;
	}
}

void BattleScene::highlightElementBtn(cocos2d::Sprite* selectedBtn) {
	if (selectedBtn) {
		auto glow = Sprite::createWithTexture(selectedBtn->getTexture());
		glow->setPosition(Vec2(selectedBtn->getContentSize().width / 2, selectedBtn->getContentSize().height / 2));
		glow->setBlendFunc(BlendFunc::ADDITIVE);
		glow->setName("glowEffect");
		selectedBtn->addChild(glow);
	}
}

vector<BattleScene::CombatCluster> BattleScene::groupUnits() {
	vector<CombatCluster> clusters;
	vector<bool> visited(boardSlots.size(), false);
	float clusterDistance = (L::CELL_W + L::GAP) * 2.5f;

	for (size_t i = 0; i < boardSlots.size(); i++) {
		if (boardSlots[i].isEmpty || boardSlots[i].data.currentHp <= 0 || visited[i]) continue;

		CombatCluster newCluster;
		queue<int> q;
		q.push(i);
		visited[i] = true;

		while (!q.empty()) {
			int curr = q.front();
			q.pop();

			if (boardSlots[curr].isPlayerSide)
				newCluster.allies.push_back(&boardSlots[curr]);
			else
				newCluster.enemies.push_back(&boardSlots[curr]);

			for (size_t j = 0; j < boardSlots.size(); j++) {
				if (!visited[j] && !boardSlots[j].isEmpty && boardSlots[j].data.currentHp > 0) {
					float dist = boardSlots[curr].pos.distance(boardSlots[j].pos);
					if (dist <= clusterDistance) {
						visited[j] = true;
						q.push(j);
					}
				}
			}
		}

		if (!newCluster.allies.empty() || !newCluster.enemies.empty()) {
			clusters.push_back(newCluster);
		}
	}
	return clusters;
}

void BattleScene::startCombatCalculation() {
	if (_isCalculatingCombat) return;
	_isCalculatingCombat = true;

	vector<Slot*> global_allies;
	vector<Slot*> global_enemies;
	for (auto& slot : boardSlots) {
		if (!slot.isEmpty && slot.card != nullptr && slot.data.currentHp > 0) {
			if (slot.isPlayerSide) global_allies.push_back(&slot);
			else global_enemies.push_back(&slot);
		}
	}

	vector<CombatCluster> clusters = groupUnits();

	thread masterThread([this, clusters, global_allies, global_enemies]() mutable {

		vector<thread> workerThreads;

		for (auto& cluster : clusters) {
			workerThreads.emplace_back([this, cluster, global_allies, global_enemies]() mutable {

				for (auto* attacker : cluster.allies) {
					if (attacker->data.currentHp <= 0) continue;

					Slot* target = findTarget(global_enemies, attacker);
					if (target && target->data.currentHp > 0) {
						float dmg = culateAllDamage(*attacker, *target, true);

						lock_guard<mutex> lock(_mtx);
						target->data.currentHp -= dmg;
						_actionQueue.push_back({ attacker, target, dmg, true, target->data.currentHp <= 0 });

						string log = string("[PLAYER] ") + attacker->data.name + " danh "
							+ target->data.name + ": Mat " + to_string((int)dmg)
							+ " mau. Dich con " + to_string(max(0, (int)target->data.currentHp)) + " mau!\n\n";
						_logBattle += log;
					}
				}

				for (auto* attacker : cluster.enemies) {
					if (attacker->data.currentHp <= 0) continue;

					Slot* target = findTarget(global_allies, attacker);
					if (target && target->data.currentHp > 0) {
						float dmg = culateAllDamage(*attacker, *target, false);

						lock_guard<mutex> lock(_mtx);
						target->data.currentHp -= dmg;
						_actionQueue.push_back({ attacker, target, dmg, false, target->data.currentHp <= 0 });

						string log = string("[ENEMY] ") + attacker->data.name + " danh "
							+ target->data.name + ": Mat " + to_string((int)dmg)
							+ " mau. Ta con " + to_string(max(0, (int)target->data.currentHp)) + " mau!\n\n";
						_logBattle += log;
					}
				}
				});
		}

		for (auto& t : workerThreads) {
			if (t.joinable()) {
				t.join();
			}
		}

		_isCalculatingCombat = false;
		});

	masterThread.detach();
}

void BattleScene::playCombatAnimations(const vector<CombatAction>& actions) {
	for (const auto& action : actions) {
		if (!action.attacker->card || !action.target->card) continue;

		action.target->card->updateStats(action.target->data.currentHp, action.target->data.atk);

		action.attacker->card->playLungeAnimation(action.target->pos, action.attacker->pos);
		action.target->card->playHitAnimation(action.target->pos);

		auto dmgLabel = Label::createWithTTF("-" + to_string((int)action.damage), "fonts/alagard.ttf", 16);
		if (dmgLabel) {
			dmgLabel->setPosition(action.target->pos + Vec2(0, 20));
			dmgLabel->setTextColor(Color4B::RED);
			dmgLabel->enableOutline(Color4B::BLACK, 1);
			this->addChild(dmgLabel, static_cast<int>(ZOrder::Notification));

			auto floatUp = MoveBy::create(0.7f, Vec2(0, 24));
			auto fadeOut = FadeOut::create(0.7f);
			dmgLabel->runAction(Sequence::create(Spawn::create(floatUp, fadeOut, nullptr), RemoveSelf::create(), nullptr));
		}
	}
}

void BattleScene::loadTeam() {
	vector<int> savedIDs = _pController.loadPlayerTeam("CAMPAIGN");
	if (savedIDs.empty()) return;

	vector<BattleCardData> allCards = _controller.loadPlayerDeck();

	int slotIndex = 0;
	for (int cardID : savedIDs) {
		if (slotIndex >= deckSlots.size()) break;

		for (const auto& cardData : allCards) {
			if (cardData.id == cardID) {
				auto& slotD = deckSlots[slotIndex];
				slotD.data = cardData;
				slotD.data.currentHp = slotD.data.hp;

				slotD.card = CardNode::createNode(slotD.data, L::CELL_W, L::CELL_H);

				if (slotD.card) {
					slotD.card->setPosition(slotD.pos);
					slotD.card->updateStats(slotD.data.currentHp, slotD.data.atk);

					if (slotD.data.star > 1) {
						slotD.card->upStar(slotD.data.star);
					}

					this->addChild(slotD.card, static_cast<int>(ZOrder::Character));
					slotD.isEmpty = false;
					slotD.isPlayerSide = true;
				}

				slotIndex++;
				break;
			}
		}
	}
}

void BattleScene::spawnCampaignEnemies(int stageId, int wave) {
	for (auto& slotE : boardSlots) {
		if (!slotE.isEmpty && slotE.card != nullptr && !slotE.isPlayerSide) {
			slotE.card->removeFromParent();
			slotE.card = nullptr;
			slotE.isEmpty = true;
			slotE.data = BattleCardData();
		}
	}

	vector<pair<BattleCardData, int>> enemies = _controller.getCampaignEnemies(stageId, wave);

	for (auto& item : enemies) {
		BattleCardData enemyData = item.first;
		int targetSlotIndex = item.second - 1;

		if (targetSlotIndex >= 0 && targetSlotIndex < boardSlots.size()) {
			if (!boardSlots[targetSlotIndex].isEmpty && boardSlots[targetSlotIndex].isPlayerSide) {
				for (auto& emptySlot : boardSlots) {
					if (emptySlot.isEmpty) {
						emptySlot.card = boardSlots[targetSlotIndex].card;
						emptySlot.data = boardSlots[targetSlotIndex].data;
						emptySlot.isEmpty = false;
						emptySlot.isPlayerSide = true;
						if (emptySlot.card) emptySlot.card->setPosition(emptySlot.pos);

						boardSlots[targetSlotIndex].card = nullptr;
						boardSlots[targetSlotIndex].isEmpty = true;
						break;
					}
				}
			}

			if (boardSlots[targetSlotIndex].isEmpty) {
				auto& slotE = boardSlots[targetSlotIndex];
				slotE.data = enemyData;
				slotE.card = CardNode::createNode(enemyData, L::CELL_W, L::CELL_H);

				if (slotE.card) {
					slotE.card->upStar(1);
					slotE.card->setPosition(slotE.pos);
					slotE.card->updateStats(enemyData.currentHp, enemyData.atk);

					this->addChild(slotE.card, static_cast<int>(ZOrder::Character));
					slotE.isEmpty = false;
					slotE.isPlayerSide = false;
				}
			}
		}
	}
}

