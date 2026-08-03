#include "LeaderboardScene.h"

USING_NS_CC;

Scene* LeaderboardScene::createScene() {
    return LeaderboardScene::create();
}

bool LeaderboardScene::init() {
    if (!Scene::init()) return false;

    auto bg = LayerColor::create(Color4B(25, 35, 50, 255));
    this->addChild(bg, static_cast<int>(ZOrder::BG));

    auto hud = PlayerHUD::create();
    this->addChild(hud, static_cast<int>(ZOrder::Info), "HUD");

    PlayerData p = _pController.loadPlayer();
    hud->updatePlayerData(p);

    _boardContainer = Node::create();
    this->addChild(_boardContainer, static_cast<int>(ZOrder::Label));

    this->createLeaderboardUI();
    this->createSortButtons();
    this->updateBoardData(Sort::FLOOR);
    this->backHome();

    return true;
}

void LeaderboardScene::backHome() {
    auto btnHome = Button::create("UI/BTN/BTN_Back.png");
    btnHome->setScale(Responsive::getScale(0.2f));
    btnHome->setAnchorPoint(Vec2(0.0f, 1.0f));
    btnHome->setPosition(Responsive::getPos(0.03f, 0.9f));
    btnHome->addClickEventListener([](Ref*) {
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
        });
    this->addChild(btnHome, static_cast<int>(ZOrder::Button));
}

void LeaderboardScene::createLeaderboardUI() {
    Vec2 centerPos = Responsive::getPos(0.5f, 0.5f);
    float boardWidth = Responsive::getSize(280.0f);
    float boardHeight = Responsive::getSize(230.0f);

    auto bgBoard = DrawNode::create();
    Vec2 bgOrigin(centerPos.x - boardWidth / 2, centerPos.y - boardHeight / 2);
    Vec2 bgDest(centerPos.x + boardWidth / 2, centerPos.y + boardHeight / 2);
    bgBoard->drawSolidRect(bgOrigin, bgDest, Color4F(0.0f, 0.0f, 0.0f, 0.6f));
    bgBoard->drawRect(bgOrigin, bgDest, Color4F(1.0f, 0.8f, 0.0f, 1.0f));
    this->addChild(bgBoard, static_cast<int>(ZOrder::BG));

    auto titleLabel = Label::createWithTTF("TOP 10 PLAYERS", "fonts/alagard.ttf", Responsive::getSize(20));
    titleLabel->setPosition(Vec2(centerPos.x, centerPos.y + boardHeight / 2 - Responsive::getSize(20.0f)));
    titleLabel->setColor(Color3B::YELLOW);
    this->addChild(titleLabel, static_cast<int>(ZOrder::Label));
}

void LeaderboardScene::createSortButtons() {
    Vec2 centerPos = Responsive::getPos(0.5f, 0.5f);
    float boardWidth = Responsive::getSize(280.0f);

    float buttonX = centerPos.x + boardWidth / 2 + Responsive::getSize(20.0f);
    float startY = centerPos.y + boardWidth / 2 - Responsive::getSize(40.0f);
    float gap = Responsive::getSize(30.0f);

    vector<pair<string, Sort>> sortTypes = {
        {"Floor", Sort::FLOOR},
        {"Gem", Sort::GEM},
        {"Stage", Sort::STAGE},
        {"Cards", Sort::CARD_OWNER}
    };

    for (size_t i = 0; i < sortTypes.size(); i++) {
        auto btn = Button::create("UI/BTN/BG_BTN2.png"); 
        btn->setScale(Responsive::getScale(0.15f));
        btn->setPosition(Vec2(buttonX, startY - i * gap));

        auto label = Label::createWithTTF(sortTypes[i].first, "fonts/04B_03__.TTF", Responsive::getSize(24));
        label->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height / 2));
        btn->addChild(label);

        btn->addClickEventListener([this, sort = sortTypes[i].second](Ref*) {
            this->updateBoardData(sort);
            });

        this->addChild(btn, static_cast<int>(ZOrder::Button));
    }
}

void LeaderboardScene::updateBoardData(Sort sort) {
    _boardContainer->removeAllChildren();

    Vec2 centerPos = Responsive::getPos(0.5f, 0.5f);
    float boardWidth = Responsive::getSize(280.0f);
    float boardHeight = Responsive::getSize(230.0f);
    float startY = centerPos.y + boardHeight / 2 - Responsive::getSize(45.0f);
    float gap = (boardHeight - Responsive::getSize(50.0f)) / 10.0f;

    auto top10 = _lController.getTop10PlayersBy((int)sort);

    string unitName = "";
    if (sort == Sort::FLOOR) unitName = "Floor";
    else if (sort == Sort::GEM) unitName = "Gems";
    else if (sort == Sort::STAGE) unitName = "Stage";
    else if (sort == Sort::CARD_OWNER) unitName = "Cards";

    for (size_t i = 0; i < top10.size(); i++) {
        string rowText = to_string(top10[i].rank) + ". " +
            top10[i].username + " - " + unitName + ": " +
            to_string(top10[i].highest_floor);

        auto rowLabel = Label::createWithTTF(rowText, "fonts/04B_03__.TTF", Responsive::getSize(14));
        rowLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
        rowLabel->setPosition(Vec2(centerPos.x - boardWidth / 2 + Responsive::getSize(30.0f), startY - i * gap));

        if (i == 0) rowLabel->setColor(Color3B(255, 215, 0));
        else if (i == 1) rowLabel->setColor(Color3B(192, 192, 192));
        else if (i == 2) rowLabel->setColor(Color3B(205, 127, 50));
        else rowLabel->setColor(Color3B::WHITE);

        _boardContainer->addChild(rowLabel);
    }
}