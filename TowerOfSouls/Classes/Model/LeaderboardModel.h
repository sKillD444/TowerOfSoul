#ifndef __LEADERBOAR_MODEL_H__
#define __LEADERBOAR_MODEL_H__

#include <iostream>
#include <string> 
#include <vector>
#include "../GameData.h"
using namespace std;

class LeaderboardModel {
public:
    static LeaderboardModel& getInstance() {
        static LeaderboardModel instance;
        return instance;
    }

    void loadLeaderboard();
    void updateLeaderboard(int userId, int score);
    void addLeaderboardEntry(int userId, int score);
    bool findPlayerLeaderboard(int userId);

    vector<LeaderboardData> getTop10Players();
    vector<LeaderboardData> getTop10PlayersBy(int criteria);
};

#endif