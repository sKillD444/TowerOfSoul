#ifndef __PLAYER_MODEL_H__
#define __PLAYER_MODEL_H__

#include <iostream>
#include <string> 
#include <vector>
#include "../GameData.h"
using namespace std;

class PlayerModel {
public:
    static PlayerModel& getInstance() {
        static PlayerModel instance;
        return instance;
    }

    PlayerData loadUser(int userID);
    bool updateGold(int newGold, int userID);
    bool updateGem(int newGem, int userID);
};

#endif