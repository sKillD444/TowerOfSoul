#include "CardModel.h"
#include "../mysql_api.h" 

vector<BattleCardData> CardModel::getPlayerDeck(int userId) {
    vector<BattleCardData> deck;
    auto& db = MySQLCli::getInstance();

    string sql = "SELECT o.id, c.card_id, c.name, o.level, o.star, "
        "(c.base_hp + o.evolve_hp_bonus) as hp, "
        "(c.base_atk + o.evolve_atk_bonus) as atk "
        "FROM owned_cards o "
        "JOIN card_definitions c ON o.card_id = c.card_id "
        "WHERE o.user_id = " + to_string(userId) + " LIMIT 5";

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
            deck.push_back(c);
        }
        delete res;
    }
    return deck;
}

vector<BattleCardData> CardModel::getShopRoll(int round) {
    vector<BattleCardData> shop;

    int star = 1;

	for (int i = 0; i < 5; i++){
	    int cost = getCostByRound(round);
        auto& db = MySQLCli::getInstance();
        string sql = "SELECT id, card_id, name, base_hp, base_atk, cost, role FROM card_definitions WHERE card_type='PLAYER' AND cost=" + to_string(cost) + "   ORDER BY RAND() LIMIT 1";

        DBResult* res = db.query(sql);
        if (res) {
            char** row;
            while ((row = res->fetchRow()) != nullptr) {
                BattleCardData c;
                c.id = stoi(row[0]);
                c.card_id = row[1];
                c.name = row[2];
                c.level = 1;

                int roll = rand() % 100;

                if (round > 9) {
                    if (roll >= 90) {
                        star = 3;
                    }
                    else if (roll >= 60) {
                        star = 2;
                    }
                    else {
                        star = 1;
                    }
                }
                else if (round > 4) {
                    if (roll > 70) {
                        star = 2;
                    }
                    else {
                        star = 1;
                    }
                }
                c.star = star;
                c.hp = stoi(row[3]);
                c.atk = stoi(row[4]);
                c.cost = stoi(row[5]) * star;
                c.role = row[6];
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
    string sql = "SELECT id, card_id, name, base_hp, base_atk, cost, role FROM card_definitions WHERE card_type='PLAYER' AND cost="+to_string(cost)+ " ORDER BY RAND() LIMIT 1";
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

	string sql = "SELECT id, card_id, name, base_hp, base_atk,role FROM card_definitions  WHERE card_type='MONSTER' ORDER BY RAND() LIMIT "+ std::to_string(count);

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
            enemyDeck.push_back(c);
        }
        delete res;
    }
    return enemyDeck;
}