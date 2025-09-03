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

- **`cards`** : The list of cards you can choose from. The card you pick will be added to your hand. You may alter this array as you wish.  

**Output**: The function must return the **card ID** of the card you wish to select (type `string`).  
If the card ID is not in the pool, the server will choose a random card for you.

---

### Play Card on Turn

`**PlayCard(Dragon dragon, vector<pair<Card, Player*>> played_cards) -> string**`  
Selects a card to play each round.

- **`dragon`** : The `Dragon` object being hunted this round.  
- **`played_cards`** : A list of pairs. Each pair contains the card played and a pointer to the owner. Cards are sorted in play order. You may alter this array as you wish.  

**Output**: The function must return the **card ID** of the card you wish to play (type `string`).  
If the card ID is not in your hand, the server will choose a random card for you.

---

### Access to Information

From the `MyPlayer` class, you can access the following information about every player (including yourself):

- **`playerName`** : A string containing the player name.  
- **`next_player`** : A pointer to the next player in turn order.  
- **`played`** : A vector containing cards that this player has already played.  

- **Ship stats**  
  - **`shield_active`** : Integer, amount of active shield.  
  - **`damage_taken`** : Integer, total damage on the ship.  

- **Points**  
  - **`card_points`** : Integer, total points from played cards.  
  - **`dragon_points`** : Integer, total points from obtained dragons.  

- **Player attributes**  
  - **`crime_sum`** : Integer, sum of crime points.  
  - **`morale_sum`** : Integer, sum of morale points.  
  - **`research_sum`** : Integer, sum of research points.  

This information is stored in the `enemyPlayers` array of the `MyPlayer` class.  

You can also access information about the game itself:
- **`dragon_deck`** : List containing all dragons appearing in the game.  
- **`number_of_rounds`** : Total number of rounds in the game.  
- **`number_of_cards`** : Number of cards dealt to each player.  
- **`environment`** : The `Environment` object storing bonuses and modifiers.  
- **`hand`** : Vector containing the cards currently in your hand.  

---

### Additional Information

- You can use the `log` function to write messages to your player log, which you can check out after the game.  
- Do not use the `pickCard` or `playCard` functions inside `MyPlayer`. Doing so will desynchronize your bot from the real game state.  

---

# Server Admin Guide

This guide explains how to simulate games.

To run the game simulation you need 5 components:
- Server code (executable)  
- Player codes (executable)  
- Config file  
- Games file  
- Runner  

---

### Server Code

Compile the code in `proboj-files/server/game.cpp` to an executable.  
Remember the path to this file.

---

### Player Code

Compile each player into an executable.  
Note the paths to these files.

---

### Config File

The runner requires a `config.json` file containing:

- **`server`** : Path to the compiled game server executable.  
- **`players`** : Defines all players.  
  - **`command`** : Path to the player’s executable.  
  - **`language`** : Language identifier used for timeouts (`cpp`, `python`, `java` for example).  

- **`processes_per_player`** : Integer, number of parallel processes per player (keep at 1).  
- **`timeout`** : Object defining max time (in seconds) for each move, depending on language.  
- **`disable_logs`** : Boolean, disables log file creation if `true`.  
- **`disable_gzip`** : Boolean, disables log compression if `true`.  
- **`game_root`** : Path to the folder where game logs will be saved.  

---

### Games File

The `games.json` file defines the games to be played. Structure:

- **`gamefolder`** : Path to this game's folder inside `game_root`.  
- **`players`** : Array of player names (must be a subset of defined players in `config.json`).  
- **`args`** : Arguments passed to the server. The server expects these three **space-separated** values:  
  1. Number of rounds to play  
  2. Number of cards dealt to each player  
  3. Number of cards to be generated 

---

### Runner File

A pre-built `runner.exe` should already exist in `proboj-files/runner/runner.exe`.  

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

This guide is for those wishing to extend the code.

---

## Server Components

- **objects.h** : Declares game objects (`Card`, `Dragon`, `Environment`).  
- **player.h** : Declares the `Player` class.  
- **card_gen.h** : Generates the card and dragon decks.  
- **communication.h** : Handles server-to-player communication using the runner.  
- **game.cpp** : Main game logic.  

---

### objects.h

This file declares three structures:

- **`Card`**  
  Encodes a crew member card with the following attributes:  
  - **`value`** : Base card value (from ID).  
  - **`damage`**, **`attack`**, **`shield`**, **`repair`** : Numeric stats applied when played.  
  - **`points`**, **`crime`**, **`morale`**, **`research`** : Attributes for scoring and bonuses.  
  - **`ID`** : Unique string identifying the card.  

  The constructor decodes stats from the ID using fixed ASCII offsets.  
  For all values except `value`, `@` represents 0 and the alphabet continues sequentially.

- **`Dragon`**  
  Encodes a dragon card with:  
  - **`crime`**, **`morale`**, **`research`** : Dragon attributes.  
  - **`points`** : Base points awarded when obtained.  
  - **`ID`** : Unique string identifying the dragon.  

  Attributes are decoded similarly to `Card` using ASCII offsets (`@` = 0).

- **Environment**  
  Defines global settings affecting scoring:  
  - **`crime_plus`**, **`morale_plus`**, **`research_plus`** : Booleans controlling whether each attribute is beneficial or harmful.  
  - **`randomise_environment`** : Randomizes these three booleans.  
  - **`print`** : Outputs the current environment settings as text.  

The constants `ACIIOFFSET` and `ACIIVALOFFSET` defined in this file are used in the project to convert characters into values.

---

### player.h

This file declares the `Player` class, which tracks each game participant.

- **`playerName`** : The player’s name.  
- **`next_player`** : Pointer to the next player in turn order.  
- **`running`** : Boolean, used to check communication status. Stops interactions if `false`.  
- **`hand`**, **`played`** : Collections of `Card` objects representing available and used cards.  

- **Ship stats**  
  - **`shield_active`** : Amount of active shield.  
  - **`damage_taken`** : Amount of damage received.  

- **Point tracking**  
  - **`card_points`** : Points from cards played.  
  - **`dragon_points`** : Points from dragons won.  

- **Attribute sums**  
  - **`crime_sum`**, **`morale_sum`**, **`research_sum`** : Accumulated attributes from cards and dragons.  

- **`points`** : Total points for the final ranking.  

- **Constructor**  
  Initializes name, resets ship stats, attributes, and card collections.  

- **`pickCard(vector<Card> &cards, const string& cardID)`**  
  Used during drafting. Picks a card from a deck by ID.  
  If ID is invalid, selects a random card. Adds card to hand and removes it from the deck.  

- **`playCard(const string& cardID)`**  
  Plays a card from the hand.  
  If ID is invalid, selects randomly. Updates attributes, shields, damage, and repairs.  
  Moves the card to `played` and returns it.  

- **`obtainDragon(const Dragon& dragon)`**  
  Adds dragon points and attributes to the player when they win a round.  

- **`takeDamage(int damage)`**  
  Applies incoming damage to shields first; any excess is added to `damage_taken`.  

---

### communication.h

This file handles communication between the server and the players using the runner.

- **`handleStatus(Player\* player)`**  
  Reads status from player.  
  If not `"OK"`, sets `player->running = false` and logs the error.  

- **`handleAnswer(Player\* player)`** 
  Reads player's response.  
  Returns `"random"` on error and sets `player->running = false`.   

- **`sendToPlayer(Player\* player, const string message, const string comment)`**  
  Sends a message to one player. Comment is sent to the players log.

- **`sendAll(vector<Player>& players, const string message, const string comment)`**  
  Sends the same message to every player in passed list using `sendToPlayer()`.  

- **`readCard(Player\* player)`**  
  Reads card ID from a player.  Returns `"random"` if there’s an error (`player->running = false`).  

- **`draftFromDeck(Player\* player, vector<Card>& deck)`**  
  Sends full deck to player, expects a pick.  
  - Calls `player->pickCard(deck, choice)`.  
  - Picks random card on invalid input.  

- **`playCardOnTurn(Player\* player)`**  
  Asks player to play a card.  
  - Calls `player->playCard(choice)`.  
  - Plays random card on error. 

--- 
### card_gen.h

Defines the class CardGenerator. This can be used to generate cards for games. I will not go into more detail in here, for more information about the process, check `card_generation.md`.

- **`generate_attribute_combination`**  
  Build all attribute combinations with scores.  

- **`generate_ability_combination`**  
  Build all valid ability combinations with scores.  

- **`evaluate_ability / evaluate_attribute`**  
  Set constants for the impact of abilities and attributes.  

- **`decide_abilities(lc, hc)`**  
  Randomly pick abilities within score bounds.  

- **`decide_attributes(lc, hc)`**  
  Randomly pick attributes within score bounds.  

- **`decide_points`**  
  Assign card point value.  

- **`generate_cards / gen_card(value`**  
  Create full card deck or single card ID with balanced stats.  

- **`setup`**  
  Generate combinations, bounds, and set scoring limits.  

- **`generate_dragon_deck`**  
  Generate a deck of dragons for the game.  

---

### game.cpp (Game loop)

This file simulates the game. The functions in this file reflect the stages of the game. The order of communication and it's for can be found in `protocols/runner_server_protocol.md` and `protocols/server_player_protocol.md`

- **`setup_game`**  
    This function handles the setup of the game.
  - Generates card/dragon decks with `CardGenerator`.  
  - Randomizes environment.  
  - Sends config to all players.  

- **`draft`**  
     Handles the draft phase of the game in it's entirety.

- **`round`**  
   Handles one round of play and returns a pointer to the player who won the round.

- **`game`**  
   Runs all rounds, winner of each round starts the next.  

- **`assignEnvPoints`**  
   Adjusts `points` based on environment.  

- **`returnScores`**  
   Calculates and returns and saves the final scores.   

- **`main`**  
  - Creates game, runs full sequence:  
    `setup_game → draft → game → assignEnvPoints → returnScores`.

---

## Player Components

- **objects** : Declares game objects (`Card`, `Dragon`, `Environment`).  
- **player** : Declares the `Player` class.  
- **my_player** :  Declares the `MyPlayer` class, derived from the `Player` class. The contestant's code is here. 
- **game** : Main player-server communication logic.  

--- 

### objects

Same as in the server files.

---

### player

Same as the `Player` class in the server files, with the following exceptions:

  - `hand` : This list is not available, as the player's code is unable to see other player's hands.
  - `running`:  Only used by the server.
  - `pick_card`: This function only intercats with the player's hand, so it is absent.

---

### my_player

Same as the `Player` class in the server files, with the exception of all the information about the game stored in it. Check `Participant's guide/Access to information`.

---

### game

This class solves the main logic behind the game and the sequencing.

It executes the three following stages:
  - **Game setup** : it reads the game configuration of the game, sets up the `Player` and `MyPlayer` classes.
  - **Draft phase** : executes the draft part of the game, calling the `PickCard` function of `MyPlayer`.
  - **Hunt phase** : executes the hunt phase of the game, calling the `PlayCardOnTurn` function of `MyPlayer`.

The player then shuts down. More information about the player's stdin can be found in `server_player_protocol.md`.
