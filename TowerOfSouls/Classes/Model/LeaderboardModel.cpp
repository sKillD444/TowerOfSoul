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

std::vector<LeaderboardData> LeaderboardModel::getTop10Players() {
    std::vector<LeaderboardData> top10;
    auto& db = MySQLCli::getInstance();

    string sql = "SELECT u.username, l.highest_floor "
        "FROM endless_leaderboard l "
        "JOIN users u ON l.user_id = u.id "
        "ORDER BY l.highest_floor DESC LIMIT 10";

    DBResult* res = db.query(sql);

    if (res) {
        int currentRank = 1;
        char** row;
        while ((row = res->fetchRow()) != nullptr) {
            LeaderboardData data;
            data.rank = currentRank++;
            data.username = row[0];
            data.highest_floor = stoi(row[1]);
            top10.push_back(data);
        }
        delete res;
    }
    return top10;
}