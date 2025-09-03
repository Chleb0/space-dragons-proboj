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

    // Player's cards
    vector<Card> played;
    Player* next_player;

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

    Player(const string& player_name)
        : player_name(player_name),

          shield_active(0),
          damage_taken(0),
          card_points(0),
          dragon_points(0),
          crime_sum(0),
          morale_sum(0),
          research_sum(0)
    {
    played = {};
    }

    virtual ~Player() = default;



    virtual string playCard(const string& cardID) {

        // Play the card
        Card playedCard = Card(cardID);
        played.push_back(playedCard);

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
        
        return playedCard.ID;
    }

    void obtainDragon(const Dragon& dragon) {
        dragon_points += dragon.points;
        crime_sum += dragon.crime;
        morale_sum += dragon.morale;
        research_sum += dragon.research;
    }

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