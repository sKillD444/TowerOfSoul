#include "TeamScene.h"

USING_NS_CC;

namespace L {
	constexpr float CELL_W = 40.0f;
	constexpr float CELL_H = 40.0f;
	constexpr float GAP = 0.5f;
	const Vec2 OriginDeck(15.0f, 0.0f);
	const Vec2 DestinationDeck(460.0f, 90.0f);
}
Scene* TeamScene::createScene()
{
	return TeamScene::create();
}

bool TeamScene::init()
{
	if (!Scene::init()) return false;

	auto bg = LayerColor::create(Color4B(25, 35, 50, 255));
	this->addChild(bg, static_cast<int>(ZOrder::BG));

	auto hud = PlayerHUD::create();
	this->addChild(hud, static_cast<int>(ZOrder::Info), "HUD");

	PlayerData p = _pController.loadPlayer();
	hud->updatePlayerData(p);

	// UI
	this->createDeck();
	this->loadSavedTeam();

	this->backHome();
	this->createOpenInventoryButton();

	return true;
}

void TeamScene::createDeck() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float deckWidth = visibleSize.width * 0.9f;
	float deckHeight = Responsive::getSize(80.0f);
	Vec2 originDeck(origin.x + (visibleSize.width - deckWidth) / 2.0f, origin.y + Responsive::getSize(10.0f));
	Vec2 destDeck(originDeck.x + deckWidth, originDeck.y + deckHeight);

	auto drawBg = DrawNode::create();
	drawBg->drawSolidRect(originDeck, destDeck, Color4F(0.1f, 0.1f, 0.1f, 1.0f));
	this->addChild(drawBg, static_cast<int>(ZOrder::Slot));

	float availableWidth = deckWidth - Responsive::getSize(20.0f);
	float stepX = availableWidth / 10.0f;
	float startX = originDeck.x + Responsive::getSize(10.0f) + stepX / 2.0f;
	float centerY = originDeck.y + deckHeight / 2.0f;

	float cellW = Responsive::getSize(L::CELL_W);
	float cellH = Responsive::getSize(L::CELL_H);

	for (int i = 0; i < 10; i++) {
		float x = startX + i * stepX;
		Vec2 center = Vec2(x, centerY);

		auto drawNode = DrawNode::create();
		Vec2 originSlot = Vec2(x - cellW / 2, centerY - cellH / 2);
		Vec2 destSlot = Vec2(x + cellW / 2, centerY + cellH / 2);
		drawNode->drawSolidRect(originSlot, destSlot, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
		drawNode->drawRect(originSlot, destSlot, Color4F(1, 1, 1, 0.3f));
		drawNode->setName("deck_draw_" + to_string(i));
		this->addChild(drawNode, static_cast<int>(ZOrder::Slot));

		Slot slot;
		slot.area = Rect(originSlot.x, originSlot.y, cellW, cellH);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		deckSlots.push_back(slot);
	}
}

void TeamScene::createOpenInventoryButton() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto btnOpen = Button::create("UI/BTN/btnHome.png");
	btnOpen->setScale(0.2f);
	btnOpen->setAnchorPoint(Vec2(1.0f, 0.0f));
	btnOpen->setPosition(Vec2(origin.x + visibleSize.width - 15.0f, origin.y + visibleSize.height * 0.7f));

	btnOpen->addClickEventListener([this](Ref*) {
		this->showInventoryPopup();
		});
	this->addChild(btnOpen, static_cast<int>(ZOrder::Button));
}

void TeamScene::showInventoryPopup() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	_inventoryLayer = Node::create();
	this->addChild(_inventoryLayer, 100);

	auto touchBlocker = Layout::create();
	touchBlocker->setContentSize(visibleSize);
	touchBlocker->setTouchEnabled(true);
	_inventoryLayer->addChild(touchBlocker, -1);

	Vec2 originBG = Responsive::getPos(0.1f, 0.1f);
	Vec2 destinationBG = Responsive::getPos(0.9f, 0.9f);
	auto bg = DrawNode::create();
	bg->drawSolidRect(originBG, destinationBG, Color4F(0.15f, 0.15f, 0.15f, 0.95f));
	bg->drawRect(originBG, destinationBG, Color4F(0.8f, 0.6f, 0.2f, 1.0f));
	_inventoryLayer->addChild(bg);

	_tempSelectedCards.clear();
	for (auto& slot : deckSlots) {
		if (!slot.isEmpty && slot.card != nullptr) {
			_tempSelectedCards.push_back(slot.data);
		}
	}

	auto scrollView = ScrollView::create();
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(destinationBG.x - originBG.x - Responsive::getSize(20.0f), destinationBG.y - originBG.y - Responsive::getSize(80.0f)));
	scrollView->setPosition(Vec2(originBG.x + Responsive::getSize(10.0f), originBG.y + Responsive::getSize(60.0f)));
	scrollView->setBounceEnabled(true);
	_inventoryLayer->addChild(scrollView);
	
	vector<BattleCardData> allCards = _cController.loadPlayerDeck();

	int cols = 5;
	float spacingX = (scrollView->getContentSize().width - Responsive::getSize(90.0f)) / (cols - 1);
	float spacingY = Responsive::getSize(80.0f);
	float innerHeight = max(scrollView->getContentSize().height, (allCards.size() / cols + 1) * spacingY + Responsive::getSize(20.0f));
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, innerHeight));

	float cellW = Responsive::getSize(L::CELL_W + 30.0f);
	float cellH = Responsive::getSize(L::CELL_H + 30.0f);

	for (size_t i = 0; i < allCards.size(); i++) {
		int row = i / cols;
		int col = i % cols;
		float x = Responsive::getSize(30.0f) + col * spacingX;
		float y = innerHeight - Responsive::getSize(50.0f) - row * spacingY;

		auto cardNode = CardNode::createNode(allCards[i], cellW, cellH);
		cardNode->setPosition(Vec2(x, y));
		scrollView->addChild(cardNode);

		auto btnInfo = Button::create("UI/BTN/BG_BTN.png");
		btnInfo->setTitleText("Info");
		btnInfo->setTitleFontName("fonts/04B_03__.ttf");
		btnInfo->setTitleFontSize(Responsive::getSize(40));
		btnInfo->setScale(Responsive::getScale(0.15f));
		btnInfo->setPosition(Vec2(x, y - cellH / 2 - Responsive::getSize(5.0f)));
		scrollView->addChild(btnInfo, 11);

		btnInfo->addClickEventListener([this, data = allCards[i]](Ref*) {
			this->showCardDetailPopup(data, nullptr);
			});

		auto touchBtn = Widget::create();
		touchBtn->setContentSize(Size(cellW, cellH));
		touchBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
		touchBtn->setPosition(Vec2(x, y));
		touchBtn->setTouchEnabled(true);
		touchBtn->setSwallowTouches(false);

		bool isAlreadySelected = false;
		for (const auto& sel : _tempSelectedCards) {
			if (sel.id == allCards[i].id) {
				isAlreadySelected = true;
				break;
			}
		}
		if (isAlreadySelected) {
			auto tickMark = Sprite::create("UI/tick.png");
			tickMark->setPosition(Vec2(cellW / 2, cellH / 2));
			tickMark->setScale(Responsive::getScale(0.07f));
			tickMark->setName("SelectedTick");
			touchBtn->addChild(tickMark, 10);
		}

		touchBtn->addClickEventListener([this, data = allCards[i], touchBtn, cellW, cellH](Ref*) {
			auto tick = touchBtn->getChildByName("SelectedTick");
			if (tick == nullptr) {
				if (_tempSelectedCards.size() < 10) {
					_tempSelectedCards.push_back(data);
					auto tickMark = Sprite::create("UI/tick.png");
					tickMark->setPosition(Vec2(cellW / 2, cellH / 2));
					tickMark->setScale(Responsive::getScale(0.07f));
					tickMark->setName("SelectedTick");
					touchBtn->addChild(tickMark, 10);
				}
			}
			else {
				touchBtn->removeChildByName("SelectedTick");
				for (auto it = _tempSelectedCards.begin(); it != _tempSelectedCards.end(); ++it) {
					if (it->id == data.id) {
						_tempSelectedCards.erase(it);
						break;
					}
				}
			}
			});
		scrollView->addChild(touchBtn, 10);
	}

	auto btnConfirm = Button::create("UI/BTN/BG_BTN.png");
	btnConfirm->setTitleText("Confirm");
	btnConfirm->setTitleFontName("fonts/04B_03__.ttf");
	btnConfirm->setScale(Responsive::getScale(0.15f));
	btnConfirm->setTitleFontSize(Responsive::getSize(40));
	btnConfirm->setPosition(Vec2(visibleSize.width / 2, originBG.y + Responsive::getSize(40.0f)));
	btnConfirm->addClickEventListener([this](Ref* sender) {
		auto btn = static_cast<Button*>(sender);
		btn->setTouchEnabled(false);
		this->confirmSelection();
		});
	_inventoryLayer->addChild(btnConfirm);

	auto btnClose = Button::create("UI/BTN/btnBack2.png");
	btnClose->setScale(Responsive::getScale(1.5f));
	btnClose->setAnchorPoint(Vec2(1.0f, 1.0f));
	btnClose->setPosition(Vec2(destinationBG.x - Responsive::getSize(5.0f), destinationBG.y - Responsive::getSize(5.0f)));
	btnClose->addClickEventListener([this](Ref* sender) {
		auto btn = static_cast<Button*>(sender);
		btn->setTouchEnabled(false);
		_tempSelectedCards.clear();
		_inventoryLayer->setVisible(false);
		_inventoryLayer->runAction(Sequence::create(DelayTime::create(0.05f), RemoveSelf::create(), nullptr));
		_inventoryLayer = nullptr;
		});
	_inventoryLayer->addChild(btnClose);
}

void TeamScene::confirmSelection() {
	for (auto& slot : deckSlots) {
		if (!slot.isEmpty && slot.card != nullptr) {
			slot.card->removeFromParent();
			slot.card = nullptr;
			slot.isEmpty = true;
		}
	}

	vector<int> selectedIDs;
	for (int i = 0; i < _tempSelectedCards.size() && i < deckSlots.size(); i++) {
		auto& slot = deckSlots[i];
		slot.card = CardNode::createNode(_tempSelectedCards[i], L::CELL_W, L::CELL_H);
		if (slot.card != nullptr) {
			slot.card->setPosition(slot.pos);
			this->addChild(slot.card, static_cast<int>(ZOrder::Card));
			slot.isEmpty = false;
			slot.data = _tempSelectedCards[i];

			selectedIDs.push_back(_tempSelectedCards[i].id);
		}
	}

	_pController.savePlayerTeam("CAMPAIGN", selectedIDs);

	_tempSelectedCards.clear();
	if (_inventoryLayer != nullptr) {
		_inventoryLayer->setVisible(false);
		_inventoryLayer = nullptr;
	}
}

void TeamScene::showCardDetailPopup(BattleCardData data, Node* cardNode) {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto detailLayer = Node::create();
	this->addChild(detailLayer, 200);

	auto touchBlocker = Layout::create();
	touchBlocker->setContentSize(visibleSize);
	touchBlocker->setTouchEnabled(true);
	detailLayer->addChild(touchBlocker, -1);

	Vec2 originBG = Vec2(visibleSize.width * 0.2f, visibleSize.height * 0.2f);
	Vec2 destinationBG = Vec2(visibleSize.width * 0.8f, visibleSize.height * 0.8f);
	auto bg = DrawNode::create();
	bg->drawSolidRect(originBG, destinationBG, Color4F(0.1f, 0.1f, 0.1f, 0.98f));
	bg->drawRect(originBG, destinationBG, Color4F(0.8f, 0.6f, 0.2f, 1.0f));
	detailLayer->addChild(bg);

	float cardX = originBG.x + (destinationBG.x - originBG.x) * 0.25f;
	float centerY = originBG.y + (destinationBG.y - originBG.y) / 2;

	auto bigCard = CardNode::createNode(data, L::CELL_W * 3.0f, L::CELL_H * 3.0f);
	bigCard->setPosition(Vec2(cardX, centerY));
	detailLayer->addChild(bigCard);

	float textX = originBG.x + (destinationBG.x - originBG.x) * 0.52f;
	float textStartY = centerY + 60;
	float lineSpacing = 25.0f;

	auto lblName = Label::createWithSystemFont("Name: " + data.name, "04B_03__", 16);
	lblName->setAnchorPoint(Vec2(0, 0.5f));
	lblName->setPosition(textX, textStartY);
	detailLayer->addChild(lblName);

	auto lblLevel = Label::createWithSystemFont("Level: " + to_string(data.level) + " / 5", "04B_03__", 15);
	lblLevel->setColor(Color3B::YELLOW);
	lblLevel->setAnchorPoint(Vec2(0, 0.5f));
	lblLevel->setPosition(textX, textStartY - lineSpacing);
	detailLayer->addChild(lblLevel);

	auto lblStar = Label::createWithSystemFont("Star: " + to_string(data.star), "04B_03__", 15);
	lblStar->setAnchorPoint(Vec2(0, 0.5f));
	lblStar->setPosition(textX, textStartY - lineSpacing * 2);
	detailLayer->addChild(lblStar);

	auto lblAtk = Label::createWithSystemFont("ATK: " + to_string((int)data.atk), "04B_03__", 15);
	lblAtk->setAnchorPoint(Vec2(0, 0.5f));
	lblAtk->setPosition(textX, textStartY - lineSpacing * 3);
	detailLayer->addChild(lblAtk);

	auto lblHp = Label::createWithSystemFont("HP: " + to_string((int)data.hp), "04B_03__", 15);
	lblHp->setAnchorPoint(Vec2(0, 0.5f));
	lblHp->setPosition(textX, textStartY - lineSpacing * 4);
	detailLayer->addChild(lblHp);

	int upgradeCost = _cController.getUpgradeCost(data.level);
	string btnText = (upgradeCost == -1) ? "MAX LEVEL" : (to_string(upgradeCost) + " Gold");

	auto btnUpgrade = ui::Button::create("UI/BTN/BG_BTN2.png");
	btnUpgrade->setScale(0.16f);
	btnUpgrade->setTitleText(btnText);
	btnUpgrade->setTitleFontSize(40);
	btnUpgrade->setPosition(Vec2(textX + 100, textStartY - lineSpacing * 5.0f));
	detailLayer->addChild(btnUpgrade);

	if (upgradeCost == -1) {
		btnUpgrade->setEnabled(false);
		btnUpgrade->setColor(Color3B::GRAY);
	}
	else {
		btnUpgrade->addClickEventListener([this, data, detailLayer](Ref*) mutable {
			PlayerData p = _pController.loadPlayer();
			int cost = _cController.getUpgradeCost(data.level);

			if (p.gold >= cost) {
				
				bool ok = _cController.upgradeCard(p.id, data, p.gold);
				if (ok) {
					p.gold -= cost;
					_pController.updateGold(p.id, p.gold);

					auto hud = dynamic_cast<PlayerHUD*>(this->getChildByName("HUD"));
					if (hud) hud->updatePlayerData(p);

					detailLayer->removeFromParent();
					this->showCardDetailPopup(data, nullptr);
				}
			}
			else {
				CCLOG("Không đủ vàng để nâng cấp!");
			}
			});
	}
	auto btnClose = Button::create("UI/BTN/BTNX.png");
	btnClose->setScale(1.5f);
	btnClose->setAnchorPoint(Vec2(1.0f, 1.0f));
	btnClose->setPosition(Vec2(destinationBG.x+10, destinationBG.y+10));
	btnClose->addClickEventListener([detailLayer](Ref*) {
		detailLayer->removeFromParent();
		});
	detailLayer->addChild(btnClose);
}

void TeamScene::backHome()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto btnHome = Button::create("UI/BTN/BTN_Back.png");
	btnHome->setScale(0.2f);
	btnHome->setAnchorPoint(Vec2(0.0f, 1.0f));
	btnHome->setPosition(Vec2(origin.x + 15.0f, origin.y + visibleSize.height - 30.0f));
	btnHome->addClickEventListener([](Ref*)
		{
			auto scene = MenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0))); });
	this->addChild(btnHome, static_cast<int>(ZOrder::Button));
}

void TeamScene::loadSavedTeam() {
	vector<int> savedIDs = _pController.loadPlayerTeam("CAMPAIGN");
	if (savedIDs.empty()) return;

	vector<BattleCardData> allCards = _cController.loadPlayerDeck();

	int slotIndex = 0;
	for (int cardID : savedIDs) {
		if (slotIndex >= deckSlots.size()) break;
		for (const auto& cardData : allCards) {
			if (cardData.id == cardID) {
				auto& slot = deckSlots[slotIndex];
				slot.card = CardNode::createNode(cardData, L::CELL_W, L::CELL_H);
				if (slot.card != nullptr) {
					slot.card->setPosition(slot.pos);
					this->addChild(slot.card, static_cast<int>(ZOrder::Card));
					slot.isEmpty = false;
					slot.data = cardData;
				}
				slotIndex++;
				break;
			}
		}
	}
}
