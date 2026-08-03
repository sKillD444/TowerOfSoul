#include "CardModel.h"
#include "../mysql_api.h" 

vector<BattleCardData> CardModel::getPlayerDeck(int userId) {
    vector<BattleCardData> deck;
    auto& db = MySQLCli::getInstance();

    string sql = "SELECT o.id, c.card_id, c.name, o.level, o.star, (c.base_hp + o.evolve_hp_bonus) as hp, (c.base_atk + o.evolve_atk_bonus) as atk, c.role, c.card_type "
        "FROM owned_cards o "
        "JOIN card_definitions c ON o.card_id = c.card_id "
        "WHERE o.user_id = " + to_string(userId);

    DBResult* res = db.query(sql);
    if (res) {
        char** row;
        while ((row = res->fetchRow()) != nullptr) {
            BattleCardData c;
            c.id = stoi(row[0]);
            c.card_id = row[1];
            c.name = row[2];
            c.level = stoi(row[3]);
            c.star =stoi(row[4]);
            c.hp =stoi(row[5]);
            c.atk =stoi(row[6]);
            c.role = row[7];
            c.type = row[8];
            deck.push_back(c);
        }
        delete res;
    }
    return deck;
}

vector<BattleCardData> CardModel::getShopRoll(int round) {
    vector<BattleCardData> shop;
	for (int i = 0; i < 5; i++){
	    int cost = getCostByRound(round);
        auto& db = MySQLCli::getInstance();
        string sql = "SELECT id, card_id, name, base_hp, base_atk, cost, role,card_type FROM card_definitions WHERE card_type='PLAYER' AND cost=" + to_string(cost) + "   ORDER BY RAND() LIMIT 1";

        DBResult* res = db.query(sql);
        if (res) {
            char** row;
            while ((row = res->fetchRow()) != nullptr) {
                BattleCardData c;
                c.id = stoi(row[0]);
                c.card_id = row[1];
                c.name = row[2];
                c.level = 1;
                c.star = 1;
                c.hp = stoi(row[3]);
                c.atk = stoi(row[4]);
                c.cost = stoi(row[5]);
                c.role = row[6];
                c.type = row[7];
                shop.push_back(c);
            }
            delete res;
        }
    }
    return shop;
}

int CardModel::getCostByRound(int round) {
    int rollCost = rand() % 100 + 1;
    if (round > 20) {
        if (rollCost >= 80) {
            return 5;
        }
        else if (rollCost >= 60) {
            return 4;
        }
        else if (rollCost >= 40) {
            return 3;
        }
        else if (rollCost >= 10) {
            return 2;
        }
        else {
            return 1;
        }
    }
    else if (round > 15) {
        if (rollCost >= 90) {
            return 5;
        }
        else if (rollCost >= 70) {
            return 4;
        }
        else if (rollCost >= 50) {
            return 3;
        }
        else if (rollCost >= 30) {
            return 2;
        }
        else {
            return 1;
        }
	}
	else if (round > 10) {
        if (rollCost >= 95) {
            return 5;
        }
        else if (rollCost >= 80) {
            return 4;
        }
        else if (rollCost >= 60) {
            return 3;
        }
        else if (rollCost >= 40) {
            return 2;
        }
        else {
            return 1;
        }
	}
    else {
        if (rollCost >= 99) {
            return 5;
        }
        else if (rollCost >= 90) {
            return 4;
        }
        else if (rollCost >= 70) {
            return 3;
        }
        else if (rollCost >= 50) {
            return 2;
        }
        else {
            return 1;
        }
    }
}

BattleCardData CardModel::getRNDCard(int cost) {
    BattleCardData c;
    auto& db = MySQLCli::getInstance();
    string sql = "SELECT id, card_id, name, base_hp, base_atk, cost, role,card_type FROM card_definitions WHERE card_type='PLAYER' AND cost="+to_string(cost)+ " ORDER BY RAND() LIMIT 1";
    DBResult* res = db.query(sql);
    if (res) {
        char** row;
        while ((row = res->fetchRow()) != nullptr) {
            c.id = stoi(row[0]);
            c.card_id = row[1];
            c.name = row[2];
            c.level = 1; c.star = 1;
            c.hp = stoi(row[3]);
            c.atk = stoi(row[4]);
            c.cost = stoi(row[5]);
            c.role = row[6];
            c.type = row[7];
        }
    }
    return c;
}

bool CardModel::buyCard(int userId, const BattleCardData& cardData) {
	auto& db = MySQLCli::getInstance();
    
	string sql = "INSERT INTO owned_cards (user_id, card_id, level, star, evolve_hp_bonus, evolve_atk_bonus) VALUES (" +
		to_string(userId) + ", '" + cardData.card_id + "', 1, 1, 0, 0)";

	return db.execute(sql);
}

vector<BattleCardData> CardModel::spawnEnemies(int count) {
	vector<BattleCardData> enemyDeck;
	auto& db = MySQLCli::getInstance();

	string sql = "SELECT id, card_id, name, base_hp, base_atk,role,card_type FROM card_definitions  WHERE card_type='MONSTER' ORDER BY RAND() LIMIT "+ to_string(count);

	DBResult* res = db.query(sql);
    if (res) {
        char** row;
        while ((row = res->fetchRow()) != nullptr) {
            BattleCardData c;
            c.id = stoi(row[0]);
            c.card_id = row[1];
            c.name = row[2];
            c.level = 1;
            c.star = 1;
            c.hp =stoi(row[3]);
            c.atk = stoi(row[4]);
			c.role = row[5];
            c.type = row[6];
            enemyDeck.push_back(c);
        }
        delete res;
    }
    return enemyDeck;
}

bool CardModel::upgradeCardLevel(int ownedCardId, int newLevel, int bonusHp, int bonusAtk) {
    auto& db = MySQLCli::getInstance();
    string sql = "UPDATE owned_cards SET level = " + to_string(newLevel) +
        ", evolve_hp_bonus = evolve_hp_bonus + " + to_string(bonusHp) +
        ", evolve_atk_bonus = evolve_atk_bonus + " + to_string(bonusAtk) +
        " WHERE id = " + to_string(ownedCardId);
    return db.execute(sql);
}

vector<pair<BattleCardData, int>> CardModel::getCampaignEnemies(int stageId, int wave) {
    vector<pair<BattleCardData, int>> enemies;
    auto& db = MySQLCli::getInstance();

    string sql = "SELECT c.id, c.card_id, c.name, c.base_hp, c.base_atk, c.role, c.card_type, se.position_slot "
        "FROM stage_enemies se "
        "JOIN card_definitions c ON se.card_id = c.card_id "
        "WHERE se.stage_number = " + to_string(stageId) + " AND se.turn_number = " + to_string(wave);

    DBResult* res = db.query(sql);
    if (res) {
        char** row;
        while ((row = res->fetchRow()) != nullptr) {
            BattleCardData c;
            c.id = stoi(row[0]);
            c.card_id = row[1];
            c.name = row[2];
            c.level = 1;
            c.star = 1;
            c.hp = stoi(row[3]);
            c.atk = stoi(row[4]);
            c.role = row[5];
            c.type = row[6];
            c.currentHp = c.hp;

            int slotPos = stoi(row[7]);
            enemies.push_back({ c, slotPos });
        }
        delete res;
    }
    return enemies;
}

int CardModel::getStageGoldReward(int stageNumber) {
    auto& db = MySQLCli::getInstance();
    string sql = "SELECT gold_reward FROM stages WHERE stage_number = " + to_string(stageNumber);
    DBResult* res = db.query(sql);

    int reward = 30; 
    if (res) {
        char** row = res->fetchRow();
        if (row != nullptr && row[0] != nullptr) {
            reward = stoi(row[0]);
        }
        delete res;
    }
    return reward;
}
