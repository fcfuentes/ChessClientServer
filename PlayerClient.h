#pragma once

#include <string>
using namespace std;

class PlayerClient
{
public:
    virtual ~PlayerClient() {}
    PlayerClient();
    PlayerClient(string nameValue);
    PlayerClient(string nameValue, string countryValue, int eloRvalue);
    PlayerClient(string nameValue, string countryValue);
    string getPlayerName() { return playerName; };
    string getCountry() { return country; };
    int getEloRating() { return eloRating; };
    int getID() { return playerID; };
    void setPlayerName(string nameValue);
    void setCountry(string countryValue);
    void setEloRating(int eloRvalue);
    std::string printData();
    virtual void Notify(const std::string& msg) = 0;

private:
    string playerName;
    string country;
    int eloRating;
    int playerID;
    static int ID;
};

