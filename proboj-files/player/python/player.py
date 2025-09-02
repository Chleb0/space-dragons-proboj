from objects import *

class Player:
    playerName = ""
    
    # Player's cards
    played = []
    next_player = None

    # Ship stats
    shield_active = 0
    damage_taken = 0

    # Points on cards in front of the player
    card_points = 0
    dragon_points = 0

    # Player's attributes
    crime_sum = 0
    morale_sum = 0
    research_sum = 0

    def __init__(self, playerName):
        self.playerName = playerName
        self.played = []
        self.next_player = None

    def playCard(self, cardID):
        # Play the card
        playedCard = Card(cardID)
        self.played.append(playedCard)

        # Update player attributes
        self.card_points += playedCard.points
        self.crime_sum += playedCard.crime
        self.morale_sum += playedCard.morale
        self.research_sum += playedCard.research

        # Apply shield
        if playedCard.shield > 0:
            self.shield_active += playedCard.shield

        # Apply damage
        if playedCard.damage > 0:
            self.takeDamage(playedCard.damage)

        # Apply repair
        if playedCard.repair > 0:
            self.damage_taken -= playedCard.repair
            if self.damage_taken < 0:
                self.damage_taken = 0

        return playedCard.ID

    def obtainDragon(self, dragon):
        self.dragon_points += dragon.points
        self.crime_sum += dragon.crime
        self.morale_sum += dragon.morale
        self.research_sum += dragon.research

    def takeDamage(self, damage):
        if self.shield_active >= damage:
            self.shield_active -= damage
        else:
            damage -= self.shield_active
            self.shield_active = 0
            self.damage_taken += damage
