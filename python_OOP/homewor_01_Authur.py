class Hero:
    def __init__(self, name, hp):
        self.name = name
        self.__hp = hp

    def info(self):
        print(f"name: {self.name} hp: {self.__hp}\n")

    def take_damage(self, amount):
        self.__hp -= amount
        if self.__hp <= 0:
            self.__hp = 0
        print(f"Hero {self.name} take {amount} damage, hp={self.__hp}\n")


class Mage(Hero):
    def __init__(self, name, hp, mp):
        super().__init__(name, hp)
        self.mp = mp

    def fireball(self):
        self.mp -= 10
        print(f"{self.name} unleashed fireball! consumed 10 mp\n")

    def attack(self, target):
        print(f"{self.name} mage's normal attack at {target.name}\n")
        target.take_damage(5)


class Warrior(Hero):
    def __init__(self, name, hp):
        super().__init__(name, hp)

    def attack(self, target):
        print(f"{self.name} warrior's normal attack at {target.name}\n")
        target.take_damage(15)


def battle_round(attacker, defender):
    attacker.attack(defender)


authur = Warrior("Authur", 100)
authur.info()

bob = Warrior("Bob", 100)
bob.info()
bob.take_damage(50)
bob.take_damage(70)

gandalf = Mage("Grandalf", 100, 100)
gandalf.fireball()

battle_round(authur, gandalf)
battle_round(gandalf, authur)

authur.info()
bob.info()
gandalf.info()
