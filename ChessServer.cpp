
//
//  ChessServer.cpp
//  Definitions the functions that clients can request to the server.
//
//  Created by Fabiola Castellanos Fuentes on 2021-03-15.
//
#include "PlayerClient.h"
#include "ChessServer.h"
#include <stdio.h>
#include <vector>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

//LogIn operation
void ChessServer::logIn(playerClientPtr player)
{
	playersList.push_back(player);
	for (auto msg : message)
		player->Notify(msg);
}

//Logout operation
void ChessServer::logOut(playerClientPtr player)
{
	if (getIndex(player) >= 0) {
		playersList.erase(playersList.begin() + getIndex(player));
	}

}
//Send a notification to players
void ChessServer::Notify(const std::string& msg)
{
	message.push_back(msg);
	while (message.size() > max_recent_msgs)
		message.pop_front();

	for (auto player : playersList)
		player->Notify(msg);
}

//Sort current players decrecing order by Elo rank
void ChessServer::sortPlayers() {
	std::sort(playersList.begin(), playersList.end(), compareTwoPlayers);
}

//List current players on the server operation
std::string ChessServer::displayPlayers() {
	sortPlayers();
	std::string listP = "";
	for (int i = 0; i < playersList.size(); i++) {
		listP = listP + playersList[i]->printData();
	}
	listP = listP + "\n";

	return listP;
}

//Request to pair operation. Returns the opponent player if pair exists or null if it doesnt.
playerClientPtr ChessServer::pairChessPlayer(playerClientPtr player) {
	sortPlayers();
	bool isFound = false;
	int index = getIndex(player);
	playerClientPtr pl1;

	if (index == playersList.size() - 1) {
		pl1 = playersList[index - 1];
	}
	else {
		pl1 = playersList[index + 1];
	}

	if (isAMatch(player, pl1))
		isFound = true;
	else if (index > 0) {
		pl1 = playersList[index - 1];
		if (isAMatch(player, pl1))
			isFound = true;
	}

	if (isFound) {
		return pl1;
	}
	else {
		cout << "No player found.\n";
		return NULL;
	}
}

//Returns true if two players have a similar Elo Rank within 100, false otherwise.
bool ChessServer::isAMatch(playerClientPtr player1, playerClientPtr player2) {
	bool match = false;
	if (abs(player1->getEloRating() - player2->getEloRating()) <= 100)
		match = true;
	return match;
}

// Compare two players by Elo Rank
bool ChessServer::compareTwoPlayers(playerClientPtr player1, playerClientPtr player2) {
	bool isGreater = false;
	if (player1->getEloRating() > player2->getEloRating())
		isGreater = true;
	else if (player1->getEloRating() == player2->getEloRating()) {
		if (player1->getID() < player2->getID())
			isGreater = true;
	}
	return isGreater;
}


// Returns the position in the vector of a given client. Returns -1 if the client is not
// part of the list
int ChessServer::getIndex(playerClientPtr pl) {
	for (int i = 0; i < playersList.size(); i++) {
		if (playersList[i]->getID() == pl->getID()) {
			return i;
		}
	}
	return -1;
}

//Notify two players with the profile of the opponent if there is a match, returns No match found otherwise.
void ChessServer::NotifyPlayers(playerClientPtr player1, playerClientPtr player2) {

	if (player2 != NULL) {
		std::string stg1 = player2->printData();
		std::string stg2 = player1->printData();
		player1->Notify(stg1);
		player2->Notify(stg2);
	}
	else {
		player1->Notify(std::string("No match found\n"));
	}
}

