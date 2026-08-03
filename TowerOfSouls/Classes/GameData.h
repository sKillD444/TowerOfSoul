#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <iostream>
#include <string>
using namespace std;

struct BattleCardData {
    int id;
    string card_id;
    string name;
    int level = 1;
    int star = 1;
    float hp;
    float currentHp;
    float atk;
    int cost;
    string role;
    string type;
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