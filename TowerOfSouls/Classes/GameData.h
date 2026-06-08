#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <iostream>
#include <string>
using namespace std;

struct BattleCardData {
    int id;
    string card_id;
    string name;
    int level;
    int star;
    float hp;
    float atk;
    int cost;
    string role;
    float currentHp = 0;
};

struct PlayerData {
    int id;
    string username;
    int gold;
    int gems;
    int current_stage;
};

struct LeaderboardData {
    int rank;
    string username;
    int highest_floor;
};

#endif