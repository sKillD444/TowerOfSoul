#ifndef __LEADERBOARD_CONTROLLER_H__
#define __LEADERBOARD_CONTROLLER_H__

#include <iostream>
using namespace std;
#include "../GameData.h"
#include "../Model/LeaderboardModel.h"


class LeaderboardController {
public:
	void updateLeaderboard(int userId, int score) {
		bool flag = LeaderboardModel::getInstance().findPlayerLeaderboard(userId);
		if (flag) {
			LeaderboardModel::getInstance().updateLeaderboard(userId, score);
		}
		else {
			LeaderboardModel::getInstance().addLeaderboardEntry(userId, score);
		}
	}

	vector<LeaderboardData> getTop10Players() {
		return LeaderboardModel::getInstance().getTop10Players();
	}

	vector<LeaderboardData> getTop10PlayersBy(int criteria) {
		return LeaderboardModel::getInstance().getTop10PlayersBy(criteria);
	}
};

#endif