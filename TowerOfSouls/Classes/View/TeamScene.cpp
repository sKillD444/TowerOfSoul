#include "TeamScene.h"
#include "PlayerHUD.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace cocos2d::ui;
namespace L {
	constexpr float CELL_W = 30.0f;
	constexpr float CELL_H = 40.0f;
	constexpr float GAP = 0.5f;    // Khoảng cách giữa các slot
	const Vec2 OriginDeck(15.0f, 0.0f);
	const Vec2 DestinationDeck(460.0f, 90.0f);
}
Scene* TeamScene::createScene()
{
	return TeamScene::create();
}

bool TeamScene::init()
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

	//UI
	this->createDeck();
	this->createTeam();
	this->backHome();
	this->createOpenInventoryButton();
	/*this->loadCardOwned();*/
	// ==========================================
	return true;
}

void TeamScene::createDeck() {
	auto drawBg = DrawNode::create();
	drawBg->drawSolidRect(L::OriginDeck, L::DestinationDeck, Color4F(0.1f, 0.1f, 0.1f, 1.0f));
	this->addChild(drawBg, static_cast<int>(ZOrder::Slot));

	for (int i = 0; i < 14; i++) {
		float x = 40 + i * (L::CELL_W + L::GAP);
		float y = 57;
		Vec2 center = Vec2(x, y);

		auto drawNode = DrawNode::create();
		Vec2 origin = Vec2(x - L::CELL_W / 2, y - L::CELL_H / 2);
		Vec2 destination = Vec2(x + L::CELL_W / 2, y + L::CELL_H / 2);
		drawNode->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
		drawNode->drawRect(origin, destination, Color4F(1, 1, 1, 0.3f));
		drawNode->setName("deck_draw_" + std::to_string(i));
		this->addChild(drawNode, static_cast<int>(ZOrder::Slot));

		Slot slot;
		slot.area = Rect(origin.x, origin.y, L::CELL_W, L::CELL_H);
		slot.card = nullptr;
		slot.isEmpty = true;
		slot.pos = center;
		deckSlots.push_back(slot);
	}
}

void TeamScene::createTeam() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x = origin.x + visibleSize.width * 0.43f;
	float y = origin.y + visibleSize.height * 0.4f;

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			float x_cell = x+ c * (L::CELL_W + L::GAP);
			float y_cell = y+ r * (L::CELL_H + L::GAP);
			Vec2 center = Vec2(x, y);
			auto drawNode = DrawNode::create();

			Vec2 origin = Vec2(x_cell - L::CELL_W / 2, y_cell - L::CELL_H / 2);
			Vec2 destination = Vec2(x_cell + L::CELL_W / 2, y_cell + L::CELL_H / 2);

			drawNode->drawSolidRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 0.1f));
			drawNode->drawRect(origin, destination, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
			drawNode->setName("grid_draw_" + to_string(r) + "_" + to_string(c));
			this->addChild(drawNode, static_cast<int>(ZOrder::Slot));
			Slot slot;
			slot.area = Rect(origin.x, origin.y, L::CELL_W, L::CELL_H);
			slot.pos = center;
			slot.card = nullptr;
			slot.isEmpty = true;
			teamSlots.push_back(slot);
		}
	}
}

//void TeamScene::loadCardOwned() {
//	vector<BattleCardData> owenedCard = _cController.loadPlayerDeck();
//
//	int safeLimit = std::min(owenedCard.size(), deckSlots.size());
//
//	for (int i = 0; i < safeLimit; i++) {
//		auto& slot = deckSlots[i];
//		slot.card = CardNode::createNode(owenedCard[i], L::CELL_W, L::CELL_H);
//		if (slot.card != nullptr) {
//			slot.card->setPosition(slot.pos);
//			this->addChild(slot.card, static_cast<int>(ZOrder::Card));
//			slot.isEmpty = false;
//			slot.data = owenedCard[i];
//		}
//	}
//}

void TeamScene::createOpenInventoryButton() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto btnOpen = ui::Button::create("UI/btnHome.png");
	btnOpen->setScale(0.2f);
	btnOpen->setPosition(Vec2(visibleSize.width * 0.95f, visibleSize.height*0.95f));

	btnOpen->addClickEventListener([this](Ref*) {
		for (auto& slot : deckSlots) {
			if (!slot.isEmpty && slot.card != nullptr) {
				slot.card->removeFromParent();
				slot.card = nullptr;
				slot.isEmpty = true;
			}
		}
		this->showInventoryPopup();
		});
	this->addChild(btnOpen, static_cast<int>(ZOrder::Button));
}

void TeamScene::showInventoryPopup() {
	auto visibleSize = Director::getInstance()->getVisibleSize();

	_inventoryLayer = Node::create();
	this->addChild(_inventoryLayer, 100);

	auto touchBlocker = ui::Layout::create();
	touchBlocker->setContentSize(visibleSize);
	touchBlocker->setTouchEnabled(true);
	_inventoryLayer->addChild(touchBlocker, -1);

	Vec2 originBG = Vec2(visibleSize.width * 0.1f, visibleSize.height * 0.1f);
	Vec2 destinationBG = Vec2(visibleSize.width * 0.9f, visibleSize.height * 0.9f);
	auto bg = DrawNode::create();
	bg->drawSolidRect(originBG, destinationBG, Color4F(0.15f, 0.15f, 0.15f, 0.95f));
	_inventoryLayer->addChild(bg);

	_tempSelectedCards.clear();
	for (auto& slot : deckSlots) {
		if (!slot.isEmpty) {
			_tempSelectedCards.push_back(slot.data);
		}
	}

	auto scrollView = ui::ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(destinationBG.x - originBG.x - 20, destinationBG.y - originBG.y - 80));
	scrollView->setPosition(Vec2(originBG.x + 10, originBG.y + 60));
	scrollView->setBounceEnabled(true);
	_inventoryLayer->addChild(scrollView);

	std::vector<BattleCardData> allCards = _cController.loadPlayerDeck();

	int cols = 5;
	float spacingX = 60.0f;
	float spacingY = 80.0f;
	float innerHeight = std::max(scrollView->getContentSize().height, (allCards.size() / cols + 1) * spacingY + 20);
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, innerHeight));

	for (size_t i = 0; i < allCards.size(); i++) {
		int row = i / cols;
		int col = i % cols;
		float x = 40.0f + col * spacingX;
		float y = innerHeight - 50.0f - row * spacingY;

		auto cardNode = CardNode::createNode(allCards[i], L::CELL_W+30, L::CELL_H+30);
		cardNode->setPosition(Vec2(x, y));
		scrollView->addChild(cardNode);

		auto touchBtn = ui::Widget::create();
		touchBtn->setContentSize(Size(L::CELL_W+30, L::CELL_H+30));
		touchBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
		touchBtn->setPosition(Vec2(x, y));
		touchBtn->setTouchEnabled(true);
		touchBtn->setSwallowTouches(false);

		touchBtn->addClickEventListener([this, data = allCards[i], touchBtn](Ref*) {
			auto tick = touchBtn->getChildByName("SelectedTick");

			if (tick == nullptr) {
				if (_tempSelectedCards.size() < 14) {
					_tempSelectedCards.push_back(data);
					auto tickMark = Sprite::create("UI/tick.png");
					tickMark->setPosition(Vec2((L::CELL_W+30) / 2, (L::CELL_H +30)/ 2));
					tickMark->setScale(0.07f);
					tickMark->setName("SelectedTick");
					touchBtn->addChild(tickMark, 10);
				}
			}
			});
		scrollView->addChild(touchBtn, 10);
	}

	auto btnConfirm = ui::Button::create("UI/btnPlay.png");
	btnConfirm->setScale(0.15f);
	btnConfirm->setPosition(Vec2(visibleSize.width / 2, originBG.y + 40));
	btnConfirm->addClickEventListener([this](Ref* sender) {
		auto btn = static_cast<ui::Button*>(sender);
		btn->setTouchEnabled(false);
		this->confirmSelection();
		});
	_inventoryLayer->addChild(btnConfirm);

	auto btnClose = ui::Button::create("UI/btnBack.png");
	btnClose->setScale(0.1f);
	btnClose->setPosition(Vec2(originBG.x + 30, destinationBG.y-10));
	btnClose->addClickEventListener([this](Ref* sender) {
		auto btn = static_cast<ui::Button*>(sender);
		btn->setTouchEnabled(false);

		_tempSelectedCards.clear();
		_inventoryLayer->setVisible(false);
		_inventoryLayer->runAction(Sequence::create(DelayTime::create(0.05f), RemoveSelf::create(), nullptr));
		_inventoryLayer = nullptr;
		});
	_inventoryLayer->addChild(btnClose);
}

void TeamScene::confirmSelection() {
	for (int i = 0; i < _tempSelectedCards.size() && i < deckSlots.size(); i++) {
		auto& slot = deckSlots[i];
		slot.card = CardNode::createNode(_tempSelectedCards[i], L::CELL_W, L::CELL_H);
		if (slot.card != nullptr) {
			slot.card->setPosition(slot.pos);
			this->addChild(slot.card, static_cast<int>(ZOrder::Card));
			slot.isEmpty = false;
			slot.data = _tempSelectedCards[i];
		}
	}
	_tempSelectedCards.clear();
	if (_inventoryLayer != nullptr) {
		_inventoryLayer->setVisible(false);
		_inventoryLayer = nullptr;
	}
}

void TeamScene::showCardDetailPopup(BattleCardData data, Node* cardNode) {
	
}

void TeamScene::backHome() {
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


