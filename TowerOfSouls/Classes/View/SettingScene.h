#pragma once
#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "View/MenuScene.h"
#include "Controller/PlayerController.h"
#include "audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;
using namespace cocos2d::experimental;

class SettingScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

private:
    PlayerController _controller;

    cocos2d::Node* _tabAudioLayer;
    cocos2d::Node* _tabGamePlayLayer;
    cocos2d::Node* _tabPlayerLayer;

    string _tempPlayerFrame;
    string _tempEnemyFrame;
    cocos2d::Label* _saveStatusLabel;

    void createTabsMenu();
    void buildAudioTab();
    void buildGamePlayTab();
    void buildPlayerTab();
    void backHome();

    void switchTab(int tabIndex);

    CREATE_FUNC(SettingScene);
};

#endif // __SETTING_SCENE_H__