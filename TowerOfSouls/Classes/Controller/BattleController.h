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
        return CardModel::getInstance().getPlayerDeck(1);
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
};

#endif