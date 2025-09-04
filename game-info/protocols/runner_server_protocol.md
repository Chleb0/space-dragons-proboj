## CONFIG
The runner sends the server the initial configuration starting with a line with the word "CONFIG".

Next, the server recieves one line with all the names of the other players.

On the two following lines, the server recieves the amount of rounds to be played and the number of cards to be dealt.

This finishes config.

*Example communication:*
```
CONFIG
bobby og sans zhar chino
7 9 80
.
```
---

## Commands

During the rest of the game, the server communicates with the player using runner commands.

This proboj is using these commands to communicate:

### TO PLAYER

Command usage: 
```
TO PLAYER player comment
data
.
```

 - `player`: name of the recieving player
 - `comment` : message that will be noted in the player's log
 - `data` : message payload

Runner response:
```
STATUS
data
.
```

 - `STATUS` : either `OK`, `ERROR` or `DIED`
 - `data` : empty if `STATUS = OK`, otherwise this shows the error message to the player.

### READ PLAYER

Command usage:
```
READ PLAYER player
.
```

 - `player`: name of the player you wish to read from

Runner response:
```
STATUS
data
.
```

 - `STATUS` : either `OK`, `ERROR` or `DIED`
 - `data` : response from player if `STATUS = OK`, otherwise this shows the error message to the player.