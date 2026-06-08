#include "TeamScene.h"
#include "PlayerHUD.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace cocos2d::ui;
namespace L {
	constexpr float CELL_W = 30.0f;
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
	this->backHome();
	// ==========================================
	return true;
}

void TeamScene::backHome()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto btnHome = Button::create("UI/btnHome.png");
	btnHome->setScale(0.2f);
	btnHome->setPosition(Vec2(origin.x + 15.0f, visibleSize.height + 10));
	btnHome->addClickEventListener([](Ref*)
		{
			auto scene = MenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0))); });
	this->addChild(btnHome, static_cast<int>(ZOrder::Button));
}


