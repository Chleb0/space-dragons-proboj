# SPACE DRAGONS PROBOJ

This repository contains the game space dragons as a bot coding contest.

Participants of the contest are tasked to build a bot which plays the game of Space Dragons for them.

This bots are then run via runner.

This readme contains:
- Participant's guide
- Server admin guide
- Programmers guide

# Participant guide

You are tasked to build a bot that plays the game of space dragons. You can find the rules in ```game_rules.md```.

This means coding two functions. You will recieve a class ```MyPlayer```, that has the following functions:

### Draft Card

```DraftCard( vector<Card> cards ) -> string```

The draft card function has the following parameter:

- ```cards``` : the list of cards you can choose from. The card you pick will be added to your hand. **You may alter this array as you wish.**

**Output**: this function expects the output to be the **card ID** of the card you wish to select. This is should be of the **string** type.
If you select a card which is not in the pool, the server will choose a random card for you. 

### Play Card On Turn

```DraftCard( Dragon dragon, vector<pair<Card, Player*>> played_cards ) -> string```

The play card on turn function has the following parameters:

- ```dragon```: a ```Dragon``` that is being hunted this round.
- `played_cards`: a list of pairs (tuples in Python). The first of the pair is the card played, the second of the pair is a pointer to the owner. The cards are sorted in the order which they were played in. **You may alter this array as you wish.**

**Output**: this function expects the output to be the **card ID** of the card you wish to play. This is should be of the **string** type.
If you select a card which is not in your hand, the server will choose a random card for you. 

### Acces to information

From the `MyPlayer` class, you can access the following information about every player (including you):
- `playerName`: a string indicating the name of the player.
- `next_player`: a pointer to the next player in turn order.
- `played`: a vector of cards this player has played.

- Ship stats:
    - `shield_active`: integer indicating how many shield this player has active.
    - `damage_taken`: integer representing the total damage on the player's ship.

- Points:
    - `card_points`: integer, total points from cards that the player has played.
    - `dragon_points`: integer, total points from dragons obtained by the player.

- Player attributes:
    - `crime_sum`: integer, sum of crime points.
    - `morale_sum`: integer, sum of morale points.
    - `research_sum`: integer, sum of research points.

This information is stored in the array of `enemyPlayers`, which is a component of the `MyPlayer` class. Moreover, the player has access to the following information about the game:

- `dragon_deck`: a list containing the deck of Dragon objects that will appear in the game.
- `number_of_rounds`: the total number of rounds to be played in the game.
- `number_of_cards`: the number of cards dealt to each player.
- `environment`: the Environment object storing the data about the environement and the bonuses applied.
- `hand`: a vector containing the Card objects currently held by the you.

### Additional information

You can use the `log` function to send messages to your player log, which you will be able to read after the game.

For correct use, do not use the `pickCard` and `playCard` functions in the class. Using them will result in the loss of synchronization with the real state of the game.

# Server admin guide

This is a guide for someone looking to simulate the games.

Running the game simulation is quite simple. You need 5 components:
- Server code (binary)
- Player codes (binary)
- Config file
- Games file
- Runner

### Server code

Compile the code in `proboj-files/server/game.cpp` to a binary file. Remember the path to this file.

### Player code

Compile the player codes into binary files. Note the paths to these files.

### Config file

In order to run properly, the runner need a `config.json` configuration file with the following information:

- `server`: Path to the compiled game server binary.
- `players`: Defines all players participating in the game.
    - `command`: Path to the player's executable.
    - `language`: Language identifier used for timeout settings (e.g., "cpp", "python", "java").

- `processes_per_player`: Integer, number of parallel processes to run for each player. **Keep at 1.**
- `timeout`: Integer, specifies the maximum time allowed (in seconds) for each player's move, depending on the language.
- `disable_logs`: Boolean, if set to `true`, the runner will not write any log output to files.
- `disable_gzip`: Boolean, if set to `true`, generated log files will not be compressed.
- `game_root`: Path to the folder where all of the game logs will be saved.

### Games file

This `games.json` file will define the games to be played. Here is the structure:

- `gamefolder`: Path to the this games folder inside the `game_root` folder.
- `players`: An array of player names participating in this game. This has to be subset of the players defined in the `config.json` file.
- `args`: Arguments (game settings) passed to the server. The server expects these three space-separated values:
    - Number of rounds to be played
    - Number of cards to be dealt to each player
    - Number of cards in play (that are generated)

### Runner file

The `runner.exe` file should be ready and working in `proboj-files/runner/runner.exe`.

If you want to compile it yourself, you can do so by running: `go run ./runner/`.

## Running the games

If you are all set, you can run the games with the following command:

`path\to\runner.exe path\to\config.json path\to\games.json`, or the recommended version (where all 3 files are in the same folder):

`.\runner.exe .\config.json .\games.json`

You can then file the scores for the games in `game_root/score.json`. 
Enjoy the madness!

# Programmer's guide