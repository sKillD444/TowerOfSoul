#include "PlayerHUD.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool PlayerHUD::init() {
    if (!Node::init()) return false;

    Vec2 topRight = Responsive::getPos(0.96f, 0.95f);
    Vec2 topLeft = Responsive::getPos(0.04f, 0.95f);

    nameLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", Responsive::getSize(12));
    nameLabel->setAnchorPoint(Vec2(0.0f, 1.0f));
    nameLabel->setPosition(topLeft);
    this->addChild(nameLabel);

    gemLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", Responsive::getSize(10));
    gemLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
    gemLabel->setPosition(topRight);
    this->addChild(gemLabel);

    auto gemIcon = Sprite::create("Items/Gem.png");
    gemIcon->setAnchorPoint(Vec2(1.0f, 1.0f));
    gemIcon->setScale(Responsive::getScale(0.03f));
    gemIcon->setName("GemIcon");
    this->addChild(gemIcon);

    goldLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", Responsive::getSize(10));
    goldLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
    this->addChild(goldLabel);

    auto goldIcon = Sprite::create("Items/Gold.png");
    goldIcon->setAnchorPoint(Vec2(1.0f, 1.0f));
    goldIcon->setScale(Responsive::getScale(0.03f));
    goldIcon->setName("GoldIcon"); 
    this->addChild(goldIcon);

    return true;
}

void PlayerHUD::updatePlayerData(const PlayerData& p) {
    if (nameLabel) nameLabel->setString(p.username);

    if (gemLabel) {
        gemLabel->setString(to_string(p.gems));
        auto gemIcon = this->getChildByName("GemIcon");
        if (gemIcon) {
            float newX = gemLabel->getPositionX() - gemLabel->getContentSize().width - Responsive::getSize(5.0f);
            gemIcon->setPosition(Vec2(newX, gemLabel->getPositionY()));
        }
    }

    if (goldLabel) {
        goldLabel->setString(to_string(p.gold));
        auto gemIcon = this->getChildByName("GemIcon");
        auto goldIcon = this->getChildByName("GoldIcon");
        if (gemIcon && goldIcon) {
            float goldLabelX = gemIcon->getPositionX() - Responsive::getSize(15.0f);
            goldLabel->setPosition(Vec2(goldLabelX, gemLabel->getPositionY()));

            float goldIconX = goldLabelX - goldLabel->getContentSize().width - Responsive::getSize(5.0f);
            goldIcon->setPosition(Vec2(goldIconX, gemLabel->getPositionY()));
        }
    }
}