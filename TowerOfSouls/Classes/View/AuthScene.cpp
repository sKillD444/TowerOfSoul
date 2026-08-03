#include "AuthScene.h"
#include <cctype>

USING_NS_CC;

Scene* AuthScene::createScene() {
    return AuthScene::create();
}

bool AuthScene::init() {
    if (!Scene::init()) return false;

    auto bg = LayerColor::create(Color4B(25, 35, 50, 255));
    this->addChild(bg, static_cast<int>(ZOrder::BG));

    setupUI();
    return true;
}

bool AuthScene::isValidPassword(const string& password) {
    if (password.length() < 8) return false;
    bool hasLetter = false;
    for (char c : password) {
        if (isalpha(c)) {
            hasLetter = true;
            break;
        }
    }
    return hasLetter;
}

void AuthScene::setupUI() {
    Vec2 centerPos = Responsive::getPos(0.5f, 0.5f);
    float boardW = Responsive::getSize(300.0f);
    float boardH = Responsive::getSize(220.0f);

    auto bgBoard = DrawNode::create();
    Vec2 bgOrigin(centerPos.x - boardW / 2.0f, centerPos.y - boardH / 2.0f);
    Vec2 bgDest(centerPos.x + boardW / 2.0f, centerPos.y + boardH / 2.0f);
    bgBoard->drawSolidRect(bgOrigin, bgDest, Color4F(0.0f, 0.0f, 0.0f, 0.8f));
    bgBoard->drawRect(bgOrigin, bgDest, Color4F(1.0f, 0.8f, 0.0f, 1.0f));
    this->addChild(bgBoard, static_cast<int>(ZOrder::BG));

    float stepY = boardH / 7.0f;
    float boxHeight = Responsive::getSize(35.0f);

    _titleLabel = Label::createWithTTF("LOGIN", "fonts/alagard.ttf", Responsive::getSize(24));
    _titleLabel->setPosition(Vec2(centerPos.x, centerPos.y + boardH / 2.0f - stepY));
    _titleLabel->setColor(Color3B::YELLOW);
    this->addChild(_titleLabel, static_cast<int>(ZOrder::UI));

    _usernameBox = EditBox::create(Size(boardW * 0.8f, boxHeight), Scale9Sprite::create());
    _usernameBox->setPosition(Vec2(centerPos.x, centerPos.y + stepY * 0.8f));
    _usernameBox->setPlaceHolder("Username");
    _usernameBox->setFontSize(Responsive::getSize(14));
    _usernameBox->setMaxLength(20);
    _usernameBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    this->addChild(_usernameBox, static_cast<int>(ZOrder::UI));

    _passwordBox = EditBox::create(Size(boardW * 0.8f, boxHeight), Scale9Sprite::create());
    _passwordBox->setPosition(Vec2(centerPos.x, centerPos.y - stepY * 0.3f));
    _passwordBox->setPlaceHolder("Password");
    _passwordBox->setFontSize(Responsive::getSize(14));
    _passwordBox->setMaxLength(30);
    _passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    _passwordBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    this->addChild(_passwordBox, static_cast<int>(ZOrder::UI));

    _statusLabel = Label::createWithSystemFont("", "Arial", Responsive::getSize(12));
    _statusLabel->setPosition(Vec2(centerPos.x, centerPos.y - stepY * 1.3f));
    _statusLabel->setColor(Color3B::RED);
    this->addChild(_statusLabel, static_cast<int>(ZOrder::UI));

    _actionButton = Button::create("UI/BTN/BG_BTN.png");
    _actionButton->setTitleText("LOGIN");
    _actionButton->setScale(Responsive::getScale(0.2f));
    _actionButton->setTitleFontSize(Responsive::getSize(32));
    _actionButton->setPosition(Vec2(centerPos.x, centerPos.y - stepY * 2.0f));
    _actionButton->addClickEventListener([this](Ref*) { this->onActionClicked(); });
    this->addChild(_actionButton, static_cast<int>(ZOrder::UI));

    _switchModeButton = Button::create();
    _switchModeButton->setTitleText("No account? Register now");
    _switchModeButton->setTitleFontSize(Responsive::getSize(12));
    _switchModeButton->setPosition(Vec2(centerPos.x, centerPos.y - boardH / 2.0f + stepY * 0.7f));
    _switchModeButton->addClickEventListener([this](Ref*) { this->switchMode(); });
    this->addChild(_switchModeButton, static_cast<int>(ZOrder::UI));
}

void AuthScene::switchMode() {
    _isLoginMode = !_isLoginMode;
    _statusLabel->setString("");

    if (_isLoginMode) {
        _titleLabel->setString("LOGIN");
        _actionButton->setTitleText("LOGIN");
        _switchModeButton->setTitleText("No account? Register now");
    }
    else {
        _titleLabel->setString("REGISTER");
        _actionButton->setTitleText("REGISTER");
        _switchModeButton->setTitleText("Have an account? Login here");
    }
}

void AuthScene::onActionClicked() {
    string username = _usernameBox->getText();
    string password = _passwordBox->getText();

    if (username.empty() || password.empty()) {
        _statusLabel->setColor(Color3B::RED);
        _statusLabel->setString("Please fill in all the required information!");
        return;
    }

    if (_isLoginMode) {
        int userId = _pController.loginAccount(username, password);
        if (userId != -1) {
            _statusLabel->setColor(Color3B::GREEN);
            _statusLabel->setString("Login successful!");
            cocos2d::UserDefault::getInstance()->setIntegerForKey("CURRENT_USER_ID", userId);
            cocos2d::UserDefault::getInstance()->flush();
            auto scene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
        }
        else {
            _statusLabel->setColor(Color3B::RED);
            _statusLabel->setString("Invalid username or password!");
        }
    }
    else {
        if (!isValidPassword(password)) {
            _statusLabel->setColor(Color3B::RED);
            _statusLabel->setString("Password > 8 chars & must contain a letter!");
            return;
        }

        bool success = _pController.registerAccount(username, password);
        if (success) {
            _statusLabel->setColor(Color3B::GREEN);
            _statusLabel->setString("Registered successfully! Please login.");
            switchMode();
        }
        else {
            _statusLabel->setColor(Color3B::RED);
            _statusLabel->setString("Username already exists!");
        }
    }
}