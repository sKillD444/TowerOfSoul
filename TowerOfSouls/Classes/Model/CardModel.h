#ifndef __BATTLE_MODEL_H__
#define __CARD_MODEL_H__

#include <iostream>
#include <string> 
#include <vector>
#include "../GameData.h"
#include <cstdlib>
#include <ctime>
using namespace std;

class CardModel {
public:
    static CardModel& getInstance() {
        static CardModel instance;
        return instance;
    }

    vector<BattleCardData> getPlayerDeck(int userId);

    vector<BattleCardData> getShopRoll(int round);
    int getCostByRound(int round);
    BattleCardData getRNDCard(int cost);

	bool buyCard(int userId, const BattleCardData& cardData);

    vector<BattleCardData> spawnEnemies(int count);
};

#endif