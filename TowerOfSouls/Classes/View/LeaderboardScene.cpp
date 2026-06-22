#include "LeaderboardScene.h"
#include "PlayerHUD.h"
#include "MenuScene.h"

USING_NS_CC;
using namespace cocos2d::ui;
namespace L
{

}
Scene * LeaderboardScene::createScene()
{
	return LeaderboardScene::create();
}

bool LeaderboardScene::init()
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

    this->createLeaderboardUI();
	this->backHome();

	// ==========================================
	return true;
}

void LeaderboardScene::backHome()
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

void LeaderboardScene::createLeaderboardUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float centerX = origin.x + visibleSize.width / 2;
    float centerY = origin.y + visibleSize.height / 2;

    auto bgBoard = DrawNode::create();
    Vec2 bgOrigin(centerX - 150.0f, centerY - 120.0f);
    Vec2 bgDest(centerX + 150.0f, centerY + 100.0f);
    bgBoard->drawSolidRect(bgOrigin, bgDest, Color4F(0.0f, 0.0f, 0.0f, 0.6f));
    bgBoard->drawRect(bgOrigin, bgDest, Color4F(1.0f, 0.8f, 0.0f, 1.0f));
    this->addChild(bgBoard, static_cast<int>(ZOrder::BG));

    auto titleLabel = Label::createWithTTF("TOP 10 PLAYERS", "fonts/alagard.ttf", 20);
    titleLabel->setPosition(Vec2(centerX, centerY + 80.0f));
    titleLabel->setColor(Color3B::YELLOW);
    this->addChild(titleLabel, static_cast<int>(ZOrder::Label));

    auto top10 = _lController.getTop10Players();

    float startY = centerY + 50.0f;
    float gap = 15.0f;

    for (size_t i = 0; i < top10.size(); i++)
    {
        std::string rowText = std::to_string(top10[i].rank) + ". " +
            top10[i].username + " - Floor: " +
            std::to_string(top10[i].highest_floor);

        auto rowLabel = Label::createWithTTF(rowText, "fonts/04B_03__.TTF", 14);

        rowLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
        rowLabel->setPosition(Vec2(centerX - 120.0f, startY - i * gap));

        if (i == 0) rowLabel->setColor(Color3B(255, 215, 0));
        else if (i == 1) rowLabel->setColor(Color3B(192, 192, 192));
        else if (i == 2) rowLabel->setColor(Color3B(205, 127, 50));
        else rowLabel->setColor(Color3B::WHITE);

        this->addChild(rowLabel, static_cast<int>(ZOrder::Label));
    }
}