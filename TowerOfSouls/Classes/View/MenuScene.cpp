#include "MenuScene.h"
#include "PlayerHUD.h"

USING_NS_CC;
namespace L {

}
Scene* MenuScene::createScene()
{
	return MenuScene::create();
}


bool MenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// ==========================================
	auto bg = Sprite::create("BG/BGMenu.png");
	float centerX = origin.x + visibleSize.width / 2;
	float centerY = origin.y + visibleSize.height / 2;
	bg->setPosition(Vec2(centerX, centerY));
	auto bgSize = bg->getContentSize();
	float scaleX = visibleSize.width / bgSize.width;
	float scaleY = visibleSize.height / bgSize.height;
	bg->setScale(MAX(scaleX, scaleY));
	this->addChild(bg, static_cast<int>(ZOrder::BG));

	this->createButtons();
	this->createUI();
	auto hud = PlayerHUD::create();
	this->addChild(hud, static_cast<int>(ZOrder::Info));

	PlayerData p = _controller.loadPlayer();
	hud->updatePlayerData(p);

	float vol = UserDefault::getInstance()->getFloatForKey("MusicVolume", 1.0f);
	string bgMusic = UserDefault::getInstance()->getStringForKey("MenuMusic", "Audio/Music/Alex_Morgan_Chillhop_Jazz_Sunny_Cafe.mp3");

	int currentId = UserDefault::getInstance()->getIntegerForKey("bgmId", -1);

	if (currentId == -1 || AudioEngine::getState(currentId) != AudioEngine::AudioState::PLAYING) {
		AudioEngine::stopAll();
		int newId = AudioEngine::play2d(bgMusic, true, vol);
		UserDefault::getInstance()->setIntegerForKey("bgmId", newId);
	}
	else {
		AudioEngine::setVolume(currentId, vol);
	}
	// ==========================================
	return true;
}

void MenuScene::createButtons() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float menuX = origin.x + visibleSize.width * 0.12f;
	float startY = origin.y + visibleSize.height * 0.8f;
	float gapY = visibleSize.height * 0.12f;

	struct MenuData { string name; function<void(Ref*)> callback; };
	vector<MenuData> menuItems = {
		{"Campaign", [](Ref*) {
	auto scene = CampaignScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
}},
		{"Endless", [](Ref*) {
			auto scene = BattleScene::createScene(false, 1);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}},
		{"Team", [](Ref*) {
			auto scene = TeamScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}},
		{"Shop", [](Ref*) {
			auto scene = ShopScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}},
		{"Leaderboard", [](Ref*) {
			auto scene = LeaderboardScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}},
		{"Settings", [](Ref*) {
			auto scene = SettingScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}},
		{"Log Out", [this](Ref*) {
			_controller.logout();
			auto scene = AuthScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B(0, 0, 0)));
		}}
	};

	for (size_t i = 0; i < menuItems.size(); ++i) {
		auto btn = ui::Button::create();
		btn->setTitleText(menuItems[i].name);
		btn->setTitleFontName("fonts/alagard.ttf");
		btn->setTitleFontSize(18);
		btn->setTitleColor(Color3B(240, 235, 220));

		btn->setAnchorPoint(Vec2(0.5f, 0.5f));
		btn->setPosition(Vec2(menuX, startY - i * gapY));

		if (menuItems[i].callback) {
			btn->addClickEventListener(menuItems[i].callback);
		}
		this->addChild(btn, static_cast<int>(ZOrder::Button));
	}
}

void MenuScene::createUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float menuBGWidth = visibleSize.width * 0.20f;
	float menuBGHeight = visibleSize.height;
	auto sideMenuBg = LayerGradient::create(Color4B(15, 20, 35, 240), Color4B(15, 20, 35, 0), Vec2(1, 0));
	sideMenuBg->setContentSize(Size(menuBGWidth, menuBGHeight));
	sideMenuBg->setPosition(Vec2::ZERO);
	this->addChild(sideMenuBg, static_cast<int>(ZOrder::BGButton));
}