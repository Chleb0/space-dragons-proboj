#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

int ACIIOFFSET=64;
int ACIIVALOFFSET=32;

struct Card
{
    public:
    
    // Value of the card
    int value;

    // Abilities triggered when played
    int damage;
    int attack;
    int shield;
    int repair;

    // Attributes of the card
    int points;
    int crime;
    int morale;
    int research;

    string ID;

    Card(const string& id) : ID(id) {
        value = id[0] -ACIIVALOFFSET;

        damage = id[1] -ACIIOFFSET;
        attack = id[2] -ACIIOFFSET;
        shield = id[3] -ACIIOFFSET;
        repair = id[4] -ACIIOFFSET;

        crime = id[5] -ACIIOFFSET;
        morale = id[6] -ACIIOFFSET;
        research = id[7] -ACIIOFFSET;

        points = id[8] -ACIIOFFSET;
    }
};

struct Dragon
{
    string ID;

    // Dragon's attributes
    int crime;
    int morale;
    int research;

    // Dragon points
    int points;

    Dragon(const string& id) : ID(id) {
        crime = id[0] -ACIIOFFSET;
        morale = id[1] -ACIIOFFSET;
        research = id[2] -ACIIOFFSET;
        points = id[3] -ACIIOFFSET;
    }
};

struct Environment
{
    // Game settings
    bool crime_plus;
    bool morale_plus;
    bool research_plus;
    Environment(bool crime = false, bool morale = false, bool research = false)
        : crime_plus(crime), morale_plus(morale), research_plus(research) {}


    /*
    Picks a random boolean for each attribute in play (crime, morale, research).
    This determines whether this attribute adds or subtracts points.
    */
    void randomiseEnvironment() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 1);

        crime_plus = dist(gen);
        morale_plus = dist(gen);
        research_plus = dist(gen);
    }

    string print() const {
        return (to_string(crime_plus) + "\n" + to_string(morale_plus) + "\n" + to_string(research_plus) + "\n");
    }
};

#endif