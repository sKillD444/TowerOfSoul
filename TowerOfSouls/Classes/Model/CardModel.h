#ifndef __CARD_MODEL_H__
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
    vector<BattleCardData> getRNDCards(const vector<int>& costs);
    bool buyCards(int userId, const vector<BattleCardData>& cards);
	bool buyCard(int userId, const BattleCardData& cardData);
    bool upgradeCardLevel(int ownedCardId, int newLevel, int bonusHp, int bonusAtk);
    vector<BattleCardData> spawnEnemies(int count);
    vector<pair<BattleCardData, int>> getCampaignEnemies(int stageId, int wave);
    int getStageGoldReward(int stageNumber);
};

#endif