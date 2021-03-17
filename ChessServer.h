#pragma once
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include "PlayerClient.h"

typedef std::shared_ptr<PlayerClient> playerClientPtr;

class ChessServer
{
public:
	void logIn(playerClientPtr player);
	void logOut(playerClientPtr player);
	std::string displayPlayers();
	playerClientPtr pairChessPlayer(playerClientPtr player);
	void NotifyPlayers(playerClientPtr player1, playerClientPtr player2);
	void Notify(const std::string& msg);
private:
	std::vector<playerClientPtr> playersList;
	enum { max_recent_msgs = 100 };
	std::deque<std::string> message;

	bool static compareTwoPlayers(playerClientPtr player1, playerClientPtr player2);

	bool isAMatch(playerClientPtr player1, playerClientPtr player2);

	void sortPlayers();

	int getIndex(playerClientPtr pl);
};

