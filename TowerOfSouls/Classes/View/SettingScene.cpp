#include "SettingScene.h"

USING_NS_CC;

Scene* SettingScene::createScene()
{
    return SettingScene::create();
}

bool SettingScene::init()
{
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = LayerColor::create(Color4B(15, 20, 35, 255));
    this->addChild(bg, 0);

    _tabAudioLayer = Node::create();
    _tabGamePlayLayer = Node::create();
    _tabPlayerLayer = Node::create();

    this->addChild(_tabAudioLayer, 1);
    this->addChild(_tabGamePlayLayer, 1);
    this->addChild(_tabPlayerLayer, 1);

    buildAudioTab();
    buildGamePlayTab();
    buildPlayerTab();

    createTabsMenu();
    backHome();

    switchTab(0);

    return true;
}

void SettingScene::backHome()
{
    auto btnHome = Button::create("UI/BTN/BTN_Back.png");
    btnHome->setScale(Responsive::getScale(0.2f));
    btnHome->setAnchorPoint(Vec2(0.0f, 1.0f));
    btnHome->setPosition(Responsive::getPos(0.03f, 0.9f));
    btnHome->addClickEventListener([](Ref*) {
        auto scene = MenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
        });
    this->addChild(btnHome, 1);
}

void SettingScene::createTabsMenu()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float topY = origin.y + visibleSize.height - 30.0f;
    float centerX = origin.x + visibleSize.width / 2.0f;

    vector<string> tabNames = { "Audio", "GamePLay", "Player" };
    float tabWidth = 100.0f;

    for (int i = 0; i < tabNames.size(); i++) {
        auto btnTab = Button::create("UI/BTN/BG_BTN2.png");
        btnTab->setTitleText(tabNames[i]);
        btnTab->setTitleFontName("fonts/alagard.ttf");
        btnTab->setTitleFontSize(32);
        btnTab->setScale(0.22f);
        btnTab->setPosition(Vec2(centerX - tabWidth + (i * tabWidth), topY));
        btnTab->addClickEventListener([this, i](Ref*) {
            switchTab(i);
            });
        this->addChild(btnTab, 5);
    }
}

void SettingScene::switchTab(int tabIndex)
{
    _tabAudioLayer->setVisible(tabIndex == 0);
    _tabGamePlayLayer->setVisible(tabIndex == 1);
    _tabPlayerLayer->setVisible(tabIndex == 2);
}

void SettingScene::buildAudioTab()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2.0f;
    float centerY = origin.y + visibleSize.height / 2.0f;

    auto lblTitle = Label::createWithTTF("-- AUDIO SETTINGS --", "fonts/04B_03__.ttf", 36);
    lblTitle->setPosition(Vec2(centerX, centerY + 80.0f));
    lblTitle->setColor(Color3B::YELLOW);
    lblTitle->setScale(0.6f);
    _tabAudioLayer->addChild(lblTitle);

    float currentVol = UserDefault::getInstance()->getFloatForKey("MusicVolume", 1.0f);
    int bgmId = UserDefault::getInstance()->getIntegerForKey("bgmId", -1);

    if (bgmId == -1 || AudioEngine::getState(bgmId) != AudioEngine::AudioState::PLAYING) {
        string bgMusic = UserDefault::getInstance()->getStringForKey("MenuMusic", "Audio/Music/Alex_Morgan_Chillhop_Jazz_Sunny_Cafe.mp3");
        bgmId = AudioEngine::play2d(bgMusic, true, currentVol);
        UserDefault::getInstance()->setIntegerForKey("bgmId", bgmId);
    }

    auto lblVol = Label::createWithTTF("Music Volume:", "fonts/alagard.ttf", 32);
    lblVol->setPosition(Vec2(centerX - 110.0f, centerY + 20.0f));
    lblVol->setScale(0.5f);
    _tabAudioLayer->addChild(lblVol);

    auto lblVolPercent = Label::createWithTTF(to_string((int)(currentVol * 100)) + "%", "fonts/04B_03__.ttf", 32);
    lblVolPercent->setPosition(Vec2(centerX + 155.0f, centerY + 20.0f));
    lblVolPercent->setColor(Color3B::YELLOW);
    lblVolPercent->setScale(0.5f);
    _tabAudioLayer->addChild(lblVolPercent);

    float barWidth = 140.0f;
    float barHeight = 8.0f;
    Vec2 barStart(centerX - 20.0f, centerY + 20.0f);
    Vec2 barEnd(barStart.x + barWidth, barStart.y);

    auto barDraw = DrawNode::create();
    barDraw->drawSolidRect(Vec2(barStart.x, barStart.y - barHeight / 2), Vec2(barEnd.x, barEnd.y + barHeight / 2), Color4F(0.3f, 0.3f, 0.3f, 1.0f));
    barDraw->drawRect(Vec2(barStart.x, barStart.y - barHeight / 2), Vec2(barEnd.x, barEnd.y + barHeight / 2), Color4F::WHITE);
    _tabAudioLayer->addChild(barDraw);

    auto thumb = Sprite::create("UI/BTN/BG_BTN.png");
    thumb->setScale(0.08f);
    float initialX = barStart.x + (currentVol * barWidth);
    thumb->setPosition(Vec2(initialX, barStart.y));
    _tabAudioLayer->addChild(thumb, 10);

    auto applyVolume = [lblVolPercent](float vol) {
        int id = UserDefault::getInstance()->getIntegerForKey("bgmId", -1);
        if (id != -1) {
            AudioEngine::setVolume(id, vol); // Cập nhật volume mượt mà
        }
        UserDefault::getInstance()->setFloatForKey("MusicVolume", vol);
        lblVolPercent->setString(to_string((int)(vol * 100)) + "%");
        };

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this, thumb, barStart, barEnd, barWidth, applyVolume](Touch* touch, Event* event) {
        if (!_tabAudioLayer->isVisible()) return false;

        Vec2 pos = touch->getLocation();
        Rect touchArea(barStart.x - 15.0f, barStart.y - 20.0f, barWidth + 30.0f, 40.0f);

        if (touchArea.containsPoint(pos)) {
            float newX = pos.x;
            if (newX < barStart.x) newX = barStart.x;
            if (newX > barEnd.x) newX = barEnd.x;
            thumb->setPositionX(newX);
            thumb->setColor(Color3B::YELLOW);

            float vol = (newX - barStart.x) / barWidth;
            applyVolume(vol);
            return true;
        }
        return false;
        };

    touchListener->onTouchMoved = [thumb, barStart, barEnd, barWidth, applyVolume](Touch* touch, Event* event) {
        float newX = touch->getLocation().x;
        if (newX < barStart.x) newX = barStart.x;
        if (newX > barEnd.x) newX = barEnd.x;
        thumb->setPositionX(newX);

        float vol = (newX - barStart.x) / barWidth;
        applyVolume(vol);
        };

    touchListener->onTouchEnded = [thumb](Touch* touch, Event* event) {
        thumb->setColor(Color3B::WHITE);
        UserDefault::getInstance()->flush();
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, thumb);

    auto lblMusic = Label::createWithTTF("Select Menu Background Music:", "fonts/alagard.ttf", 32);
    lblMusic->setPosition(Vec2(centerX, centerY - 30.0f));
    lblMusic->setScale(0.5f);
    _tabAudioLayer->addChild(lblMusic);

    vector<string> musicFiles = {
        "Audio/Music/Alex_Morgan_Chillhop_Jazz_Sunny_Cafe.mp3",
        "Audio/Music/Kaazoom_The_Market_Square_Daytime.mp3",
        "Audio/Music/Kaazoom_The_Market_Square_Night.mp3"
    };
    vector<string> musicNames = { "Sunny Cafe", "Market Day", "Market Night" };

    for (int i = 0; i < musicNames.size(); i++) {
        auto btnMusic = Button::create("UI/BTN/BG_BTN.png");
        btnMusic->setTitleText(musicNames[i]);
        btnMusic->setTitleFontName("fonts/04B_03__.ttf");
        btnMusic->setTitleFontSize(28);
        btnMusic->setScale(0.22f);
        btnMusic->setPosition(Vec2(centerX - 110.0f + (i * 110.0f), centerY - 70.0f));

        string selectedMusic = musicFiles[i];
        btnMusic->addClickEventListener([selectedMusic](Ref*) {
            float vol = UserDefault::getInstance()->getFloatForKey("MusicVolume", 1.0f);

            int currentId = UserDefault::getInstance()->getIntegerForKey("bgmId", -1);
            if (currentId != -1) {
                AudioEngine::stop(currentId);
            }
            int newId = AudioEngine::play2d(selectedMusic, true, vol);
            UserDefault::getInstance()->setIntegerForKey("bgmId", newId);

            UserDefault::getInstance()->setStringForKey("MenuMusic", selectedMusic);
            UserDefault::getInstance()->flush();
            });
        _tabAudioLayer->addChild(btnMusic);
    }
}

void SettingScene::buildGamePlayTab()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2.0f;
    float centerY = origin.y + visibleSize.height / 2.0f;

    auto lblTitle = Label::createWithTTF("-- GAME PLAY --", "fonts/04B_03__.ttf", 36);
    lblTitle->setPosition(Vec2(centerX, centerY + 85.0f));
    lblTitle->setColor(Color3B::YELLOW);
    lblTitle->setScale(0.6f);
    _tabGamePlayLayer->addChild(lblTitle);

    vector<string> colorNames = { "Brown", "Red", "Blue", "Pink" };
    vector<string> framePaths = { "UI/Frame_Brown.png", "UI/Frame_Red.png", "UI/Frame_Blue.png", "UI/Frame_Pink.png" };

    _tempPlayerFrame = UserDefault::getInstance()->getStringForKey("PlayerFrameColor", "UI/Frame_Blue.png");
    _tempEnemyFrame = UserDefault::getInstance()->getStringForKey("EnemyFrameColor", "UI/Frame_Red.png");

    _saveStatusLabel = Label::createWithTTF("", "fonts/04B_03__.ttf", 12);
    _saveStatusLabel->setColor(Color3B::GREEN);
    _saveStatusLabel->setPosition(Vec2(centerX, centerY - 70.0f));
    _tabGamePlayLayer->addChild(_saveStatusLabel);

    auto previewPlayer = Sprite::create(_tempPlayerFrame);
    if (previewPlayer) {
        previewPlayer->setPosition(Vec2(centerX - 120.0f, centerY + 10.0f));
        previewPlayer->setScale(0.8f);
        _tabGamePlayLayer->addChild(previewPlayer);
    }

    auto previewEnemy = Sprite::create(_tempEnemyFrame);
    if (previewEnemy) {
        previewEnemy->setPosition(Vec2(centerX - 120.0f, centerY - 60.0f));
        previewEnemy->setScale(0.8f);
        _tabGamePlayLayer->addChild(previewEnemy);
    }

    auto lblPlayer = Label::createWithTTF("Player Team Frame:", "fonts/alagard.ttf", 32);
    lblPlayer->setPosition(Vec2(centerX - 120.0f, centerY + 45.0f));
    lblPlayer->setScale(0.5f);
    _tabGamePlayLayer->addChild(lblPlayer);

    for (int i = 0; i < colorNames.size(); i++) {
        auto btn = Button::create("UI/BTN/BG_BTN.png");
        btn->setTitleText(colorNames[i]);
        btn->setTitleFontName("fonts/04B_03__.ttf");
        btn->setTitleFontSize(28);
        btn->setScale(0.16f);
        btn->setPosition(Vec2(centerX - 30.0f + (i * 55.0f), centerY + 25.0f));

        string path = framePaths[i];

        btn->addClickEventListener([this, previewPlayer, previewEnemy, path](Ref*) {

            if (path == _tempEnemyFrame) {
                _tempEnemyFrame = _tempPlayerFrame;
                if (previewEnemy) previewEnemy->setTexture(_tempEnemyFrame);
            }

            if (previewPlayer) previewPlayer->setTexture(path);
            _tempPlayerFrame = path;
            _saveStatusLabel->setString("");
            });
        _tabGamePlayLayer->addChild(btn);
    }

    auto lblEnemy = Label::createWithTTF("Enemy Team Frame:", "fonts/alagard.ttf", 32);
    lblEnemy->setPosition(Vec2(centerX - 120.0f, centerY - 25.0f));
    lblEnemy->setScale(0.5f);
    _tabGamePlayLayer->addChild(lblEnemy);

    for (int i = 0; i < colorNames.size(); i++) {
        auto btn = Button::create("UI/BTN/BG_BTN.png");
        btn->setTitleText(colorNames[i]);
        btn->setTitleFontName("fonts/04B_03__.ttf");
        btn->setTitleFontSize(28);
        btn->setScale(0.16f);
        btn->setPosition(Vec2(centerX - 30.0f + (i * 55.0f), centerY - 45.0f));

        string path = framePaths[i];

        btn->addClickEventListener([this, previewEnemy, previewPlayer, path](Ref*) {

            if (path == _tempPlayerFrame) {
                _tempPlayerFrame = _tempEnemyFrame; 
                if (previewPlayer) previewPlayer->setTexture(_tempPlayerFrame);
            }

            if (previewEnemy) previewEnemy->setTexture(path);
            _tempEnemyFrame = path;
            _saveStatusLabel->setString("");
            });
        _tabGamePlayLayer->addChild(btn);
    }

    auto btnConfirm = Button::create("UI/BTN/BG_BTN.png");
    btnConfirm->setTitleText("CONFIRM");
    btnConfirm->setTitleFontName("fonts/04B_03__.ttf");
    btnConfirm->setTitleFontSize(32);
    btnConfirm->setScale(0.22f);
    btnConfirm->setPosition(Vec2(centerX, centerY - 95.0f));

    btnConfirm->addClickEventListener([this](Ref*) {
        UserDefault::getInstance()->setStringForKey("PlayerFrameColor", _tempPlayerFrame);
        UserDefault::getInstance()->setStringForKey("EnemyFrameColor", _tempEnemyFrame);
        UserDefault::getInstance()->flush();

        _saveStatusLabel->setString("Saved Frame Colors!");
        });
    _tabGamePlayLayer->addChild(btnConfirm);

    auto btnFullScreen = Button::create("UI/BTN/BG_BTN.png");
    btnFullScreen->setTitleText("FULL SCREEN");
    btnFullScreen->setTitleFontName("fonts/04B_03__.ttf");
    btnFullScreen->setTitleFontSize(32);
    btnFullScreen->setScale(0.22f);
    btnFullScreen->setPosition(Vec2(centerX - 150.0f, centerY - 130.0f));
    btnFullScreen->addClickEventListener([](Ref*) {
        auto director = Director::getInstance();
        auto glview = dynamic_cast<GLViewImpl*>(director->getOpenGLView());
        if (glview) {
            glview->setFullscreen();
        }
        });
    _tabGamePlayLayer->addChild(btnFullScreen);

    auto btnWindowed = Button::create("UI/BTN/BG_BTN.png");
    btnWindowed->setTitleText("WINDOWED");
    btnWindowed->setTitleFontName("fonts/04B_03__.ttf");
    btnWindowed->setTitleFontSize(32);
    btnWindowed->setScale(0.22f);
    btnWindowed->setPosition(Vec2(centerX - 50.0f, centerY - 130.0f));
    btnWindowed->addClickEventListener([](Ref*) {
        auto director = Director::getInstance();
        auto glview = dynamic_cast<GLViewImpl*>(director->getOpenGLView());
        if (glview) {
            glview->setWindowed(1600, 880);
        }
        });
    _tabGamePlayLayer->addChild(btnWindowed);
}

void SettingScene::buildPlayerTab()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float centerX = origin.x + visibleSize.width / 2.0f;
    float centerY = origin.y + visibleSize.height / 2.0f;

    auto lblTitle = Label::createWithTTF("-- PLAYER STATS --", "fonts/04B_03__.ttf", 36);
    lblTitle->setPosition(Vec2(centerX, centerY + 80.0f));
    lblTitle->setColor(Color3B::YELLOW);
    lblTitle->setScale(0.6f);
    _tabPlayerLayer->addChild(lblTitle);

    PlayerData p = _controller.loadPlayer();

    int cardCount = _controller.getOwnedCardsCount(p.id);
    int highestEndless = _controller.getHighestEndlessFloor(p.id);

    float startY = centerY + 30.0f;
    float gapY = 25.0f;

    auto lblName = Label::createWithTTF("Username: " + p.username, "fonts/alagard.ttf", 32);
    lblName->setPosition(Vec2(centerX, startY));
    lblName->setScale(0.5f);
    _tabPlayerLayer->addChild(lblName);

    auto lblCards = Label::createWithTTF("Total Owned Cards: " + to_string(cardCount), "fonts/alagard.ttf", 32);
    lblCards->setPosition(Vec2(centerX, startY - gapY));
    lblCards->setScale(0.5f);
    _tabPlayerLayer->addChild(lblCards);

    auto lblStage = Label::createWithTTF("Highest Campaign Stage: Stage " + to_string(p.current_stage), "fonts/alagard.ttf", 32);
    lblStage->setPosition(Vec2(centerX, startY - gapY * 2));
    lblStage->setScale(0.5f);
    _tabPlayerLayer->addChild(lblStage);

    auto lblEndless = Label::createWithTTF("Highest Endless Floor: Floor " + to_string(highestEndless), "fonts/alagard.ttf", 32);
    lblEndless->setPosition(Vec2(centerX, startY - gapY * 3));
    lblEndless->setScale(0.5f);
    _tabPlayerLayer->addChild(lblEndless);
}