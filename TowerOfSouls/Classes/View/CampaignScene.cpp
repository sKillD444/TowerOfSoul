#include "CampaignScene.h"


Scene* CampaignScene::createScene() { return CampaignScene::create(); }

bool CampaignScene::init() {
	if (!Scene::init()) return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float centerX = origin.x + visibleSize.width / 2.0f;
	float centerY = origin.y + visibleSize.height / 2.0f;

	auto bg = Sprite::create("BG/BGCampaign.png");
	bg->setPosition(Vec2(centerX, centerY));
	float scaleX = visibleSize.width / bg->getContentSize().width;
	float scaleY = visibleSize.height / bg->getContentSize().height;
	bg->setScale(MAX(scaleX, scaleY));
	this->addChild(bg, 0);

	createStageNodes();
	backHome();
	return true;
}

void CampaignScene::backHome() {
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

void CampaignScene::createStageNodes() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PlayerController pController;
	PlayerData p = pController.loadPlayer();

	int cols = 4;
	float startX = origin.x + visibleSize.width * 0.2f;
	float startY = origin.y + visibleSize.height * 0.15f;

	float gapX = visibleSize.width * 0.2f;
	float gapY = visibleSize.height * 0.3f;  

	for (int i = 1; i <= 12; i++) {
		int row = (i - 1) / cols;
		int col = (i - 1) % cols;

		Vec2 pos(startX + col * gapX, startY + row * gapY);

		string iconPath = "UI/Stage.png";
		if (i == 4) iconPath = "UI/Boss1.png";
		else if (i == 8) iconPath = "UI/Boss2.png";
		else if (i == 12) iconPath = "UI/Boss3.png";

		auto btnStage = Button::create(iconPath);
		btnStage->setPosition(pos);
		btnStage->setScale(0.6f);

		auto lblStageNum = Label::createWithTTF(to_string(i), "fonts/04B_03__.ttf", 24);
		lblStageNum->setPosition(Vec2(btnStage->getContentSize().width / 2, -15));
		lblStageNum->setColor(Color3B::WHITE);
		btnStage->addChild(lblStageNum);

		if (i > p.current_stage) {
			btnStage->setColor(Color3B::GRAY);
			btnStage->setOpacity(150);
			btnStage->addClickEventListener([](Ref*) {});
		}
		else {
			btnStage->setColor(Color3B::WHITE);
			btnStage->addClickEventListener([i](Ref*) {
				auto battleScene = BattleScene::createScene(true, i);
				Director::getInstance()->replaceScene(TransitionFade::create(0.5f, battleScene, Color3B(0, 0, 0)));
			});
		}

		this->addChild(btnStage, 5);
	}
}