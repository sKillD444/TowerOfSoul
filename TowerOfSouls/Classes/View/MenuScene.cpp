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
	// ==========================================
	return true;
}

void MenuScene::createButtons() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x = origin.x + visibleSize.width * 0.10f;
	float y = origin.y + visibleSize.height * 0.75f;

	auto campaignLabel = Label::createWithTTF("Campaign", "fonts/alagard.ttf", 14);
	campaignLabel->enableBold();
	auto campainItem = MenuItemLabel::create(campaignLabel, [](Ref*) {

		});
	campainItem->setColor(Color3B(240, 235, 220));
	campainItem->setPosition(Vec2(x, y));

	auto EndlessLabel = Label::createWithTTF("Endless", "fonts/alagard.ttf", 14);
	EndlessLabel->enableBold();
	auto EndlessItem = MenuItemLabel::create(EndlessLabel, [](Ref*) {
		auto sceneEndless = BattleScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sceneEndless, Color3B(0, 0, 0)));
		});
	EndlessItem->setColor(Color3B(240, 235, 220));
	EndlessItem->setPosition(Vec2(x, y - 30.0f));

	auto teamLabel = Label::createWithTTF("Team", "fonts/alagard.ttf", 14);
	teamLabel->enableBold();
	auto teamItem = MenuItemLabel::create(teamLabel, [](Ref*) {
		auto sceneTeam = TeamScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sceneTeam, Color3B(0, 0, 0)));
		});
	teamItem->setColor(Color3B(240, 235, 220));
	teamItem->setPosition(Vec2(x, y - 60.0f));

	auto shopLabel = Label::createWithTTF("Shop", "fonts/alagard.ttf", 14);
	shopLabel->enableBold();
	auto shopItem = MenuItemLabel::create(shopLabel, [](Ref*) {
		auto sceneShop = ShopScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sceneShop, Color3B(0, 0, 0)));
		});
	shopItem->setColor(Color3B(240, 235, 220));
	shopItem->setPosition(Vec2(x, y - 90.0f));

	auto rankLabel = Label::createWithTTF("Leaderboard", "fonts/alagard.ttf", 14);
	rankLabel->enableBold();
	auto rankItem = MenuItemLabel::create(rankLabel, [](Ref*) {
		auto sceneEndless = LeaderboardScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, sceneEndless, Color3B(0, 0, 0)));
		});
	rankItem->setColor(Color3B(240, 235, 220));
	rankItem->setPosition(Vec2(x, y - 120.0f));

	auto settingLable = Label::createWithTTF("Settings", "fonts/alagard.ttf", 14);
	settingLable->enableBold();
	auto settingItem = MenuItemLabel::create(settingLable, [](Ref*) {

		});
	settingItem->setColor(Color3B(240, 235, 220));
	settingItem->setPosition(Vec2(x, y - 150.0f));

	auto logOutLabel = Label::createWithTTF("Log Out", "fonts/alagard.ttf", 14);
	logOutLabel->enableBold();
	auto logOutItem = MenuItemLabel::create(logOutLabel, [](Ref*) {

		});
	logOutItem->setColor(Color3B(240, 235, 220));
	logOutItem->setPosition(Vec2(x, y - 180.0f));

	auto menu = Menu::create(campainItem, EndlessItem, teamItem, shopItem, rankItem, settingItem, logOutItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, static_cast<int>(ZOrder::Button));
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

