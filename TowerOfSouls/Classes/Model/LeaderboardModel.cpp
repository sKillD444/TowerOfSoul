#include "LeaderboardModel.h"
#include "../mysql_api.h" 

void LeaderboardModel::loadLeaderboard() {
}

void LeaderboardModel::updateLeaderboard(int userId, int score) {
    auto& db = MySQLCli::getInstance();
    string sql = "UPDATE endless_leaderboard SET highest_floor=" + to_string(score) +
        " WHERE user_id = " + to_string(userId) +
        " AND highest_floor < " + to_string(score);

    db.execute(sql);
}

void LeaderboardModel::addLeaderboardEntry(int userId, int score) {
    auto& db = MySQLCli::getInstance();
    string sql = "INSERT INTO endless_leaderboard (user_id, highest_floor, rank_position) "
        "VALUES (" + to_string(userId) + ", " + to_string(score) + ", 0)";
    db.execute(sql);
}

bool LeaderboardModel::findPlayerLeaderboard(int userId) {
    auto& db = MySQLCli::getInstance();
    string sql = "SELECT * FROM endless_leaderboard WHERE user_id = " + to_string(userId);

    DBResult* res = db.query(sql);

    if (res && res->numRows() > 0) {
        delete res;
        return true;
    }

    if (res) delete res;
    return false;
}