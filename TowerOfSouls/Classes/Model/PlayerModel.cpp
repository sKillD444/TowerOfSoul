#include "PlayerModel.h"
#include "../mysql_api.h" 

PlayerData PlayerModel::loadUser(int userID) {
	auto& db = MySQLCli::getInstance();
	string sql = "SELECT u.id, u.username, u.gold, u.gems, IFNULL(MAX(usp.stage_number), 1) as current_stage "
		"FROM users u "
		"LEFT JOIN user_stage_progress usp ON u.id = usp.user_id "
		"WHERE u.id=" + to_string(userID) + " GROUP BY u.id";

	DBResult* res = db.query(sql);

	PlayerData p = { -1, "Unknown", 0, 0, 1 };

	if (res) {
		char** row = res->fetchRow();
		if (row != nullptr) {
			p.id = stoi(row[0]);
			p.username = row[1];
			p.gold = stoi(row[2]);
			p.gems = stoi(row[3]);
			p.current_stage = stoi(row[4]);
		}
		delete res;
	}
	return p;
}

bool PlayerModel::updateGold(int newGold, int userID) {
	auto& db = MySQLCli::getInstance();
	if (newGold > 10000)
		newGold = 10000;
	string sql="Update users set gold="+to_string(newGold)+" where id = "
		+ to_string(userID);
	return db.execute(sql);
}

bool PlayerModel::updateGem(int newGem, int userID) {
	auto& db = MySQLCli::getInstance();
	if (newGem > 10000)
		newGem = 10000;
	string sql = "Update users set gems=" + to_string(newGem) + " where id = "
		+ to_string(userID);
	return db.execute(sql);
}

bool PlayerModel::registerUser(const string& username, const string& password) {
	auto& db = MySQLCli::getInstance();
	string checkSql = "SELECT id FROM users WHERE username = '" + username + "'";
	DBResult* res = db.query(checkSql);
	if (res && res->fetchRow() != nullptr) {
		delete res;
		return false;
	}
	if (res) delete res;

	string insertSql = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
	return db.execute(insertSql);
}

int PlayerModel::loginUser(const string& username, const string& password) {
	auto& db = MySQLCli::getInstance();
	string sql = "SELECT id FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
	DBResult* res = db.query(sql);
	int userId = -1;
	if (res) {
		char** row = res->fetchRow();
		if (row != nullptr) {
			userId = stoi(row[0]);
		}
		delete res;
	}
	return userId;
}

int PlayerModel::getOwnedCardsCount(int userID) {
	auto& db = MySQLCli::getInstance();
	string sql = "SELECT COUNT(*) FROM owned_cards WHERE user_id = " + to_string(userID);
	DBResult* res = db.query(sql);
	int count = 0;
	if (res) {
		char** row = res->fetchRow();
		if (row != nullptr && row[0] != nullptr) {
			count = stoi(row[0]);
		}
		delete res;
	}
	return count;
}

int PlayerModel::getHighestEndlessFloor(int userID) {
	auto& db = MySQLCli::getInstance();
	string sql = "SELECT highest_floor FROM endless_leaderboard WHERE user_id = " + to_string(userID);
	DBResult* res = db.query(sql);
	int floor = 0;
	if (res) {
		char** row = res->fetchRow();
		if (row != nullptr && row[0] != nullptr) {
			floor = stoi(row[0]);
		}
		delete res;
	}
	return floor;
}

bool PlayerModel::saveUserTeam(int userID, const string& teamType, const vector<int>& ownedCardIDs) {
	auto& db = MySQLCli::getInstance();

	string deleteSql = "DELETE FROM user_teams WHERE user_id = " + to_string(userID) +
		" AND team_type = '" + teamType + "'";
	db.execute(deleteSql);

	for (size_t i = 0; i < ownedCardIDs.size(); i++) {
		string insertSql = "INSERT INTO user_teams (user_id, team_type, card_slot_index) VALUES (" +
			to_string(userID) + ", '" + teamType + "', " + to_string(ownedCardIDs[i]) + ")";
		db.execute(insertSql);
	}
	return true;
}

vector<int> PlayerModel::loadUserTeam(int userID, const string& teamType) {
	auto& db = MySQLCli::getInstance();
	vector<int> teamIDs;

	string sql = "SELECT card_slot_index FROM user_teams WHERE user_id = " + to_string(userID) +
		" AND team_type = '" + teamType + "' ORDER BY id ASC";
	DBResult* res = db.query(sql);

	if (res) {
		char** row;
		while ((row = res->fetchRow()) != nullptr) {
			if (row[0] != nullptr) {
				teamIDs.push_back(stoi(row[0]));
			}
		}
		delete res;
	}
	return teamIDs;
}

bool PlayerModel::updateCurrentStage(int newStage, int userID) {
	auto& db = MySQLCli::getInstance();

	string sqlUpdate = "UPDATE user_stage_progress SET is_cleared = 1 WHERE user_id = "
		+ to_string(userID) + " AND stage_number = " + to_string(newStage - 1);
	db.execute(sqlUpdate);

	string sqlInsert = "INSERT IGNORE INTO user_stage_progress (user_id, stage_number, is_cleared) "
		"VALUES (" + to_string(userID) + ", " + to_string(newStage) + ", 0)";

	return db.execute(sqlInsert);
}