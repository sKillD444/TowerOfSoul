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
    bool registerUser(const string& username, const string& password);
    int loginUser(const string& username, const string& password);
    int getOwnedCardsCount(int userID);
    int getHighestEndlessFloor(int userID);
    bool saveUserTeam(int userID, const string& teamType, const vector<int>& ownedCardIDs);
    vector<int> loadUserTeam(int userID, const string& teamType);
    bool updateCurrentStage(int newStage, int userID);
};

#endif