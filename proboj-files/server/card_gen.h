#ifndef CARD_GEN
#define CARD_GEN

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "objects.h"

using namespace std;

static random_device rd; 
static mt19937 gen(rd()); 

struct CardGenerator
{
    // Number of cards to generate
    int cards;

    // Max default score for a card without any abilities
    int max_score;

    char zero;

    // Maximum and minimum final scores of the cards
    int max_fin_score;
    int min_fin_score = 0;

    // Maximum and minimul final scores of the cards after the abilities have been handed out.
    int ab_max_score;
    int ab_min_score;
    
    // Constants
    const int MAX_ABILITIES = 5;
    const vector<double> ability_probability = {19, 27, 15, 7, 1, 0.1};
    const int MAX_ATTRIBUTES = 4;
    const vector<double> attribute_probability = {23, 27, 24, 9, 3};
    const vector<double> pts_probality = {1, 20, 50, 20, 1};
    const vector<double> dragon_dist = {5, 9, 6};
    vector<vector<string>> dragon_attributes = {{"@@@"},{"A@@","@A@","@@A"}, {"B@@","@B@","@@B","AA@","A@A","@AA"}};

    // Evaluation of the abilities
    int shield_eval(int shield) {return shield*(max_score/2.32) + shield*shield*(max_score/8.53);}
    int attack_eval(int attack) {return attack*(max_score/3.2);}
    int damage_eval(int damage) {return -damage*(max_score/2.28);}
    int repair_eval(int repair) {return repair*(max_score/2.78) + repair*repair * (max_score/12.8);}
    int crime_eval(int crime) {return -crime*(max_score/4.92) + (crime-1)*(crime-1) * (max_score/38);}
    int morale_eval(int morale) {return morale*(max_score/6.4) + morale*morale * (max_score/32);}
    int research_eval(int research) {return research*(max_score/6.4);}

    vector<vector<pair<int,string>>> ability_combinations;
    vector<int> ab_min;
    vector<int> ab_max;
    vector<vector<pair<int,string>>> attribute_combinations;
    vector<int> at_min;
    vector<int> at_max;

    /*
    Generates all possible combinations of attributes within the allowed range (of number of attributes).
    Puts the generated combinations in the attribute combinations vector.
    */
    void generate_attribute_combination(int altering, string combination, int depth) {
        if (depth == MAX_ATTRIBUTES+1 ) return;
        if (altering == 3) return;
        
        // Either skip this attribute and add 0 of it.
        generate_attribute_combination(altering+1, combination, depth);

        for (int i = 1; i + depth < MAX_ATTRIBUTES+1; i++)
        {
            // Or add every possible amout of this attribute.
            combination[altering] = combination[altering] +1;
            int score = evaluate_attribute(combination);
            attribute_combinations[depth+i].push_back({score, combination});
            generate_attribute_combination(altering+1, combination, depth+i);
        }
    }

    /*
    Generates all possible combinations of abilities within the allowed range (of number of abilities).
    Puts the generated combinations in the abilities combinations vector.
    */
    void generate_ability_combination(int altering, string combination, bool has_damage, int depth) {
        if (depth == MAX_ABILITIES+1) return;
        if (altering == 4) return;
        // Cards can't have both damage and shield/repair.
        if (has_damage && altering > 1) return;
        
        // Skip ability.
        generate_ability_combination(altering+1, combination, has_damage, depth);
        
        if (!altering) has_damage = true;
        
        for (int i = 1; i + depth < MAX_ABILITIES+1; i++)
        {
            // Add every amount of this ability.
            combination[altering] = combination[altering] +1;
            int score = evaluate_ability(combination);

            ability_combinations[depth+i].push_back({score, combination});
            generate_ability_combination(altering+1, combination, has_damage, depth+i);
        }
    }

    /*
    Evaluates the card's abilities with a score.
    */
    int evaluate_ability(string combination) {
        int damage = combination[0] -ACIIOFFSET;
        int attack = combination[1] -ACIIOFFSET;
        int shield = combination[2] -ACIIOFFSET;
        int repair = combination[3] -ACIIOFFSET;
        int score = 0;
        score += ( shield_eval(shield)
            + attack_eval(attack) +
            damage_eval(damage) +
            repair_eval(repair));
        return score;
    }

    /*
    Evaluates the card's attributes with a score.
    */
    int evaluate_attribute(string combination) {
        int crime = combination[0] -ACIIOFFSET;
        int morale = combination[1] -ACIIOFFSET;
        int research = combination[2] -ACIIOFFSET;
        int score = 0;
        score += crime_eval(crime)
                +morale_eval(morale)
                +research_eval(research);
        return score;
    }

    /*
    Decided abilities for the card.
    The input is the lower and upper bound for how strong/weak the abilities are able to be.
    */
    string decide_abilities(int lc, int hc) {
        vector <double> weights = ability_probability;
        for (int i = 0; i < MAX_ABILITIES+1; i++)
        {
            if ((ab_min[i] > hc || lc > ab_max[i]))
            {
                // If there is no possibility of this number of abilities, remove it.
                weights[i] = 0;
            }
        }
        discrete_distribution<> dist(weights.begin(), weights.end());
        int n_of_abs = dist(gen);

        // Count the valid ability combinations and choose one.
        int valid = 0;
        for (auto &x: ability_combinations[n_of_abs]) {
            if (lc <= x.first && x.first <= hc) valid++;
        }
        uniform_int_distribution<> rand_idx(0, valid - 1);
        int pull = rand_idx(gen);

        for (auto &x: ability_combinations[n_of_abs]) {
            if (lc <= x.first && x.first <= hc) {
                if (pull == 0)
                {
                    return x.second;
                } else
                {
                    pull-=1;
                }
            }
        }

        return ability_combinations[n_of_abs][pull].second;
    }

    /*
    Decided attributes for the card.
    The input is the lower and upper bound for how strong/weak the attributes are able to be.
    */
    string decide_attributes(int lc, int hc) {
        vector <double> weights = attribute_probability;
        discrete_distribution<> dist(weights.begin(), weights.end());
        int n_of_atts = dist(gen);

        int valid = 0;
        for (auto &x: attribute_combinations[n_of_atts]) {
            if (lc <= x.first && x.first <= hc) valid++;
        }

        // If we choose a number of attributes that is not able to fit in the lower and upper bound, we choose a random combination.
        if (valid == 0)
        {
            uniform_int_distribution<> rand_idx(0, attribute_combinations[n_of_atts].size()-1);
            int pull = rand_idx(gen);
            return attribute_combinations[n_of_atts][pull].second;
        }
        
        uniform_int_distribution<> rand_idx(0, valid - 1);
        int pull = rand_idx(gen);

        for (auto &x: attribute_combinations[n_of_atts]) {
            if (lc <= x.first && x.first <= hc) {
                if (pull == 0)
                {
                    return x.second;
                } else
                {
                    pull-=1;
                }
            }
        }

        return attribute_combinations[n_of_atts][pull].second;
    }

    /*
    This function decided points for the card.
    */
    string decide_points(int score) {
        int chunk = max_fin_score / pts_probality.size();
        discrete_distribution<> dist(pts_probality.begin(), pts_probality.end());
        int pull = dist(gen);
        int base = score/chunk;
        int pts = pull - base +1;
        string out = "!";
        out[0] = pts+ACIIOFFSET;
        return out;
    }

    /*
    Generates and returns a deck of cards.
    */
    vector<Card> generate_cards() {
        vector<Card> out;
        out.reserve(cards);

        for (int i = 1; i <= cards; i++) {
            out.push_back(gen_card(i));
        }

        return out;
    }

    /*
    Generates and returns a card with a certain value.
    */
    Card gen_card(int value) {
        int score = (value-1)*(value-1) - (cards-value)*(cards-value);
        
        int lc = ab_min_score - score;
        int hc = ab_max_score - score;

        string abilities = decide_abilities(lc, hc);
        score += evaluate_ability(abilities);

        lc = min_fin_score - score;
        hc = max_fin_score - score;

        string attributes = decide_attributes(lc, hc);
        score += evaluate_attribute(attributes);

        string points = decide_points(score);

        string cardid = "!" + abilities + attributes + points;
        cardid[0] = ACIIVALOFFSET + value;
        Card card(cardid);
        return card;
    }

    /*
    This function prepares everything in order for the cards to be generated,
    such as generating attribute and ability combinations.
    */
    void setup(int n_cards) {
        // Initialize constants
        cards = n_cards; 
        zero = ACIIOFFSET;
        max_score = cards*cards;
        max_fin_score = max_score - max_score/6;
        ab_max_score = max_score;
        ab_min_score = max_fin_score - ab_max_score;

        // Initialize vectors
        ability_combinations.resize(MAX_ABILITIES+1);
        ab_max.resize(MAX_ABILITIES+1, 0);
        ab_min.resize(MAX_ABILITIES+1, 0);
        attribute_combinations.resize(MAX_ATTRIBUTES+1);
        at_max.resize(MAX_ATTRIBUTES+1, 0);
        at_min.resize(MAX_ATTRIBUTES+1, 0);

        // Add blank combinations
        string blank(4, zero);
        ability_combinations[0].push_back({0,blank});
        blank = string(3, zero);
        attribute_combinations[0].push_back({evaluate_attribute(blank),blank});

        // Generate combinations
        generate_ability_combination(0,blank,false,0);
        generate_attribute_combination(0,blank,0);

        for (auto &deck: attribute_combinations) {
            sort(deck.begin(), deck.end());
        }
        for (auto &deck: ability_combinations) {
            sort(deck.begin(), deck.end());
        }
        for (int i = 0; i < MAX_ATTRIBUTES+1; i++)
        {
            at_max[i] = attribute_combinations[i].back().first;
            at_min[i] = attribute_combinations[i][0].first;
        }
        for (int i = 0; i < MAX_ABILITIES+1; i++)
        {
            ab_max[i] = ability_combinations[i].back().first;
            ab_min[i] = ability_combinations[i][0].first;
        }

    }

    CardGenerator() {}

    /*
    Generates and returns a dragon deck of a given size.
    */
    vector <Dragon> generate_dragon_deck(int dragons) {
        vector <Dragon> out;

        for (int i = 0; i < dragons; i++)
        {
            string dragid = "@";

            // Decide the number of attributes the dragon has
            discrete_distribution<> attribute_dist(dragon_dist.begin(), dragon_dist.end());
            int dragon_atts = attribute_dist(gen);

            // Decide the combination of attributes
            uniform_int_distribution<> attr_comb_dist(0, dragon_attributes[dragon_atts].size() - 1);
            int attr_idx = attr_comb_dist(gen);

            // Decide points for the dragon
            uniform_int_distribution<> pts_roll(8, 12);
            int points = pts_roll(gen);
            dragid[0] = points + ACIIOFFSET;

            dragid = dragon_attributes[dragon_atts][attr_idx] + dragid;
            Dragon new_dragon(dragid);
            out.push_back(new_dragon);
        }
        return out;
    }
};

/*
int main() {
    CardGenerator cg = CardGenerator();
    cg.setup(80);
    vector<Card> lmao = cg.generate_cards();
    vector <Dragon> drak = cg.generate_dragon_deck(8);
    cout << "done";
}
*/

#endif