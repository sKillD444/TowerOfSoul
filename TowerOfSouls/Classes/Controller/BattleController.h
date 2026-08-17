#ifndef __BATTLE_CONTROLLER_H__
#define __BATTLE_CONTROLLER_H__

#include <iostream>
using namespace std;
#include "../GameData.h"
#include "../Model/CardModel.h"

enum class BattleState {
    PLAYING,
    WIN,
    LOSE
};

class BattleController {
public:
	vector<BattleCardData> loadPlayerDeck() {
		int currentUserId = cocos2d::UserDefault::getInstance()->getIntegerForKey("CURRENT_USER_ID", -1);
		if (currentUserId != -1) {
			return CardModel::getInstance().getPlayerDeck(currentUserId);
		}
		return vector<BattleCardData>();
	}

	vector<BattleCardData> getShopRoll(int round) {
        return CardModel::getInstance().getShopRoll(round);
    }

    bool buyCard(int userID, const BattleCardData& cardData) {
        return CardModel::getInstance().buyCard(userID, cardData);
    }

	vector<BattleCardData> spawnEnemies(int count) {
		return CardModel::getInstance().spawnEnemies(count);
	}

	vector<BattleCardData> getRNDCards(const vector<int>& costs) {
		return CardModel::getInstance().getRNDCards(costs);
	}

	bool buyCards(int userID, const vector<BattleCardData>& cards) {
		return CardModel::getInstance().buyCards(userID, cards);
	}

	BattleCardData getRNDCard(int cost) {
		return CardModel::getInstance().getRNDCard(cost);
	}

	float culateDamage(const BattleCardData& attacker, const BattleCardData& defender) {
		float dmg = attacker.atk;
		if ((attacker.role == "Warrior" && defender.role == "Defender")
			||(attacker.role == "Defender" && defender.role == "Assassin") 
			||(attacker.role == "Assassin" && defender.role == "Ranged DPS")
			||(attacker.role == "Ranged DPS" && defender.role == "Warrior")){
			dmg=dmg * 1.3;
		}
		if((defender.role=="Warrior"&& attacker.role == "Defender")
			||(defender.role == "Defender" && attacker.role == "Assassin")
			||(defender.role == "Assassin" && attacker.role == "Ranged DPS")
			||(defender.role == "Ranged DPS" && attacker.role == "Warrior"))
			dmg = dmg * 0.7;
		return dmg;
	}

	BattleState checkBattleState(int alivePlayers, int aliveEnemys) {
		if (alivePlayers > 0 && aliveEnemys == 0) {
			return BattleState::WIN;
		}
		else if (alivePlayers == 0 && aliveEnemys > 0) {
			return BattleState::LOSE;
		}
		else {
			return BattleState::PLAYING;
		}
	}

	void updateStar(BattleCardData& card) {
		switch (card.star)
		{
		case 2:
			card.atk += 10;
			card.hp += 30;
			card.currentHp = card.hp;
			break;
		case 3:
			card.atk += 20;
			card.hp += 40;
			card.currentHp = card.hp;
			break;
		case 4:
			card.atk += 35;
			card.hp += 55;
			card.currentHp = card.hp;
			break;
		case 5:
			card.atk += 50;
			card.hp += 70;
			card.currentHp = card.hp;
			break;
		}
	}

	int getUpgradeCost(int currentLevel) {
		if (currentLevel >= 5) return -1; 
		return 10 *pow(2, currentLevel);
	}

	bool upgradeCard(int userId, BattleCardData& card, int& playerGold) {
		int cost = getUpgradeCost(card.level);
		if (cost == -1 || playerGold < cost) return false;

		int bonusHp = 30;
		int bonusAtk = 10;

		bool success = CardModel::getInstance().upgradeCardLevel(card.id, card.level + 1, bonusHp, bonusAtk);
		if (success) {
			card.level += 1;
			card.hp += bonusHp;
			card.currentHp += bonusHp;
			card.atk += bonusAtk;
			return true;
		}
		return false;
	}

	vector<pair<BattleCardData, int>> getCampaignEnemies(int stageId, int wave) {
		return CardModel::getInstance().getCampaignEnemies(stageId, wave);
	}

	int getStageGoldReward(int stageNumber) {
		return CardModel::getInstance().getStageGoldReward(stageNumber);
	}
};

#endif