# ChessClientServer
Client-Server assignment 

To start the server, run server.cpp

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
