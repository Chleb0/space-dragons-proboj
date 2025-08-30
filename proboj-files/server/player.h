#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "objects.h"

using namespace std;

class Player
{
    public:

    string playerName;
    Player* next_player;

    // For communication checks
    bool running = true;

    // Player's cards
    vector<Card> hand;
    vector<Card> played;

    // Ship stats
    int shield_active = 0;
    int damage_taken = 0;

    // Points on cards in front of the player
    int card_points = 0;
    int dragon_points = 0;

    // Player's attributes
    int crime_sum = 0;
    int morale_sum = 0;
    int research_sum = 0;

    // Points for final ranking
    int points = 0;

    Player(const string& playerName)
        : playerName(playerName),

          shield_active(0),
          damage_taken(0),
          card_points(0),
          dragon_points(0),
          crime_sum(0),
          morale_sum(0),
          research_sum(0)
    {hand = {};
    played = {};
    }

    /*
    Picks a card from the deck provided and adds it to the player's hand. 
    If there is no such card, picks a random card.
    */
    void pickCard(vector<Card> &cards, const string& cardID) {
        auto card = std::find_if(cards.begin(), cards.end(), [&](const Card& c) { return c.ID == cardID; });

        // Invalid card ID was given, pick a random one
        if (card == cards.end()) {
            if (cards.empty()) return;
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, cards.size() - 1);
            card = cards.begin() + dis(gen);
        }

        hand.push_back(*card);
        cards.erase(card);
        cerr << card->ID << endl;
    }

    /*
    Plays the given card from the players hand. 
    If there is no such card in the players hand, plays a random card.
    Returns the card played.
    */
    Card playCard(const string& cardID) {
        // Find card in hand
        auto card = find_if(hand.begin(), hand.end(), [&](const Card& c) { return c.ID == cardID; });

        // If not found, pick a random card
        if (card == hand.end()) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, hand.size() - 1);
            card = hand.begin() + dis(gen);
        }

        // Play the card
        Card playedCard = *card;
        played.push_back(playedCard);
        hand.erase(card);

        // Update player attributes
        card_points += playedCard.points;
        crime_sum += playedCard.crime;
        morale_sum += playedCard.morale;
        research_sum += playedCard.research;

        // Apply shield
        if (playedCard.shield > 0) {
            shield_active += playedCard.shield;
        }

        // Apply damage
        if (playedCard.damage > 0) {
            takeDamage(playedCard.damage);
        }

        // Apply repair
        if (playedCard.repair > 0) {
            damage_taken -= playedCard.repair;
            if (damage_taken < 0) damage_taken = 0;
        }

        return playedCard;
    }

    /*
    Applies bonuses from obtaining a dragon.
    */
    void obtainDragon(const Dragon& dragon) {
        dragon_points += dragon.points;
        crime_sum += dragon.crime;
        morale_sum += dragon.morale;
        research_sum += dragon.research;

        cerr << playerName << " obtained a dragon" << endl;
    }

    /*
    Correctly inflicts damage onto the player.
    */
    void takeDamage(int damage) {
        if (shield_active >= damage)
        {
            shield_active -= damage;
        } else
        {   
            damage -= shield_active;
            shield_active = 0;
            damage_taken += damage;
        }
    }
};

#endif

