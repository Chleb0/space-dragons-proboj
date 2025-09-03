#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "objects.h"

using namespace std;

class Player
{
    public:

    string player_name;
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

    Player(const string& player_name)
        : player_name(player_name),

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
        auto card = find_if(cards.begin(), cards.end(), [&](const Card& c) { return c.ID == cardID; });

        // Invalid card ID was given, pick a random one
        if (card == cards.end()) {
            if (cards.empty()) return;
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dis(0, cards.size() - 1);
            card = cards.begin() + dis(gen);
        }

        cerr << "Player " << player_name << " picked card: " << card->ID << endl;
        hand.push_back(*card);
        cards.erase(card);
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
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<> dis(0, hand.size() - 1);
            card = hand.begin() + dis(gen);
        }

        // Play the card
        Card played_card = *card;
        played.push_back(played_card);
        hand.erase(card);

        // Update player attributes
        card_points += played_card.points;
        crime_sum += played_card.crime;
        morale_sum += played_card.morale;
        research_sum += played_card.research;

        // Apply shield
        if (played_card.shield > 0) {
            shield_active += played_card.shield;
        }

        // Apply damage
        if (played_card.damage > 0) {
            takeDamage(played_card.damage);
        }

        // Apply repair
        if (played_card.repair > 0) {
            damage_taken -= played_card.repair;
            if (damage_taken < 0) damage_taken = 0;
        }

        return played_card;
    }

    /*
    Applies bonuses from obtaining a dragon.
    */
    void obtainDragon(const Dragon& dragon) {
        dragon_points += dragon.points;
        crime_sum += dragon.crime;
        morale_sum += dragon.morale;
        research_sum += dragon.research;

        cerr << player_name << " won the dragon "+dragon.ID << endl;
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

