#include "BattleScene.h"


USING_NS_CC;
namespace L {
	const Vec2 PlayerPos(60.0f, 200.0f);
	const Vec2 EnemyPos(330.0f, 200.0f);

	constexpr float CELL_W = 30.0f; // Chiều rộng slot
	constexpr float CELL_H = 40.0f; // Chiều cao slot
	constexpr float GAP = 0.5f;    // Khoảng cách giữa các slot

	constexpr float CELLSHOP_W = 40.0f;
	constexpr float CELLSHOP_H = 55.0f;
	constexpr float CELLSHOP_X = 150.0f;
	constexpr float CELLSHOP_Y = 165.0f;
	constexpr float GAPSHOP = 5.0f;
	const Vec2 OriginShop(100.0f, 115.0f);
	const Vec2 DestinationShop(390.0f, 200.0f);

	constexpr float BtnStart_X = 240.0f;
	constexpr float BtnStart_Y = 260.0f;
	constexpr float BtnShop_X = 430.0f;
	constexpr float BtnShop_Y = 105.0f;


	constexpr float SceneCenter_X = 240.0f;
	constexpr float SceneCenter_Y = 160.0;

	const Vec2 OriginDeck(30.0f, 0.0f);
	const Vec2 DestinationDeck(445.0f, 90.0f);

	constexpr float CoinLabel_X = 40.0f;
	constexpr float CoinLabel_Y = 85.0f;
}

Scene* BattleScene::createScene()
{
	return BattleScene::create();
}

bool BattleScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ==========================================

	//Layer
	_coins = 10000;
	_uiLayer = Node::create();
	this->addChild(_uiLayer, static_cast<int>(ZOrder::UI), "uiLayer");
	this->createUI();

	//Shop
	_shopLayer = Node::create();
	this->addChild(_shopLayer, static_cast<int>(ZOrder::Shop), "shopLayer");
	this->createShop();
	_shopLayer->setVisible(false);
	loadCardShop();

	//Setting
	_settingLayer = Node::create();
	this->addChild(_settingLayer, static_cast<int>(ZOrder::Notification), "settingLayer");
	this->createSetting();
	_settingLayer->setVisible(false);

	//logBattle
	_logBattleLayer = Node::create();
	this->addChild(_logBattleLayer, static_cast<int>(ZOrder::Notification), "logBattleLayer");
	this->createLogBattle();
	_logBattleLayer->setVisible(false);

	//Bg
	auto bg = Sprite::create("BG/Background.png");
	bg->setPosition(Vec2(L::SceneCenter_X, L::SceneCenter_Y));
	auto bgSize = bg->getContentSize();
	float scaleX = visibleSize.width / bgSize.width;
	float scaleY = visibleSize.height / bgSize.height;
	bg->setScale(MAX(scaleX, scaleY));
	this->addChild(bg, static_cast<int> (ZOrder::BackGround), 'bg');

	//Deck
	this->createDeck();

	//Player slot
	this->createGrid3x3(L::PlayerPos, true);
	//Enemy slot
	this->createGrid3x3(L::EnemyPos, false);

	this->spawnEnemies();
	// ==========================================
	this->setUpTouchListener();
	return true;
}

// Create UI

void BattleScene::createUI() {
	//LABEL
	_infoLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 16);
	_infoLabel->setPosition(Vec2(L::SceneCenter_X, L::SceneCenter_Y));
	_infoLabel->setColor(Color3B::WHITE);
	this->addChild(_infoLabel, static_cast<int>(ZOrder::Notification), "infoLabel");

	auto IconCoin = Sprite::create("UI/Coin.png");
	IconCoin->setPosition(Vec2(L::CoinLabel_X, L::CoinLabel_Y));
	IconCoin->setScale(3.0f);
	_uiLayer->addChild(IconCoin, static_cast<int>(ZOrder::UI), "iconCoin");

	_coinLabel = Label::createWithTTF(std::to_string(_coins), "fonts/Marker Felt.ttf", 7);
	_coinLabel->setPosition(Vec2(L::CoinLabel_X + 10, L::CoinLabel_Y));
	_coinLabel->setColor(Color3B::YELLOW);
	_uiLayer->addChild(_coinLabel, static_cast<int>(ZOrder::UI), "coinLabel");

	//BUTTON
	auto btnStart = Sprite::create("UI/btnStart.png");
	btnStart->setPosition(Vec2(L::BtnStart_X, L::BtnStart_Y));
	_uiLayer->addChild(btnStart, static_cast<int>(ZOrder::UI), "btnStart");

	auto btnShop = Sprite::create("UI/btnShop.png");
	btnShop->setPosition(Vec2(L::BtnShop_X, L::BtnShop_Y));
	btnShop->setScale(0.15f);
	_uiLayer->addChild(btnShop, static_cast<int>(ZOrder::Shop), "btnShop");

	auto btnSetting = Sprite::create("UI/btnSetting.png");
	btnSetting->setPosition(Vec2(L::BtnStart_X + 40.0f, L::BtnStart_Y));
	btnSetting->setScale(0.15f);
	_uiLayer->addChild(btnSetting, static_cast<int>(ZOrder::UI), "btnSetting");
}

void BattleScene::createGrid3x3(Vec2 startPos, bool isPlayer) {
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			float x = startPos.x + c * (L::CELL_W + L::GAP);
			float y = startPos.y - r * (L::CELL_H + L::GAP);
			Vec2 center = Vec2(x, y);
			auto drawNode = DrawNode::create();

			Vec2 origin = Vec2(x - L::CELL_W / 2, y - L::CELL_H / 2);
			Vec2 destination = Vec2(x + L::CELL_W / 2, y + L::CELL_H / 2);

			Color4F bgColor = isPlayer ? Color4F(0, 0, 1, 0.1f) : Color4F(1, 0, 0, 0.1f);
			Color4F borderColor = isPlayer ? Color4F(0, 0.5f, 1, 0.3f) : Color4F(1, 0.2f, 0.2f, 0.3f);

			drawNode->drawSolidRect(origin, destination, bgColor);
			drawNode->drawRect(origin, destination, borderColor);
			drawNode->setName("grid_draw_" + to_string(r) + "_" + to_string(c));
			this->addChild(drawNode, static_cast<int>(ZOrder::Slot));
			Slot slot;
			slot.area = Rect(origin.x, origin.y, L::CELL_W, L::CELL_H);
			slot.pos = center;
			slot.card = nullptr;
			slot.isEmpty = true;

			if (isPlayer) playerSlots.push_back(slot);
			else enemySlots.push_back(slot);
		}
	}
}

void BattleScene::createDeck() {
	auto drawBg = DrawNode::create();
	drawBg->drawSolidRect(L::OriginDeck, L::DestinationDeck, Color4F(0.1f, 0.1f, 0.1f, 1.0f));
	this->addChild(drawBg, static_cast<int>(ZOrder::Deck));

	for (int i = 0; i < 12; i++) {
		float x = 70 + i * (L::CELL_W + L::GAP);
		float y = 57;
		Vec2 center = Vec2(x, y);

		auto drawNode = DrawNode::create();
		Vec2 origin = Vec2(x - L::CELL_W / 2, y - L::CELL_H / 2);
		Vec2 destination = Vec2(x + L::CELL_W / 2, y + L::CELL_H / 2);
		drawNode->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
		drawNode->drawRect(origin, destination, Color4F(1, 1, 1, 0.3f));
		drawNode->setName("deck_draw_" + std::to_string(i));
		this->addChild(drawNode, static_cast<int>(ZOrder::Deck));

		auto drawDelete = DrawNode::create();
		drawDelete->setName("delete_draw");
		this->addChild(drawDelete, static_cast<int>(ZOrder::Deck));

		Slot slot;
		slot.area = Rect(origin.x, origin.y, L::CELL_W, L::CELL_H);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		deckSlots.push_back(slot);
	}
}

void BattleScene::createShop() {
	auto shopBg = DrawNode::create();
	shopBg->drawSolidRect(L::OriginShop, L::DestinationShop, Color4F(0.0f, 0.0f, 0.0f, 1.0f));
	_shopLayer->addChild(shopBg, static_cast<int>(ZOrder::Shop));
	for (int i = 0; i < 5; i++) {
		float x = L::CELLSHOP_X + i * (L::CELLSHOP_W + L::GAPSHOP);
		float y = L::CELLSHOP_Y;
		Vec2 center = Vec2(x, y);

		//Cell Shop
		auto drawNode = DrawNode::create();
		Vec2 origin = Vec2(x - L::CELLSHOP_W / 2, y - L::CELLSHOP_H / 2);
		Vec2 destination = Vec2(x + L::CELLSHOP_W / 2, y + L::CELLSHOP_H / 2);
		drawNode->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
		drawNode->drawRect(origin, destination, Color4F(1, 1, 1, 1));
		_shopLayer->addChild(drawNode, static_cast<int>(ZOrder::Shop));

		//Coin
		auto CostLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 7);
		CostLabel->setPosition(Vec2(center.x, center.y - 35));
		CostLabel->setColor(Color3B::YELLOW);
		arrLabelCoin.push_back(CostLabel);
		_shopLayer->addChild(CostLabel, 10, "costLabel" + std::to_string(i));

		auto IconCoin = Sprite::create("UI/Coin.png");
		IconCoin->setPosition(Vec2(center.x - 10, center.y - 35));
		IconCoin->setScale(2.5f);
		arrIconCoin.push_back(IconCoin);
		_shopLayer->addChild(IconCoin, 10, "iconCoin" + std::to_string(i));

		//Roll
		auto RollCoinLable = Label::createWithTTF("2", "fonts/Marker Felt.ttf", 7);
		RollCoinLable->setPosition(Vec2(L::DestinationShop.x - 5, L::OriginShop.y + 25));
		RollCoinLable->setColor(Color3B::YELLOW);
		_shopLayer->addChild(RollCoinLable, static_cast<int>(ZOrder::Shop), "costLabel" + std::to_string(i));
		auto IconCoinRoll = Sprite::create("UI/Coin.png");
		IconCoinRoll->setPosition(Vec2(L::DestinationShop.x - 12, L::OriginShop.y + 25));
		IconCoinRoll->setScale(2.5f);
		_shopLayer->addChild(IconCoinRoll, static_cast<int>(ZOrder::Shop), "iconCoin");
		auto btnRoll = Sprite::create("UI/btnRoll.png");
		btnRoll->setPosition(Vec2(L::DestinationShop.x - 10, L::OriginShop.y + 10));
		btnRoll->setScale(0.2f);
		_shopLayer->addChild(btnRoll, static_cast<int>(ZOrder::Shop), "btnRoll");

		Slot slot;
		slot.area = Rect(origin.x, origin.y, L::CELLSHOP_W, L::CELLSHOP_H);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		ShopSlots.push_back(slot);
	}

}

void BattleScene::createSetting() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x_W = origin.x + visibleSize.width * 0.4;
	float y_W = origin.y + visibleSize.height * 0.3;
	float x_H = origin.x + visibleSize.width * 0.6;
	float y_H = origin.y + visibleSize.height * 0.7;

	float x = origin.x + visibleSize.width * 0.5;
	float y = origin.y + visibleSize.height * 0.65;
	Vec2 originSetting(x_W, y_W);
	Vec2 DestinationSetting(x_H, y_H);

	auto settingBg = DrawNode::create();
	settingBg->drawSolidRect(originSetting, DestinationSetting, Color4F(0.0f, 0.0f, 0.0f, 1.0f));
	_settingLayer->addChild(settingBg, static_cast<int>(ZOrder::Notification));

	auto resumeLabel = Label::createWithTTF("Resume", "fonts/alagard.ttf", 16);
	resumeLabel->enableBold();
	auto resumeItem = MenuItemLabel::create(resumeLabel, [this](Ref*) {
		_settingLayer->setVisible(false);
		});
	resumeItem->setColor(Color3B(240, 235, 220));
	resumeItem->setPosition(Vec2(x, y));

	auto newGameLabel = Label::createWithTTF("New Game", "fonts/alagard.ttf", 16);
	newGameLabel->enableBold();
	auto newGameItem = MenuItemLabel::create(newGameLabel, [this](Ref*) {
		_settingLayer->setVisible(false);
		this->endGame();
		});
	newGameItem->setColor(Color3B(240, 235, 220));
	newGameItem->setPosition(Vec2(x, y - 30.0f));

	auto homeLabel = Label::createWithTTF("Home", "fonts/alagard.ttf", 16);
	homeLabel->enableBold();
	auto homeItem = MenuItemLabel::create(homeLabel, [](Ref*) {
		auto scene = MenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		});
	homeItem->setColor(Color3B(240, 235, 220));
	homeItem->setPosition(Vec2(x, y - 60.0f));

	auto menu = Menu::create(resumeItem, newGameItem, homeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	_settingLayer->addChild(menu, static_cast<int>(ZOrder::Notification));
}

void BattleScene::createLogBattle() {
	auto scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setContentSize(Size(250, 100));
	scrollView->setPosition(Vec2(L::SceneCenter_X, L::SceneCenter_Y));
	scrollView->setAnchorPoint(Vec2(0.5f, 0.5f));
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL); // Chỉ cuộn dọc
	scrollView->setBounceEnabled(true);

	scrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
	scrollView->setBackGroundColor(Color3B(20, 20, 20)); // Màu đen nhạt
	scrollView->setBackGroundColorOpacity(200); // Độ trong suốt

	_logLabel = Label::createWithTTF(_logBattle, "fonts/arial.ttf", 7);
	_logLabel->setColor(Color3B::WHITE);
	float margin = 10.0f;
	_logLabel->setDimensions(scrollView->getContentSize().width - margin * 2, 0);
	_logLabel->setAnchorPoint(Vec2(0.0f, 1.0f));

	_resultLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 16);
	_resultLabel->setColor(Color3B::WHITE);
	_resultLabel->setPosition(Vec2(L::SceneCenter_X, L::SceneCenter_Y + 60.0f));
	_resultLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	_logBattleLayer->addChild(_resultLabel, static_cast<int>(ZOrder::Notification));

	float innerHeight = _logLabel->getContentSize().height + margin * 2;
	float viewHeight = scrollView->getContentSize().height;

	if (innerHeight < viewHeight) {
		innerHeight = viewHeight;
	}
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, innerHeight));
	_logLabel->setPosition(Vec2(margin, innerHeight - margin));

	auto btnOk = cocos2d::ui::Button::create("UI/Ok.png", "UI/OkHover.png");
	btnOk->setPosition(Vec2(L::SceneCenter_X, L::SceneCenter_Y - 60.0f));
	btnOk->setAnchorPoint(Vec2(0.5f, 0.5f));
	_logBattleLayer->addChild(btnOk, static_cast<int>(ZOrder::Notification), "btnOkLog");

	scrollView->addChild(_logLabel);
	_logBattleLayer->addChild(scrollView, static_cast<int>(ZOrder::Notification), "infoScrollView");
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
	if (_isBattle) return false;
	Vec2 pos = touch->getLocation();

	auto btnStart = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnStart"));

	if (btnStart && btnStart->getBoundingBox().containsPoint(pos) && _btnPlayEnabled&&!_shopLayer->isVisible()&& !_settingLayer->isVisible()) {
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
			this->startBattle();
		}
		return true;
	}

	auto btnShop = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnShop"));
	if (btnShop && btnShop->getBoundingBox().containsPoint(pos) && _btnShopEnabled&& !_settingLayer->isVisible()) {
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
	if (btnSetting && btnSetting->getBoundingBox().containsPoint(pos) && _btnSettingEnabled&&!_shopLayer->isVisible()) {
		bool isOpening = !_settingLayer->isVisible();
		_settingLayer->setVisible(isOpening);
		btnSetting->setOpacity(200);
		return true;
	}

	//Shop
	if (_shopLayer->isVisible()) {
		//Roll
		auto btnRoll = dynamic_cast<Sprite*>(_shopLayer->getChildByName("btnRoll"));
		if (btnRoll && btnRoll->getBoundingBox().containsPoint(pos) && _btnShopEnabled) {
			if (_coins >= 2) {
				_coins -= 2;
				_coinLabel->setString(std::to_string(_coins));
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
				if (slotShop.data.cost <= _coins) {
					for (auto& slotD : deckSlots) {
						if (slotD.isEmpty) {
							slotD.card = CardNode::createNode(slotShop.data, L::CELL_W, L::CELL_H);
							if (slotD.card != nullptr) {
								slotD.data = slotShop.data;
								slotD.card->setPosition(slotD.pos);
								this->addChild(slotD.card, static_cast<int>(ZOrder::Character));
								slotD.isEmpty = false;

								slotShop.card->removeFromParent();
								slotShop.card = nullptr;
								slotShop.isEmpty = true;

								_coins -= slotShop.data.cost;
								_coinLabel->setString(std::to_string(_coins));

								arrLabelCoin[&slotShop - &ShopSlots[0]]->setString("");
								arrIconCoin[&slotShop - &ShopSlots[0]]->setVisible(false);

								if (slotD.data.star < 4) {
									mergeCard(slotD);
								}
							}
							break;
						}
					}
					return true;
				}
				else {
					_infoLabel->setString("Khong du coin");
					auto delay = DelayTime::create(1.5f);
					auto squence = Sequence::create(delay, CallFunc::create([this]() {
						_infoLabel->setString("");
						}), nullptr);
					this->runAction(squence);
					break;
				}
			}
		}

	}

	if(!_shopLayer->isVisible()&&!_settingLayer->isVisible())
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

	for (auto& slot : playerSlots) {
		if (!slot.isEmpty && slot.card && slot.area.containsPoint(pos)) {
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
	if (btnStart) {
		btnStart->setOpacity(255);
	}

	auto btnShop = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnShop"));
	if (btnShop) {
		btnShop->setOpacity(255);
	}

	auto btnSetting = dynamic_cast<Sprite*>(_uiLayer->getChildByName("btnSetting"));
	if (btnSetting) {
		btnSetting->setOpacity(255);
	}

	if (!selectedCard) return;
	bool placed = false;
	highLightLine(false);

	//DeleteCard
	if (deleteCard(selectedCard, selectedData)) {
		selectedCard = nullptr;
		return;
	}

	Slot* select = nullptr;
	for (auto& slot : playerSlots) {
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
	if (select) {
		select->card = selectedCard;
		select->data = selectedData;
		select->isEmpty = false;
	}

	selectedCard->setScale(selectedCard->getScale() / 1.2f);
	Vec2 cardCenter = selectedCard->getPosition();
	for (auto& slot : playerSlots) {
		if (slot.area.containsPoint(cardCenter)) {
			if (select == &slot) {
				placed = false;
				break;
			}
			if (slot.isEmpty) {
				selectedCard->setPosition(slot.pos);
				slot.card = selectedCard;
				slot.data = selectedData;
				if (select != nullptr && select != &slot) {
					select->card = nullptr;
					select->isEmpty = true;
				}
			}
			else {
				swapCard(*select, slot);
			}
			slot.isEmpty = false;
			placed = true;
			break;
		}
	}

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
	if (!placed) {
		auto moveBack = MoveTo::create(0.15f, originalPos);
		selectedCard->runAction(moveBack);

		bool isRestored = false;

		for (auto& slotD : deckSlots) {
			if (slotD.pos == originalPos) {
				slotD.card = selectedCard;
				slotD.isEmpty = false;
				isRestored = true;
				break;
			}
		}

		if (!isRestored) {
			for (auto& slot : playerSlots) {
				if (slot.pos == originalPos) {
					slot.card = selectedCard;
					slot.isEmpty = false;
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
	_round++;
	_infoLabel->setString("Round " + std::to_string(_round));
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

	for (auto& p : playerSlots) {
		if (!p.isEmpty && p.card) {
			p.data.currentHp = p.data.hp;
			float dmg = p.data.atk;
			dmg += synergyAtkMult(p.data.role);
			p.card->updateStats(p.data.currentHp, dmg);
		}
	}

	for (auto& e : enemySlots) {
		if (!e.isEmpty && e.card) {
			e.data.currentHp = e.data.hp;;
			float dmg = e.data.atk;
			dmg += e.data.atk * (_round / 10);
			e.card->updateStats(e.data.currentHp, dmg);
		}
	}
	this->scheduleUpdate();
}

void BattleScene::endBattle() {
	_btnShopEnabled = true;
	_btnPlayEnabled = true;
	_isBattle = false;

	_infoLabel->setString("");
	_shopLayer->setVisible(true);

	_coins += 3;
	_coinLabel->setString(std::to_string(_coins));
	loadCardShop();
	spawnEnemies();

	for (auto& p : playerSlots) {
		if (!p.isEmpty && p.card) {
			p.card->updateStats(p.data.hp, p.data.atk);
		}
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

	_infoLabel->setString("");
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
	_coinLabel->setString(std::to_string(_coins));
	_round = 0;

	for (auto& slot : playerSlots) {
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
	loadCardShop();
	spawnEnemies();
}

//Card Management
int BattleScene::countUnitsOnBoard() {
	int count = 0;
	for (const auto& slot : playerSlots) {
		if (!slot.isEmpty) count++;
	}
	return count;
}

int BattleScene::countCard(BattleCardData data) {
	int count = 0;
	for (auto slot : playerSlots) {
		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == data.card_id && slot.data.star == data.star)
			count++;
	}
	for (auto slot : deckSlots) {
		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == data.card_id && slot.data.star == data.star)
			count++;
	}
	return count;
}

void BattleScene::mergeCard(Slot& card) {
	string targetId = card.data.card_id;
	int targetStar = card.data.star;
	if (countCard(card.data) < 3)return;
	bool flag = true;
	int count = 0;
	Slot* upgradedSlot = nullptr;
	for (auto& slot : playerSlots) {
		if (count > 2)break;
		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == targetId && slot.data.star == targetStar) {
			if (flag) {
				slot.data.star++;
				_controller.updateStar(slot.data);
				slot.card->updateStats(slot.data.currentHp, slot.data.atk);
				slot.card->upStar(slot.data.star);
				count++;
				upgradedSlot = &slot;
				flag = false;
			}
			else {
				slot.card->removeFromParentAndCleanup(true);
				slot.card = nullptr;
				slot.isEmpty = true;
				count++;
			}
		}
	}
	for (auto& slot : deckSlots) {
		if (count > 2)break;
		if (!slot.isEmpty && slot.card != nullptr && slot.data.card_id == targetId && slot.data.star == targetStar) {
			if (flag) {
				slot.data.star++;
				_controller.updateStar(slot.data);
				slot.card->updateStats(slot.data.currentHp, slot.data.atk);
				slot.card->upStar(slot.data.star);
				count++;
				upgradedSlot = &slot;
				flag = false;
			}
			else {
				slot.card->removeFromParentAndCleanup(true);
				slot.card = nullptr;
				slot.isEmpty = true;
				count++;
			}
		}
	}
	if (upgradedSlot != nullptr && upgradedSlot->data.star < 3) {
		mergeCard(*upgradedSlot);
	}
}

bool BattleScene::deleteCard(CardNode* select, BattleCardData data) {
	Vec2 originDelete = Vec2(L::OriginDeck.x - 30.0f, L::OriginDeck.y);
	Vec2 destinationDelete = Vec2(L::OriginDeck.x, L::DestinationDeck.y);
	float width = destinationDelete.x - originDelete.x;
	float height = destinationDelete.y - originDelete.y;
	Rect deleteZone(originDelete.x, originDelete.y, width, height);
	Rect deleteZone2(originDelete.x + 445, originDelete.y, width, height);
	if (deleteZone.containsPoint(select->getPosition()) || deleteZone2.containsPoint(select->getPosition())) {
		_coins += data.cost;
		_coinLabel->setString(std::to_string(_coins));
		select->removeFromParentAndCleanup(true);
		return 1;
	}
	return 0;
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
		arrLabelCoin[i]->setString(std::to_string(shopData[i].cost));
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
	for (int i = 0; i < 12; i++) {
		string str = "deck_draw_" + to_string(i);
		auto drawDeck = dynamic_cast<DrawNode*>(this->getChildByName(str));
		if (drawDeck != nullptr) {
			drawDeck->clear();
			float x = 68 + i * (L::CELL_W + L::GAP);
			float y = 57;
			Vec2 origin = Vec2(x - L::CELL_W / 2, y - L::CELL_H / 2);
			Vec2 destination = Vec2(x + L::CELL_W / 2, y + L::CELL_H / 2);
			if (flag) {
				drawDeck->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
				drawDeck->drawRect(origin, destination, Color4F(1, 1, 1, 1.0f));
			}
			else {
				drawDeck->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
				drawDeck->drawRect(origin, destination, Color4F(1, 1, 1, 0.3f));
			}
		}
	}
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			string str = "grid_draw_" + to_string(r) + "_" + to_string(c);
			auto drawGrid = dynamic_cast<DrawNode*>(this->getChildByName(str));
			if (drawGrid != nullptr) {
				drawGrid->clear();
				float x = L::PlayerPos.x + c * (L::CELL_W + L::GAP);
				float y = L::PlayerPos.y - r * (L::CELL_H + L::GAP);
				Vec2 origin = Vec2(x - L::CELL_W / 2, y - L::CELL_H / 2);
				Vec2 destination = Vec2(x + L::CELL_W / 2, y + L::CELL_H / 2);
				if (flag) {
					drawGrid->drawSolidRect(origin, destination, Color4F(0, 0, 1, 0.1f));
					drawGrid->drawRect(origin, destination, Color4F(0, 0.5f, 1, 1.0f));
				}
				else {
					drawGrid->drawSolidRect(origin, destination, Color4F(0, 0, 1, 0.1f));
					drawGrid->drawRect(origin, destination, Color4F(0, 0.5f, 1, 0.3f));
				}
			}
		}
	}
	auto drawDelete = dynamic_cast<DrawNode*>(this->getChildByName("delete_draw"));
	if (flag) {
		Vec2 originDelete = Vec2(L::OriginDeck.x - 30.0f, L::OriginDeck.y);
		Vec2 destinationDelete = Vec2(L::OriginDeck.x, L::DestinationDeck.y);
		drawDelete->drawSolidRect(originDelete, destinationDelete, Color4F(1, 0, 0, 0.5f));
		drawDelete->drawRect(originDelete, destinationDelete, Color4F(1, 0.2f, 0.2f, 1.0f));
		drawDelete->drawSolidRect(originDelete + Vec2(445, 0), destinationDelete + Vec2(470, 0), Color4F(1, 0, 0, 0.5f));
		drawDelete->drawRect(originDelete + Vec2(445, 0), destinationDelete + Vec2(470, 0), Color4F(1, 0.2f, 0.2f, 1.0f));
	}
	else {
		drawDelete->clear();
		drawDelete->removeAllChildren();
	}
}

void BattleScene::spawnEnemies() {

	for (auto& slotE : enemySlots) {
		if (!slotE.isEmpty && slotE.card != nullptr) {
			slotE.card->removeFromParent();
			slotE.card = nullptr;
			slotE.isEmpty = true;
			slotE.data = BattleCardData();
		}
	}

	std::vector<int> emptySlotIndices;
	for (int i = 0; i < enemySlots.size(); i++) {
		emptySlotIndices.push_back(i);
	}

	int count = 1 + (_round / 2);
	int upStar = 1 + (_round / 7);
	if (count > 9)
		count = 9;
	auto enemies = _controller.spawnEnemies(count);
	for (size_t i = 0; i < enemies.size(); i++) {
		if (emptySlotIndices.empty()) break;
		int randomIndex = cocos2d::RandomHelper::random_int(0, (int)emptySlotIndices.size() - 1);

		int targetSlotIndex = emptySlotIndices[randomIndex];
		emptySlotIndices.erase(emptySlotIndices.begin() + randomIndex);

		auto& slotE = enemySlots[targetSlotIndex];
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
			slotE.data = enemies[i];
		}
	}
}

BattleScene::Slot* BattleScene::findTarget(std::vector<Slot>& targetSlots, Slot& attacker) {
	Slot* bestTarget = nullptr;
	float bestDist = 99999.0f;

	if (!attacker.card) return nullptr;
	Vec2 attackerPos = attacker.pos;

	for (auto& slot : targetSlots) {
		if (!slot.isEmpty && slot.card != nullptr && slot.data.currentHp > 0) {
			float dist = attackerPos.distance(slot.pos);
			if (dist < bestDist) {
				bestDist = dist;
				bestTarget = &slot;
			}
		}
	}
	return bestTarget;
}

float BattleScene::synergyAtkMult(std::string role) {
	int dem = 0;
	for (auto& p : playerSlots) {
		if (p.data.role == role)
			dem++;
	}
	if (dem > 5)
		return 25 ;
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
	if (isPlayerAttacking) {
		dmg += synergyAtkMult(attacker.data.role);
	}
	else
	{
		dmg += dmg * (_round / 10);
	}
	return dmg;
}

void BattleScene::doAttack(Slot& attacker, Slot& target, bool isPlayerAttacking) {
	if (!attacker.card || !target.card) return;

	float dmg = culateAllDamage(attacker, target, isPlayerAttacking);

	target.data.currentHp -= dmg;


	target.card->updateStats(target.data.currentHp, target.data.atk);

	attacker.card->playLungeAnimation(target.pos, attacker.pos);
	target.card->playHitAnimation(target.pos);

	auto dmgLabel = Label::createWithTTF("-" + std::to_string((int)dmg), "fonts/Marker Felt.ttf", 16);
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

void BattleScene::update(float dt) {
	if (!_isBattle) return;

	int alivePlayers = 0;
	int aliveEnemys = 0;

	for (auto& p : playerSlots) {
		if (!p.isEmpty) {
			if (p.data.currentHp <= 0) {
				p.card->playDeathAnimation(L::CELL_W, L::CELL_H);
				p.card = nullptr;
				p.isEmpty = true;
			}
			else
				alivePlayers++;
		}
	}

	for (auto& e : enemySlots) {
		if (!e.isEmpty) {
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
				this->endBattle();
				});
		}
		return;
	}
	else if (state == BattleState::LOSE) {
		this->unscheduleUpdate();
		_logLabel->setString(_logBattle);
		_resultLabel->setString("LOSE");
		_resultLabel->setString("");
		_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
		auto btn = dynamic_cast<cocos2d::ui::Button*>(_logBattleLayer->getChildByName("btnOkLog"));
		if (btn) {
			btn->addClickEventListener([this](Ref*) {
				_logBattleLayer->setVisible(!_logBattleLayer->isVisible());
				_logBattle = "";
				_infoLabel->setString("");
				_logLabel->setString(_logBattle);
				this->endGame();
				});
		}
		return;
	}

	_turnTimer -= dt;
	if (_turnTimer > 0) return;

	bool attackExecuted = false;
	int loopCount = 0;

	while (!attackExecuted && loopCount < 18) {
		std::vector<Slot>& attackerSide = _isPlayerTurn ? playerSlots : enemySlots;
		std::vector<Slot>& targetSide = _isPlayerTurn ? enemySlots : playerSlots;

		const int playerOrder[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
		const int enemyOrder[9] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };

		if (_currentAttackerIndex < 9) {

			int realIndex = _isPlayerTurn ? playerOrder[_currentAttackerIndex] : enemyOrder[_currentAttackerIndex];

			if (realIndex < attackerSide.size()) {
				Slot& atk = attackerSide[realIndex];
				if (!atk.isEmpty && atk.data.currentHp > 0 && atk.card) {
					Slot* target = findTarget(targetSide, atk);
					if (target) {
						doAttack(atk, *target, _isPlayerTurn);
						attackExecuted = true;
						_turnTimer = 1.0f;
					}
				}
			}
		}

		bool wasPlayer = _isPlayerTurn;
		_isPlayerTurn = !_isPlayerTurn;
		if (!wasPlayer) {
			_currentAttackerIndex++;
		}

		if (_currentAttackerIndex >= 9) {
			_currentAttackerIndex = 0;
		}
		loopCount++;
	}
}




