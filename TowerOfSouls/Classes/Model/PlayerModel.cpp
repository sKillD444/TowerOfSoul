#include "PlayerModel.h"
#include "../mysql_api.h" 

PlayerData PlayerModel::loadUser(int userID) {
	auto& db = MySQLCli::getInstance();
	string sql = "Select id, username, gold, gems, current_stage from users where id="
		+ to_string(userID);
	DBResult* res = db.query(sql);
	PlayerData p;
	if (res) {
		char** row;
		while((row=res->fetchRow())!=nullptr){
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