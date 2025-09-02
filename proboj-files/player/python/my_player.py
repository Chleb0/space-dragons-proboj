from objects import *
from player import Player
import random
import sys

class MyPlayer(Player):
    enemyPlayers : list
    dragon_deck : list

    number_of_rounds : int
    number_of_cards : int
    environment : Environment

    hand : list

    def __init__(self, name):
        super().__init__(name)
        self.enemyPlayers = []
        self.dragon_deck = []
        self.hand = []
        self.environment = Environment()

    def pickCard(self, cards: list, cardID: str):
        # Look for the card with matching ID
        card = next((c for c in cards if c.ID == cardID), None)

        if card is None:
            if not cards:
                return ""
            # Pick random card if ID not found
            card = random.choice(cards)
            sys.stderr.write("You picked a card which was not in the card offer for this round. You took " + card.ID + " instead.\n")

        cardid = card.ID
        self.hand.append(card)
        cards.remove(card)
        return cardid

    def log(self, message):
        sys.stderr.write(message + "\n")

    # Override base Player::playCard
    def playCard(self, cardID: str):
        # Look for card in hand
        card = next((c for c in self.hand if c.ID == cardID), None)

        # If not found, pick random card
        if card is None:
            card = random.choice(self.hand)
            sys.stderr.write("You picked a card which was not in your hand. You played " + card.ID + " instead.\n")

        playedCard = card
        self.played.append(playedCard)
        self.hand.remove(card)

        self.card_points += playedCard.points
        self.crime_sum += playedCard.crime
        self.morale_sum += playedCard.morale
        self.research_sum += playedCard.research

        if playedCard.shield > 0:
            self.shield_active += playedCard.shield

        if playedCard.damage > 0:
            self.takeDamage(playedCard.damage)

        if playedCard.repair > 0:
            self.damage_taken -= playedCard.repair
            if self.damage_taken < 0:
                self.damage_taken = 0

        return playedCard.ID

    def DraftCard(self, cards: list):
        # Contestant's code
        return "cardID"

    def PlayCardOnTurn(self, dragon: Dragon, played_cards: list):
        # Contestant's code
        return "cardID"
