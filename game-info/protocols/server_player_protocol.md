# Protocol between server and player

This file explain the messages between the player and the server.

Every message sent to the player ends with a `.`. Here's the explanation of each message in their phase of the game.

---

## Game setup

Messages that help the player side setup the game properly on their end.

### Message 1: Player configuration

The player recieves their name on the first line. The server will adress them by their name for the entirety of the match.

Then, the player recieves the number of players in the game on the second line, and a name for each of the opposing players.

Note that the player recieves the opponent names in the order in which the opponents take their turns after the player.

*Example communication:*
```
bobby
5
og
sans
zhar
chino
.
```

### Message 2: Game configuration

In the next phase, the player recieves the game settings.

First, the player recieves the number of rounds to be played and the number of cards to be dealt to each player.

Next, the player recieves three bits of information, each indicating (in this order) whether *crime, morale, research* provide or retract points. 0 indicates negative bonus, 1 indicates positive bonus.

Lastly, the dragon deck is shown to the player. The server sends a dragon card ID for each round to be played.

---

## Draft phase

In the draft phase, the player will recieve a deck of cards in each round. In each round, the player is expected to reply with a card from the offer.

In each round, the number of cards in the deck will decrease by 1.

---

## GAME

The beginning player will be chosen at random.

In consecutive rounds, the server will message everyone the name of the player playing the first card in the round and the dragon at stake in this round.

Then, the server will send every player each card being played. Each player is expected to message the server the card they wish to play on their turn.

If the player messages a card they do not have or an invalid card, they will play a random card from their hand.