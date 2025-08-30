#ifndef COMMUNICATION
#define COMMUNICATION


#include <iostream>
#include <vector>
#include <string>
#include "player.h"

using namespace std;

/*
Handles the status returned by the runner when logging to a player.
*/
void handleLogStatus(Player* player) {
    string status; cin >> status;

    string errmsg = "";
    string line;
    do {
        getline(cin, line);
        errmsg += line + "\n";
    } while (line != ".");

    if (status != "OK")
    {
        player->running = false;
    }
}

/*
Logs the message into the player's log.
*/
void playerLog(Player* player, const string message) {
    cout << "TO PLAYER " << player->playerName << " comment" << endl;
    cout << message;
    cout << "." << endl;
    handleLogStatus(player);
}

/*
Handles the status returned by the runner when communicating with player.
*/
void handleStatus(Player* player) {
    string status; cin >> status;

    string errmsg = "";
    string line;
    do {
        getline(cin, line);
        errmsg += line + "\n";
    } while (line != ".");

    if (status != "OK")
    {
        player->running = false;
        playerLog(player, errmsg);
    }
}

/*
Handles and returns the players answer to a query.
*/
string handleAnswer(Player* player) {
    string status; cin >> status;

    if (status != "OK")
    {
        player->running = false;
        
        string errmsg = "";
        string line;
        do {
            getline(cin, line);
            errmsg += line + "\n";
        } while (line != ".");

        playerLog(player, errmsg);
        return "random";

    } else
    {
        string answer; cin >> answer;
        string line;
        do {
            getline(cin, line);
        } while (line != ".");
        return answer;
    }
}


/*
Sends the message to the player's stdin.
*/
void sendToPlayer(Player* player, const string message) {
    if (!player->running)
    {
        return;
    }
    
    cout << "TO PLAYER " << player->playerName << endl;
    cout << message << endl;
    cout << "." << endl;
    handleStatus(player);
}


/*
Sends the message to stdin to all players.
*/
void sendAll(vector<Player> &players, const string message) {
    for (auto& player : players) {
        sendToPlayer(&player, message);
    }
}

/*
Reads player's stdout expecting a card ID and returns it.
Returns "random" if there is an error.
*/
string readCard(Player* player) {
    if (!player->running)
    {
        return "random";
    }
    
    cout << "READ PLAYER " << player->playerName << endl;
    cout << "." << endl;
    return handleAnswer(player);
}


/*
Sends a deck of cards to the player, then expects the player to pick one of the cards in the deck.
If the player picks an invalid card or there is an error, picks a random card from the deck.
Modifies the deck directly and the card is added to the players hand.
*/
void draftFromDeck(Player* player, vector<Card> &deck) {
    string cards = "";
    for (int i = 0; i < deck.size(); i++)
    {
        cards += deck[i].ID + "\n";
    }
    sendToPlayer(player, cards);
    string choice; choice = readCard(player);

    player->pickCard(deck, choice);
}

/*
Expects the player to pick a card they wish to play on their turn.
If they do not pick a card or there is an error, the player plays a random card.
The card's changes are applies to the player.
Returns the card played.
*/
Card playCardOnTurn(Player* player) {
    string choice = readCard(player);
    return player->playCard(choice);
}

#endif
