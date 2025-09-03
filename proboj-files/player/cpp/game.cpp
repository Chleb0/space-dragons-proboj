#include <iostream>
#include <string>
#include "objects.h"
#include <vector>
#include "player.h"
#include "my_player.h"

using namespace std;

/*
Runner works by sending a dot after each message to the player. This clears it.
*/
void clearcomm() {
    string dot;
    cin >> dot;
    //cerr << "Cleared comms with: " << dot << endl;
}

/*
This program synchronizes the game between the server and the player.
*/

int main() {

    // Player recieves their name
    string my_name; cin >> my_name;
    MyPlayer player = MyPlayer(my_name);

    // Player recieves the number of rounds
    int number_of_players;
    cin >> number_of_players;

    // Load the enemies and make a turn cycle.
    player.enemy_players.reserve(number_of_players - 1);
    for (int i = 1; i < number_of_players; i++)
    {
        string enem_name; cin >> enem_name;
        Player enm = Player(enem_name);
        player.enemy_players.push_back(enm);
        if (i != 0) player.enemy_players[i-1].next_player = &player.enemy_players[i];
    }

    player.enemy_players[number_of_players-2].next_player = &player;
    player.next_player = &player.enemy_players[0];
    clearcomm();

    // Pass the game settings to the participant's code
    int number_of_rounds, number_of_cards;
    cin >> number_of_rounds >> number_of_cards;
    player.number_of_rounds = number_of_rounds;
    player.number_of_cards = number_of_cards;

    // Pass the environment settings to the participant
    bool crime, morale, research;
    cin >> crime >> morale >> research;
    player.environment = Environment(crime, morale, research);

    // Pass the dragon deck to the participant
    for (int i = 0; i < number_of_rounds; i++)
    {
        string dragonID; cin >> dragonID;
        Dragon dragon = Dragon(dragonID);
        player.dragon_deck.push_back(dragon);
    }
    clearcomm();


    /*
    START OF DRAFT
    */

    for (int i = 0; i < number_of_cards; i++)
    {
        vector<Card> draft_deck = {};
        for (int g = 0; g < number_of_cards-i; g++)
        {
            string cardid; cin >> cardid;
            draft_deck.emplace_back(cardid);
        }
        string choice = player.DraftCard(draft_deck);
        choice = player.pickCard(draft_deck, choice);

        cerr << "You picked card: " << choice << endl;
        cout << choice << endl;
        cout << "." << endl;
        clearcomm();
    }

    /*
    END OF DRAFT
    */

    /*
    START OF GAME
    */

    for (int round = 0; round < number_of_rounds; round++)
    {
        string starting_player; cin >> starting_player; clearcomm();
        string dragonID; cin >> dragonID; clearcomm();
        Dragon dragon = Dragon(dragonID);

        // Find the player starting the round
        Player* turn = &player;
        for (int i = 0; i < number_of_players-1; i++)
        {
            if (starting_player == player.enemy_players[i].player_name)
            {
                turn = &player.enemy_players[i];
            }
        }

        vector <pair<Card, Player* >> played_cards = {};
        int highest_card = 0;
        Player* winner = turn;
        
        for (int i = 0; i < number_of_players; i++)
        {
            Card card = Card("nothing");
            if (turn == &player)
            {
                string choice = player.PlayCardOnTurn(dragon, played_cards);
                choice = player.playCard(choice);
                card = Card(choice);
                cout << choice << endl;
                cout << "." << endl;
                cin >> choice; clearcomm(); // Clear the card just played

            } else
            {
                string played; cin >> played; clearcomm();
                card = Card(played);
                turn->playCard(played);
            }
            played_cards.push_back({card, turn});
            
            if (card.value > highest_card)
            {
                winner = turn;
                highest_card = card.value;
            }
            
            turn = turn->next_player;
        }

        int shots_fired = 0;
        for (const auto& entry : played_cards) {
            const Card& card = entry.first;
            Player* card_owner = entry.second;
            if (card_owner != winner)
            {
                shots_fired += card.attack;
            }
        }
        
        winner->obtainDragon(dragon);
        winner->takeDamage(shots_fired);
    }

    /*
    END OF GAME
    */
    
}
    

