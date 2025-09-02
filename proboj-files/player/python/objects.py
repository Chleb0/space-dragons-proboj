import random

# Constants matching the original C++ offsets
ACIIOFFSET = 64
ACIIVALOFFSET = 32

class Card:

    # Value of the card
    value: int

    # Abilities triggered when played
    damage: int
    attack: int
    shield: int
    repair: int

    # Attributes of the card
    points: int
    crime: int
    morale: int
    research: int

    ID: str

    def __init__(self, id: str):
        self.ID = id
        self.value = ord(id[0]) - ACIIVALOFFSET

        self.damage = ord(id[1]) - ACIIOFFSET
        self.attack = ord(id[2]) - ACIIOFFSET
        self.shield = ord(id[3]) - ACIIOFFSET
        self.repair = ord(id[4]) - ACIIOFFSET

        self.crime = ord(id[5]) - ACIIOFFSET
        self.morale = ord(id[6]) - ACIIOFFSET
        self.research = ord(id[7]) - ACIIOFFSET

        self.points = ord(id[8]) - ACIIOFFSET


class Dragon:

    ID: str

    # Dragon's attributes
    crime: int
    morale: int
    research: int

    # Dragon points
    points: int

    def __init__(self, id: str):
        self.ID = id
        self.crime = ord(id[0]) - ACIIOFFSET
        self.morale = ord(id[1]) - ACIIOFFSET
        self.research = ord(id[2]) - ACIIOFFSET
        self.points = ord(id[3]) - ACIIOFFSET


class Environment:
    # Game settings
    crime_plus: bool
    morale_plus: bool
    research_plus: bool

    def __init__(self, crime: bool = False, morale: bool = False, research: bool = False):
        self.crime_plus = crime
        self.morale_plus = morale
        self.research_plus = research

    def randomise_environment(self):
        self.crime_plus = bool(random.randint(0, 1))
        self.morale_plus = bool(random.randint(0, 1))
        self.research_plus = bool(random.randint(0, 1))

    def print(self) -> str:
        return f"{int(self.crime_plus)}\n{int(self.morale_plus)}\n{int(self.research_plus)}\n"