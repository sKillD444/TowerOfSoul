#pragma once
#ifndef __AUTH_SCENE_H__
#define __AUTH_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "View/MenuScene.h"
#include "Controller/PlayerController.h"
#include "View/ResponsiveHelper.h"
#include <iostream>
using namespace std;
using namespace cocos2d::ui;

class AuthScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    enum class ZOrder { BG, UI, Popup };

    CREATE_FUNC(AuthScene);

private:
    PlayerController _pController;
    bool _isLoginMode = true; 

    EditBox* _usernameBox;
    EditBox* _passwordBox;
    cocos2d::Label* _titleLabel;
    Button* _actionButton;
    Button* _switchModeButton;
    cocos2d::Label* _statusLabel;

    void setupUI();
    void switchMode();
    void onActionClicked();

    bool isValidPassword(const string& password);
};

#endif // __AUTH_SCENE_H__