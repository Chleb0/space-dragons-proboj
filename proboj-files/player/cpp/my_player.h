#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include <iostream>
#include <vector>
#include "objects.h"
#include "player.h"

using namespace std;



class MyPlayer : public Player
{
    public:

    vector<Player> enemy_players;
    vector<Dragon> dragon_deck = {};

    int number_of_rounds;
    int number_of_cards;
    Environment environment;

    vector<Card> hand; 


    MyPlayer(const string& name)
        : Player(name)
    {
        hand.clear();
    }

    string pickCard(vector<Card>& cards, const string& cardID) {
        auto card = find_if(cards.begin(), cards.end(), [&](const Card& c) { return c.ID == cardID; });

        if (card == cards.end()) {
            if (cards.empty()) "";
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dis(0, static_cast<int>(cards.size()) - 1);
            card = cards.begin() + dis(gen);
            cerr << "You picked a card which was not in the card offer for this round. You took " << card->ID << " instead." << endl;
        }

        string cardid = card->ID;
        hand.push_back(*card);
        cards.erase(card);
        return cardid;
    }

    void log(string message) {
        cerr << message << endl;
    }

    // Override base Player::playCard
    string playCard(const string& cardID) override {
        auto card = find_if(hand.begin(), hand.end(), [&](const Card& c) { return c.ID == cardID; });

        // If not found, pick random card
        if (card == hand.end()) {
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dis(0, static_cast<int>(hand.size()) - 1);
            card = hand.begin() + dis(gen);
            cerr << "You picked a card which was not in your hand. You played " << card->ID << " instead." << endl;
        }

        Card played_card = *card;
        played.push_back(played_card);
        hand.erase(card);

        card_points += played_card.points;
        crime_sum += played_card.crime;
        morale_sum += played_card.morale;
        research_sum += played_card.research;

        if (played_card.shield > 0) shield_active += played_card.shield;

        if (played_card.damage > 0) takeDamage(played_card.damage);

        if (played_card.repair > 0) {
            damage_taken -= played_card.repair;
            if (damage_taken < 0) damage_taken = 0;
        }

        return played_card.ID;
    }

    string DraftCard(vector<Card> cards) {
        // Contestant's code
        return "cardID";
    }

    string PlayCardOnTurn(Dragon dragon, vector<pair<Card, Player*>> played_cards) {
        // Contestant's code
        return "cardID";
    }
};

#endif