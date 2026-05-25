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

    string filename = data.card_id + ".png";
	_bgSprite = Sprite::createWithSpriteFrameName(filename);
	if (_bgSprite == nullptr) {
		CCLOG("Lỗi không tìm thấy file %s", filename.c_str());
		return false;
	}
	float charScaleX = cellW / _bgSprite->getContentSize().width + 0.2f;
	float charScaleY = cellH / _bgSprite->getContentSize().height + 0.2f;
	_baseScale = std::min(charScaleX, charScaleY);
	_bgSprite->setScale(_baseScale);

	this->addChild(_bgSprite);

    float x = _bgSprite->getContentSize().width;
    float y = _bgSprite->getContentSize().height;

	_atkLabel = Label::createWithTTF(to_string((int)data.atk), "fonts/arial.ttf", 3.0f);
	_atkLabel->setTextColor(Color4B::RED);
	_atkLabel->setPosition(Vec2(x / 2.5f,y / 5.2f));
	_atkLabel->setName("CardATK");
	_bgSprite->addChild(_atkLabel);

	int hp = data.currentHp > 0 ? (int)data.currentHp : (int)data.hp;
	_hpLabel = Label::createWithTTF(to_string(hp), "fonts/arial.ttf", 3.0f);
	_hpLabel->setTextColor(Color4B::GREEN);
	_hpLabel->setPosition(Vec2(x/ 1.3f,y / 5.2f));
	_hpLabel->setName("CardHP");
	_bgSprite->addChild(_hpLabel);

    int star = data.star;
    _starLabel = Label::createWithTTF(to_string(star), "fonts/arial.ttf", 8.4f);
    _starLabel->setTextColor(Color4B(255, 215, 0, 255));
    _starLabel->enableOutline(Color4B::BLACK, 1);
    _starLabel->setPosition(x - 4.7f, y - 4.3f);
    _starLabel->setScale(0.33f);
    _starLabel->setName("CardStar");
    _bgSprite->addChild(_starLabel);

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
    Vec2 lungePos = basePos + dir *14.0f;

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
    int currentStar = stoi(_starLabel->getString());
    if (currentStar < newStar && newStar < 4)
        _starLabel->setString(to_string(newStar));
}