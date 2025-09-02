# SPACE DRAGONS PROBOJ

This repository contains the game Space Dragons as a bot coding contest.

Participants of the contest are tasked with building a bot that plays the game of Space Dragons for them.

These bots are then run via the runner.

This README contains:
- Participant's guide
- Server admin guide
- Programmer's guide

---

# Participant's Guide

You are tasked with building a bot that plays the game of Space Dragons. You can find the rules in `game_rules.md`.

Your task is to implement two functions inside the `MyPlayer` class:

---

### Draft Card

`**DraftCard(vector<Card> cards) -> string**`  
Selects a card during the drafting phase.

- **cards** : The list of cards you can choose from. The card you pick will be added to your hand. You may alter this array as you wish.  

**Output**: The function must return the **card ID** of the card you wish to select (type `string`).  
If the card ID is not in the pool, the server will choose a random card for you.

---

### Play Card on Turn

`**PlayCard(Dragon dragon, vector<pair<Card, Player*>> played_cards) -> string**`  
Selects a card to play each round.

- **dragon** : The `Dragon` object being hunted this round.  
- **played_cards** : A list of pairs. Each pair contains the card played and a pointer to the owner. Cards are sorted in play order. You may alter this array as you wish.  

**Output**: The function must return the **card ID** of the card you wish to play (type `string`).  
If the card ID is not in your hand, the server will choose a random card for you.

---

### Access to Information

From the `MyPlayer` class, you can access the following information about every player (including yourself):

- **playerName** : A string containing the player name.  
- **next_player** : A pointer to the next player in turn order.  
- **played** : A vector containing cards that this player has already played.  

- **Ship stats**  
  - **shield_active** : Integer, amount of active shield.  
  - **damage_taken** : Integer, total damage on the ship.  

- **Points**  
  - **card_points** : Integer, total points from played cards.  
  - **dragon_points** : Integer, total points from obtained dragons.  

- **Player attributes**  
  - **crime_sum** : Integer, sum of crime points.  
  - **morale_sum** : Integer, sum of morale points.  
  - **research_sum** : Integer, sum of research points.  

This information is stored in the `enemyPlayers` array of the `MyPlayer` class.  

You can also access information about the game itself:
- **dragon_deck** : List containing all dragons appearing in the game.  
- **number_of_rounds** : Total number of rounds in the game.  
- **number_of_cards** : Number of cards dealt to each player.  
- **environment** : The `Environment` object storing bonuses and modifiers.  
- **hand** : Vector containing the cards currently in your hand.  

---

### Additional Information

- You can use the `log` function to write messages to your player log (visible after the game).  
- Do not use the `pickCard` or `playCard` functions inside `MyPlayer`. Doing so will desynchronize your bot from the game state.  

---

# Server Admin Guide

This guide explains how to simulate games.

To run the game simulation you need 5 components:
- Server code (binary)  
- Player codes (binaries)  
- Config file  
- Games file  
- Runner  

---

### Server Code

Compile the code in `proboj-files/server/game.cpp` to a binary.  
Remember the path to this file.

---

### Player Code

Compile each player into a binary.  
Note the paths to these files.

---

### Config File

The runner requires a `config.json` file containing:

- **server** : Path to the compiled game server binary.  
- **players** : Defines all players.  
  - **command** : Path to the player’s executable.  
  - **language** : Language identifier used for timeouts (e.g. `cpp`, `python`, `java`).  

- **processes_per_player** : Integer, number of parallel processes per player (keep at 1).  
- **timeout** : Object defining max time (in seconds) for each move, depending on language.  
- **disable_logs** : Boolean, disables log file creation if `true`.  
- **disable_gzip** : Boolean, disables log compression if `true`.  
- **game_root** : Path to the folder where game logs will be saved.  

---

### Games File

The `games.json` file defines which games are played. Structure:

- **gamefolder** : Path to this game's folder inside `game_root`.  
- **players** : Array of player names (must match those in `config.json`).  
- **args** : Arguments passed to the server:  
  1. Number of rounds to play  
  2. Number of cards dealt to each player  
  3. Number of cards generated in play  

---

### Runner File

Ensure `runner.exe` exists in `proboj-files/runner/runner.exe`.  

To build it yourself, run:  
`go run ./runner/` inside the `runner-files` folder.  

---

### Running the Games

Run using:  

`path\to\runner.exe path\to\config.json path\to\games.json`

or the recommended version (where all 3 files are in the same folder):

`.\runner.exe .\config.json .\games.json`

Game scores are written to `game_root/score.json`.  

---

# Programmer's Guide

This guide is for those extending the code.

---

## Server Components

- **objects.h** : Declares game objects (`Card`, `Dragon`, `Environment`).  
- **player.h** : Declares the `Player` class.  
- **card_gen.h** : Generates the card and dragon decks.  
- **communication.h** : Handles runner-to-player communication.  
- **game.cpp** : Main game logic.  

---

### objects.h

This file declares three structures:

- **Card**  
  Encodes a crew member card with the following attributes:  
  - **value** : Base card value (from ID).  
  - **damage**, **attack**, **shield**, **repair** : Numeric stats applied when played.  
  - **points**, **crime**, **morale**, **research** : Attributes for scoring and bonuses.  
  - **ID** : Unique string identifying the card.  

  The constructor decodes stats from the ID using fixed ASCII offsets.  
  For all values except `value`, `@` represents 0 and the alphabet continues sequentially.

- **Dragon**  
  Encodes a dragon card with:  
  - **crime**, **morale**, **research** : Dragon attributes.  
  - **points** : Base points awarded when obtained.  
  - **ID** : Unique string identifying the dragon.  

  Attributes are decoded similarly to `Card` using ASCII offsets (`@` = 0).

- **Environment**  
  Defines global settings affecting scoring:  
  - **crime_plus**, **morale_plus**, **research_plus** : Booleans controlling whether each attribute is beneficial or harmful.  
  - **randomise_environment()** : Randomizes these three booleans.  
  - **print()** : Outputs the current environment settings as text.  

The constants `ACIIOFFSET` and `ACIIVALOFFSET` are used throughout the project to convert characters into values.

---

### player.h

This file declares the `Player` class, which tracks each game participant.

- **playerName** : The player’s name.  
- **next_player** : Pointer to the next player in turn order.  
- **running** : Boolean, used to check communication status. Stops interactions if `false`.  
- **hand**, **played** : Collections of `Card` objects representing available and used cards.  

- **Ship stats**  
  - **shield_active** : Amount of active shield.  
  - **damage_taken** : Amount of damage received.  

- **Point tracking**  
  - **card_points** : Points from cards played.  
  - **dragon_points** : Points from dragons won.  

- **Attribute sums**  
  - **crime_sum**, **morale_sum**, **research_sum** : Accumulated attributes from cards and dragons.  

- **points** : Total points for the final ranking.  

- **Constructor**  
  Initializes name, resets ship stats, attributes, and card collections.  

- **pickCard(vector<Card> &cards, const string& cardID)**  
  Used during drafting. Picks a card from a deck by ID.  
  If ID is invalid, selects a random card. Adds card to hand and removes it from the deck.  

- **playCard(const string& cardID)**  
  Plays a card from the hand.  
  If ID is invalid, selects randomly. Updates attributes, shields, damage, and repairs.  
  Moves the card to `played` and returns it.  

- **obtainDragon(const Dragon& dragon)**  
  Adds dragon points and attributes to the player when they win a round.  

- **takeDamage(int damage)**  
  Applies incoming damage to shields first; any excess is added to `damage_taken`.  
