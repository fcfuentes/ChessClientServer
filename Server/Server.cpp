//
// Server.cpp
// ~~~~~~~~~~~~~~~
// Modified variables names and added some specific code for the chess server site to a 
// chat_sever example from the boost library. 
//
// Server can handle the following commands from the client:
// 
//  When a client start a session, the client should enter,
//
//  name PlayersName
//  country countryOrigen
//  elo eloRank
//  list players 
//  pair me
//  logout me 
//
//  A player can request a list of the current players and it will received a list of the current player in descending order by elo rank.
//  It also can logout and it will be removed from the list of participants. Player can request to be paired and if there is another player
//  within 100 points in Elo Rank. Both player will receive the profile of the opponent.
//  60 s waiting for a match if a match is not found with the current players was not implemented.
//
////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com).   //
//                                                                                //
// Distributed under the Boost Software License, Version 1.0. (See accompanying.  //
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).         //
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>
#include <boost/algorithm/string.hpp>

#include "PlayerClient.h"
#include "ChessServer.h"

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::redirect_error;
using boost::asio::use_awaitable;


class Session
	: public PlayerClient,
	public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, ChessServer& room)
		: socketS(std::move(socket)),
		timerS(socketS.get_executor()),
		serverRoom(room)
	{
		timerS.expires_at(std::chrono::steady_clock::time_point::max());
	}

	void start()
	{
		serverRoom.logIn(shared_from_this());

		co_spawn(socketS.get_executor(),
			[self = shared_from_this()]{ return self->reader(); },
			detached);

		co_spawn(socketS.get_executor(),
			[self = shared_from_this()]{ return self->writer(); },
			detached);
	}

	void Notify(const std::string& msg)
	{
		write_msgs_.push_back(msg);
		timerS.cancel_one();
	}

private:
	awaitable<void> reader()
	{
		try
		{
			for (std::string read_msg;;)
			{
				std::size_t n = co_await boost::asio::async_read_until(socketS,
					boost::asio::dynamic_buffer(read_msg, 1024), "\n", use_awaitable);

				string input = read_msg.substr(0, n);

				vector<string> SplitVec;
				boost::split(SplitVec, input, boost::is_any_of(" "), boost::token_compress_on);

				string command = SplitVec.at(0);

				string parameter = "";
				if (SplitVec.size() > 1) {
					parameter = SplitVec.at(1);
					parameter = parameter.substr(0, parameter.length() - 2);
				}
                                //Added functionality to received a request from the client and send information requested to the 
				//player. 60 s waiting for a match if a match is not found with the current players was not implemented.
				
				cout << "Command received is " << command << ", with parameter: " << parameter << "\n";

				if (command == "name") {
					setPlayerName(parameter);
				}
				else if (command == "country") {
					setCountry(parameter);
				}
				else if (command == "elo") {
					setEloRating(atoi(parameter.c_str()));
				}
				else if (command == "print") {
					string clientData = serverRoom.displayPlayers();
					Notify(clientData);
				}
				else if (command == "pair") {
					auto target = serverRoom.pairChessPlayer(shared_from_this());
					serverRoom.NotifyPlayers(shared_from_this(), target);
				}
				else if (command == "logout") {
					stop();
				}

				//serverRoom.Notify(read_msg.substr(0, n));
				read_msg.erase(0, n);
			}
		}
		catch (std::exception&)
		{
			stop();
		}
	}

	awaitable<void> writer()
	{
		try
		{
			while (socketS.is_open())
			{
				if (write_msgs_.empty())
				{
					boost::system::error_code ec;
					co_await timerS.async_wait(redirect_error(use_awaitable, ec));
				}
				else
				{
					co_await boost::asio::async_write(socketS,
						boost::asio::buffer(write_msgs_.front()), use_awaitable);
					write_msgs_.pop_front();
				}
			}
		}
		catch (std::exception&)
		{
			stop();
		}
	}

	void stop()
	{
		serverRoom.logOut(shared_from_this());
		socketS.close();
		timerS.cancel();
	}

	tcp::socket socketS;
	boost::asio::steady_timer timerS;
	ChessServer& serverRoom;
	std::deque<std::string> write_msgs_;
};

awaitable<void> listener(tcp::acceptor acceptor)
{
	ChessServer room;

	for (;;)
	{
		std::make_shared<Session>(
			co_await acceptor.async_accept(use_awaitable),
			room
			)->start();
	}
}

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_context io_context(1);

		unsigned short port = 1234;
		co_spawn(io_context,
			listener(tcp::acceptor(io_context, { tcp::v4(), port })),
			detached);

		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&](auto, auto) { io_context.stop(); });

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
