//
//  PlayerClient.cpp
//  Declarations of constructors, mutators and printData function.
//
//  Created by Fabiola Castellanos Fuentes on 2021-03-15.
//

#include "PlayerClient.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int PlayerClient::ID = 0;
typedef std::shared_ptr<PlayerClient> player_client_ptr;

// Empty construtor
PlayerClient::PlayerClient() {
	playerName = "NoName";
	playerID = ID++;
	eloRating = 100;
	country = "None";
}

PlayerClient::PlayerClient(string nameValue, string countryValue, int eloRvalue) {
	playerName = nameValue;
	country = countryValue;
	setEloRating(eloRvalue);
	playerID = ID++;
}

PlayerClient::PlayerClient(string nameValue, string countryValue) {
	playerName = nameValue;
	country = countryValue;
	eloRating = 100;
	playerID = ID++;
}

PlayerClient::PlayerClient(string nameValue) {
	playerName = nameValue;
	country = "NONE";
	eloRating = 100;
	playerID = ID++;
}

void PlayerClient::setCountry(string countryValue)
{
	country = countryValue;
}

void PlayerClient::setPlayerName(string nameValue)
{
	playerName = nameValue;
}

//Set the Elo Raiting to some value between 100 and 3000. 
void PlayerClient::setEloRating(int eloRvalue) {
	if (eloRvalue < 100) {
		eloRating = 100;
	}
	else if (eloRvalue > 3000) {
		eloRating = 3000;
	}
	else {
		eloRating = eloRvalue;
	}
}

//Returns a string with the current info of the client.
std::string PlayerClient::printData() {
	std::string stg = playerName + std::string("\t") + country + std::string("\t") + std::to_string(eloRating) + std::string("\n");
	return stg;
}
