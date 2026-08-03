#include "CardNode.h"

USING_NS_CC;

CardNode* CardNode::createNode(const BattleCardData& data, float cellW, float cellH) {
    CardNode* node = new (std::nothrow) CardNode();//std::nothrow nếu thất bại nó trả về nullptr
    if (node->init(data, cellW, cellH)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool CardNode::init(const BattleCardData& data, float cellW, float cellH) {
    if (!Node::init()) return false;

    string bgFilename = "BG/BG_Card_Purple.png";
    if (data.role == "Assassin")
        bgFilename = "BG/BG_Card_Gray.png";
    else if (data.role == "Warrior")
        bgFilename = "BG/BG_Card_Red.png";
    else if (data.role == "Ranged DPS")
        bgFilename = "BG/BG_Card_Green.png";
    else if (data.role == "Defender")
        bgFilename = "BG/BG_Card_Blue.png";
    else if (data.role == "BOSS")
        bgFilename = "BG/BG_Card_Purple.png";

    _bgSprite = Sprite::create(bgFilename);
    if (_bgSprite == nullptr) {
        CCLOG("Lỗi không tìm thấy file nền %s", bgFilename.c_str());
        return false;
    }

    float charScaleX = cellW / _bgSprite->getContentSize().width + 0.3f;
    float charScaleY = cellH / _bgSprite->getContentSize().height + 0.3f;
    _baseScale = std::min(charScaleX, charScaleY);
    _bgSprite->setScale(_baseScale);
    this->addChild(_bgSprite);

    float bgW = _bgSprite->getContentSize().width;
    float bgH = _bgSprite->getContentSize().height;

    string charFilename = data.card_id + ".png";
    auto charSprite = Sprite::createWithSpriteFrameName(charFilename);
    if (charSprite != nullptr) {
        charSprite->setPosition(Vec2(bgW / 2.0f - 5, bgH / 2.0f));
        _bgSprite->addChild(charSprite, 1);
    }

    string frameFilename = "UI/Frame_Brown.png";

    if (data.type == "PLAYER") {
        frameFilename = UserDefault::getInstance()->getStringForKey("PlayerFrameColor", "UI/Frame_Blue.png");
    }
    else if (data.type == "MONSTER" || data.role == "BOSS" || data.type == "BOSS") {
        frameFilename = UserDefault::getInstance()->getStringForKey("EnemyFrameColor", "UI/Frame_Red.png");
    }

    auto frameSprite = Sprite::create(frameFilename);
    if (frameSprite != nullptr) {
        frameSprite->setPosition(Vec2(bgW / 2.0f, bgH / 2.0f));
        _bgSprite->addChild(frameSprite, 2);
    }
    else {
        CCLOG("Lỗi không tìm thấy file khung %s", frameFilename.c_str());
    }

    //Name
    auto nameLabel = Label::createWithTTF(data.name, "fonts/arial.ttf", 2.8f);
    nameLabel->setMaxLineWidth(bgW * 0.2f);
    nameLabel->setTextColor(Color4B::BLACK);
    nameLabel->setPosition(Vec2(bgW * 0.7f, bgH * 0.65f));
    _bgSprite->addChild(nameLabel, 3);

    // ATK
    _atkLabel = Label::createWithTTF(to_string((int)data.atk), "fonts/04B_03__.ttf", 3.5f);
    _atkLabel->setTextColor(Color4B::RED);
    _atkLabel->setPosition(Vec2(bgW * 0.7f, bgH * 0.5f));
    _atkLabel->setName("CardATK");
    _bgSprite->addChild(_atkLabel, 3);

    // HP
    _hpLabel = Label::createWithTTF(to_string((int)data.hp), "fonts/04B_03__.ttf", 3.5f);
    _hpLabel->setTextColor(Color4B(0, 150, 0, 255));
    _hpLabel->setPosition(Vec2(bgW * 0.7f, bgH * 0.4f));
    _hpLabel->setName("CardHP");
    _bgSprite->addChild(_hpLabel, 3);

    // Star
    int star = data.star;
    _starLabel = Label::createWithTTF("Star: " + to_string(star), "fonts/04B_03__.ttf", 8.4f);
    _starLabel->setTextColor(Color4B(255, 215, 0, 255));
    _starLabel->enableOutline(Color4B::BLACK, 1);
    _starLabel->setPosition(Vec2(bgW * 0.7f, bgH * 0.25f));
    _starLabel->setScale(0.33f);
    _starLabel->setName("CardStar");
    _bgSprite->addChild(_starLabel, 3);

    return true;
}

void CardNode::updateStats(float currentHp, float currentAtk) {
    if (_hpLabel) _hpLabel->setString(to_string((int)currentHp));
    if (_atkLabel) _atkLabel->setString(to_string((int)currentAtk));
}

void CardNode::playDeathAnimation(float cellW, float cellH) {
    if (_bgSprite) _bgSprite->setVisible(false);
    if (_hpLabel) _hpLabel->setVisible(false);
    if (_atkLabel) _atkLabel->setVisible(false);

    auto frameCache = SpriteFrameCache::getInstance();
    auto firstFrame = frameCache->getSpriteFrameByName("DesCard000.png");

    if (firstFrame == nullptr) {
        CCLOG("Không thấy DesCard000.png!");
        this->removeFromParent();
        return;
    }

    auto effectSprite = Sprite::createWithSpriteFrame(firstFrame);
    effectSprite->setPosition(Vec2::ZERO);

    float scaleX = (cellW + 5.0f) / effectSprite->getContentSize().width;
    float scaleY = (cellH + 5.0f) / effectSprite->getContentSize().height;
    effectSprite->setScale(std::min(scaleX, scaleY) * 1.5f);

    this->addChild(effectSprite, 100);

    Vector<SpriteFrame*> animFrames;
    char str[100] = { 0 };

    for (int i = 0; i <= 12; i++) {
        if (i == 10) continue;
        sprintf(str, "DesCard%03d.png", i);
        auto frame = frameCache->getSpriteFrameByName(str);
        if (frame != nullptr) {
            animFrames.pushBack(frame);
        }
    }

    if (animFrames.size() > 0) {
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.04f);
        Animate* animate = Animate::create(animation);

        auto sequence = Sequence::create(
            animate,
            CallFunc::create([this]() {
                this->removeFromParent();
                }),
            nullptr
        );
        effectSprite->runAction(sequence);
    }
    else {
        this->removeFromParent();
    }
}

void CardNode::playLungeAnimation(cocos2d::Vec2 targetPos, cocos2d::Vec2 basePos) {
    this->resetStateAndZOrder(basePos);
    Vec2 dir = (targetPos - basePos).getNormalized();
    Vec2 lungePos = basePos + dir * 14.0f;

    auto setZHigh = CallFunc::create([this]() {this->setLocalZOrder(100); });
    auto setZLow = CallFunc::create([this]() {this->setLocalZOrder(static_cast<int>(4)); });
    auto sequence = Sequence::create(setZHigh, MoveTo::create(0.08f, lungePos), MoveTo::create(0.08f, basePos), setZLow, nullptr);
    sequence->setTag(1);
    this->runAction(sequence);
}

void CardNode::playHitAnimation(cocos2d::Vec2 basePos) {
    if (_bgSprite) {
        _bgSprite->stopActionByTag(2);
        _bgSprite->setColor(Color3B::WHITE);
        auto tintRed = TintTo::create(0.1f, 255, 100, 100);
        auto tintBack = TintTo::create(0.1f, 255, 255, 255);
        auto seqColor = Sequence::create(tintRed, tintBack, nullptr);
        seqColor->setTag(2);
        _bgSprite->runAction(seqColor);
    }
    this->resetStateAndZOrder(basePos);
    auto jiggle = Sequence::create(
        MoveBy::create(0.03f, Vec2(-3, 0)),
        MoveBy::create(0.03f, Vec2(6, 0)),
        MoveBy::create(0.03f, Vec2(-3, 0)),
        nullptr
    );
    jiggle->setTag(1);
    this->runAction(jiggle);
}

void CardNode::resetStateAndZOrder(cocos2d::Vec2 basePos) {
    this->stopActionByTag(1);
    this->setPosition(basePos);
    this->setLocalZOrder(static_cast<int>(4));
}

void CardNode::upStar(int newStar) {
    if (_starLabel) {
        string currentText = _starLabel->getString();

        int currentStar = 0;
        if (currentText.length() > 6) {
            currentStar = stoi(currentText.substr(6));
        }

        if (currentStar < newStar) {
            _starLabel->setString("Star: " + to_string(newStar));
        }
    }
}