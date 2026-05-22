#include "PlayerHUD.h"


USING_NS_CC;
using namespace cocos2d::ui;
bool PlayerHUD::init()
{
	if (!Node::init())
	{
		return false;
	}

	// ==========================================
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x = origin.x + visibleSize.width * 0.85f;
	float y = origin.y + visibleSize.height * 0.95f;

	nameLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 10);
	nameLabel->setPosition(Vec2(origin.x + visibleSize.width * 0.1f, y));
	nameLabel->setTextColor(Color4B(255, 255, 255, 200));
	nameLabel->enableOutline(Color4B::BLACK, 2);
	this->addChild(nameLabel);

	goldLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 8);
	goldLabel->setColor(Color3B::YELLOW);
	goldLabel->setPosition(Vec2(x, y));
	goldLabel->enableOutline(Color4B::BLACK, 2);
	this->addChild(goldLabel);

	auto goldIcon = Sprite::create("UI/Coin.png");
	goldIcon->setPosition(Vec2(x - 19.0f, y-1));
	goldIcon->setScale(2.5);
	this->addChild(goldIcon);

	gemLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 8);
	gemLabel->setColor(Color3B(255, 150, 255));
	gemLabel->setPosition(Vec2(x + 40.0f, y));
	gemLabel->enableOutline(Color4B::BLACK, 2);
	this->addChild(gemLabel);

	auto gemIcon = Sprite::create("UI/Coin.png");
	gemIcon->setPosition(Vec2(x + 21.0f, y-1));
	gemIcon->setScale(2.5);
	this->addChild(gemIcon);

	// ==========================================
	return true;
}

void PlayerHUD::updatePlayerData(const PlayerData& p) {
	if (nameLabel) nameLabel->setString(p.username);
	if (goldLabel)goldLabel->setString(to_string(p.gold));
	if (gemLabel) gemLabel->setString(to_string(p.gems));
}

