#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include <iostream>
using namespace std;
#include "../GameData.h"
#include "../Model/PlayerModel.h"


class PlayerController {
public:
	PlayerData loadPlayer() {
		int currentUserId = 1;
		return PlayerModel::getInstance().loadUser(currentUserId);
	}

	bool updateGems(int userID,int newGems) {
		return PlayerModel::getInstance().updateGem(newGems, userID);
	}
	bool updateGold(int userID,int newGold) {
		return PlayerModel::getInstance().updateGold(newGold, userID);
	}
};

#endif