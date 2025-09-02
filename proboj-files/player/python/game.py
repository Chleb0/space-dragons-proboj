import sys
from objects import *
from player import Player
from my_player import MyPlayer
import copy

# Runner works by sending a dot after each message to the player. This clears it.
def clearcomm():
    dot = sys.stdin.readline().strip()

# This program synchronizes the game between the server and the player.
def main():
    # Player recieves their name
    my_name = sys.stdin.readline().strip()
    player = MyPlayer(my_name)

    # Player recieves the number of rounds
    number_of_players = int(sys.stdin.readline().strip())

    # Load the enemies and make a turn cycle.
    player.enemyPlayers = []
    for i in range(1, number_of_players):
        enem_name = sys.stdin.readline().strip()
        enm = Player(enem_name)
        player.enemyPlayers.append(enm)
        if i != 0:
            player.enemyPlayers[i-1].next_player = player.enemyPlayers[i]

    player.enemyPlayers[number_of_players-2].next_player = player
    player.next_player = player.enemyPlayers[0]
    clearcomm()

    # Pass the game settings to the participant's code
    parts = sys.stdin.readline().strip().split()
    number_of_rounds = int(parts[0])
    number_of_cards = int(parts[1])
    player.number_of_rounds = number_of_rounds
    player.number_of_cards = number_of_cards

    # Pass the environment settings to the participant
    parts = sys.stdin.readline().strip().split()
    crime = parts[0] == "1" or parts[0].lower() == "true"
    morale = parts[1] == "1" or parts[1].lower() == "true"
    research = parts[2] == "1" or parts[2].lower() == "true"
    player.environment = Environment(crime, morale, research)

    # Pass the dragon deck to the participant
    player.dragon_deck = []
    for i in range(number_of_rounds):
        dragonID = sys.stdin.readline().strip()
        dragon = Dragon(dragonID)
        player.dragon_deck.append(dragon)
    clearcomm()

    # START OF DRAFT
    for i in range(number_of_cards):
        draft_deck = []
        for g in range(number_of_cards - i):
            cardid = sys.stdin.readline().strip()
            draft_deck.append(Card(cardid))
        choice = player.DraftCard(draft_deck.copy())
        choice = player.pickCard(draft_deck, choice)

        sys.stderr.write("You picked card: " + choice + "\n")
        sys.stdout.write(choice + "\n")
        sys.stdout.write(".\n")
        sys.stdout.flush()
        clearcomm()
    # END OF DRAFT

    # START OF GAME
    for round in range(number_of_rounds):
        starting_player = sys.stdin.readline().strip()
        clearcomm()
        dragonID = sys.stdin.readline().strip()
        clearcomm()
        dragon = Dragon(dragonID)

        # Find the player starting the round
        turn = player
        for i in range(number_of_players - 1):
            if starting_player == player.enemyPlayers[i].playerName:
                turn = player.enemyPlayers[i]

        played_cards = []
        highest_card = 0
        winner = turn

        for i in range(number_of_players):
            card = Card("nothing")
            if turn == player:
                choice = player.PlayCardOnTurn(copy.deepcopy(dragon), copy.deepcopy(played_cards))
                choice = player.playCard(choice)
                card = Card(choice)
                sys.stdout.write(choice + "\n")
                sys.stdout.write(".\n")
                sys.stdout.flush()
                choice = sys.stdin.readline().strip()
                clearcomm()
            else:
                played = sys.stdin.readline().strip()
                clearcomm()
                card = Card(played)
                turn.playCard(played)
            played_cards.append((card, turn))

            if card.value > highest_card:
                winner = turn
                highest_card = card.value

            turn = turn.next_player

        shots_fired = 0
        for entry in played_cards:
            card = entry[0]
            card_owner = entry[1]
            if card_owner != winner:
                shots_fired += card.attack

        winner.obtainDragon(dragon)
        winner.takeDamage(shots_fired)
    # END OF GAME

if __name__ == "__main__":
    main()
