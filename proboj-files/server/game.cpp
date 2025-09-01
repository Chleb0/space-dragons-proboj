#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <random>
#include "card_gen.h"
#include "player.h"
#include "objects.h"
#include "communication.h"

using namespace std;

struct Game
{
    vector <Player> players;
    vector <Dragon> dragon_deck;
    vector <Card> card_deck;
    Environment env;

    CardGenerator card_gen;

    int number_of_players;
    int rounds;
    int cards;
    int total_cards;

    Game() {}

    /*
    Loads the game configuration sent by the Runner and generates additional config for the game.
    Passes the game configuration to the players.
    */
    void setup_game() {
        // Load players
        string line;
        getline(cin, line); // ignore first message
        getline(cin, line);

        vector<string> names;
        istringstream iss(line);
        string name;
        while (iss >> name) {
            names.push_back(name);
            players.emplace_back(name);
        }

        // Load preset game settings
        number_of_players = players.size();
        cin >> rounds;
        cin >> cards;
        cin >> total_cards;
        cin >> line; // clear the dot

        /*
        GENERATE GAME SETTINGS
        */

        // Determine player order
        shuffle(players.begin(), players.end(), gen);
        for (int i = 1; i < number_of_players; i++)
        {
            players[i-1].next_player = &players[i];
        }
        players[number_of_players-1].next_player = &players[0];

        // Generate dragon deck and card deck
        card_gen.setup(total_cards);
        card_deck = card_gen.generate_cards();
        dragon_deck = card_gen.generate_dragon_deck(rounds);

        // Randomise environment
        env = Environment();
        env.randomise_environment();

        /*
        SERVER TAKES CONTROL
        */

        // Send all players the initial configuration

        // Number of rounds and cards
        string config = "";
        config += (
            to_string(rounds) + "\n" + 
            to_string(cards) + "\n"
        );
        
        // Environment and dragons
        config += env.print();
        for (int i = 0; i < rounds; i++)
        {
            config += dragon_deck[i].ID + "\n";
        }

        // Send each player their config
        for (int i = 0; i < number_of_players; i++)
        {
            // Player configuration
            Player* current = &players[i];
            string player_config = "";
            player_config += (
                current->playerName + "\n" + 
                to_string(number_of_players) + "\n"
            );

            // Generate turn order from the player
            Player* iter = current->next_player;
            while (iter != current)
            {
                player_config += iter->playerName + "\n";
                iter = iter->next_player;
            }

            sendToPlayer(current, player_config);
            sendToPlayer(current, config);
        }
        
    }

    /*
    Carries out the draft part of the game.
    */
    void draft() {
        vector<vector<Card>> draft_decks(number_of_players);
        vector<Player*> turns;
        for (int i = 0; i < number_of_players; i++)
        {
            turns.push_back(&players[i]);
        }

        // Divide the cards into decks evenly
        shuffle(card_deck.begin(), card_deck.end(), gen);
        int card_index = 0;
        for (int i = 0; i < number_of_players; i++) {
            for (int j = 0; j < cards; j++) {
                draft_decks[i].push_back(card_deck[card_index++]);
            }
        }

        // Carry out the draft
        for (int j= 0; j < cards; j++)
        {
            for (int i = 0; i < number_of_players; i++)
            {
                draftFromDeck(turns[i], draft_decks[i]);
                turns[i] = turns[i]->next_player;
            }
        }

    }

    /*
    Carries out one round of player.
    Returns the pointer to the player who won the round.
    */
    Player* round(Player* first_player) {

        // Send the initial round configuration
        Dragon dragon = dragon_deck.back();
        Player* playing = first_player;
        sendAll(players, playing->playerName);
        sendAll(players, dragon.ID);

        Player* winning = first_player;
        vector<pair<Card, Player*>> played_cards = {};
        int highest = 0;

        // Each player plays a card
        for (int i = 0; i < number_of_players; i++)
        {
            Card played_card = playCardOnTurn(playing);
            played_cards.push_back({played_card, playing});
            sendAll(players, played_card.ID);

            if (played_card.value > highest) winning = playing; highest = played_card.value;
            playing = playing->next_player;
        }

        // Count the shots fired by players other than the winning one
        int shots = 0;
        for (int i = 0; i < number_of_players; i++) {
            if (played_cards[i].second != winning) shots += played_cards[i].first.attack;
        }
        
        winning->takeDamage(shots);
        winning->obtainDragon(dragon);
        return winning;
    }
    
    /*
    Carries out the game. 
    */
    void game() {
        Player* starting = &players[0];
        for (int i = 0; i < rounds; i++)
        {
            starting = round(starting);
        }
    }

    /*
    Assigns points based on the environment.
    */
    void assignEnvPoints() {
        vector <pair<int,Player*>> morale;
        vector <pair<int,Player*>> crime;
        vector <pair<int,Player*>> research;
        // Prepares arrays to be sorted.
        for (int i = 0; i < number_of_players; i++)
        {
            morale.push_back({players[i].morale_sum, &players[i]});
            crime.push_back({players[i].crime_sum, &players[i]});
            research.push_back({players[i].research_sum, &players[i]});
        }

        int reward, lowest, highest;
        bool other = false;

        if (env.crime_plus)
        {
            sort(crime.begin(),crime.end());
            reward = 10;
            lowest = crime[0].first;
            for (int i = 0; i < number_of_players; i++)
            {
                if (crime[i].first == lowest) crime[i].second->points += reward;
                else
                {
                    if (other) break;
                    else
                    {
                        other = true;
                        lowest = crime[i].first;
                        reward = 5;
                    }
                }
            }
        } else
        {
            sort(crime.begin(),crime.end(), greater<pair<int, Player*>>());
            reward = -10;
            highest = crime[0].first;
            for (int i = 0; i < number_of_players; i++)
            {
                if (crime[i].first == highest) crime[i].second->points += reward;
                else
                {
                    if (other) break;
                    else
                    {
                        other = true;
                        highest = crime[i].first;
                        reward = -5;
                    }
                }
            }
        }

        if (env.morale_plus)
        {
            sort(morale.begin(),morale.end(), greater<pair<int, Player*>>());
            reward = 10;
            highest = morale[0].first;
            for (int i = 0; i < number_of_players; i++)
            {
                if (morale[i].first == highest) morale[i].second->points += reward;
                else
                {
                    if (other) break;
                    else
                    {
                        other = true;
                        highest = morale[i].first;
                        reward = 5;
                    }
                }
            }
        } else
        {
            sort(morale.begin(),morale.end());
            reward = -10;
            lowest = morale[0].first;
            for (int i = 0; i < number_of_players; i++)
            {
                if (morale[i].first == lowest) morale[i].second->points += reward;
                else
                {
                    if (other) break;
                    else
                    {
                        other = true;
                        lowest = morale[i].first;
                        reward = -5;
                    }
                }
            }
        }

        sort(research.begin(),research.end());
        int bot_rew;
        if (env.research_plus)
        {
            bot_rew = 5; reward = 10;
        } else
        {
            bot_rew = -10; reward = -5;
        }
        lowest = research[0].first;
        highest = research.back().first;
        for (int i = 0; i < number_of_players; i++)
        {
            if (research[i].first == lowest) research[i].second->points+=bot_rew;
            else break;
        }
        for (int i = number_of_players-1; i >= 0; i-=1)
        {
            if (research[i].first == highest) research[i].second->points+=reward;
            else break;
        }
        
    } 

    /*
    Communicates final scores to the runner.
    */
    void returnScores() {
        cout << "SCORES" << endl;
        for (auto& player : players) {
            player.points += player.dragon_points + player.card_points;
            cout << player.playerName << " " << player.points << endl;
        }
        cout << "." << endl;
        cout << "END" << endl;
        cout << "." << endl;
    }
};

int main() {
    cerr << "sex je super" << endl;
    Game game = Game();
    game.setup_game();
    game.draft();
    game.game();
    game.assignEnvPoints();
    game.returnScores();
}