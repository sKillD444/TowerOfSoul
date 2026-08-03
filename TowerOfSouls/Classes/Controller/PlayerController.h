#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include <iostream>
using namespace std;
#include "../GameData.h"
#include "cocos2d.h"
#include "../Model/PlayerModel.h"


class PlayerController {
public:
	PlayerData loadPlayer() {
		int currentUserId = cocos2d::UserDefault::getInstance()->getIntegerForKey("CURRENT_USER_ID", -1);

		if (currentUserId != -1) {
			return PlayerModel::getInstance().loadUser(currentUserId);
		}

		PlayerData defaultData;
		defaultData.id = -1;
		defaultData.username = "Guest";
		defaultData.gold = 0;
		defaultData.gems = 0;
		defaultData.current_stage = 1;
		return defaultData;
	}

	bool updateGems(int userID,int newGems) {
		return PlayerModel::getInstance().updateGem(newGems, userID);
	}
	bool updateGold(int userID,int newGold) {
		return PlayerModel::getInstance().updateGold(newGold, userID);
	}

	bool registerAccount(const string& username, const string& password) {
		return PlayerModel::getInstance().registerUser(username, password);
	}

	int loginAccount(const string& username, const string& password) {
		return PlayerModel::getInstance().loginUser(username, password);
	}

	void logout() {
		cocos2d::UserDefault::getInstance()->setIntegerForKey("CURRENT_USER_ID", -1);
		cocos2d::UserDefault::getInstance()->flush();
	}

	int getOwnedCardsCount(int userID) {
		return PlayerModel::getInstance().getOwnedCardsCount(userID);
	}

	int getHighestEndlessFloor(int userID) {
		return PlayerModel::getInstance().getHighestEndlessFloor(userID);
	}

	bool savePlayerTeam(const string& teamType, const vector<int>& ownedCardIDs) {
		int currentUserId = cocos2d::UserDefault::getInstance()->getIntegerForKey("CURRENT_USER_ID", -1);
		if (currentUserId == -1) return false;
		return PlayerModel::getInstance().saveUserTeam(currentUserId, teamType, ownedCardIDs);
	}

	vector<int> loadPlayerTeam(const string& teamType) {
		int currentUserId = cocos2d::UserDefault::getInstance()->getIntegerForKey("CURRENT_USER_ID", -1);
		if (currentUserId == -1) return vector<int>();
		return PlayerModel::getInstance().loadUserTeam(currentUserId, teamType);
	}

	bool updateCurrentStage(int userID, int newStage) {
		return PlayerModel::getInstance().updateCurrentStage(newStage, userID);
	}
};

#endif