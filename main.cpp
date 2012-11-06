#include <iostream>
#include "game.h"
#include <ctime>
#include <cstdlib>

using namespace std;

void init(void)
{
	srand(time(0));
}

int main(int argc, char **argv)
{
	init();
	//TODO: load this from a file later
	//int num_races = 3;
	Race races[]={	Race("Human", 8, 4, 8, 4),
			Race("Dwarf", 10, 6, 4, 2),
			Race("Goblin", 4, 4, 10, 4),
			};
	//player, TODO: ask them for name, class and race
	Character pc = Character("Dan", &races[0], MALE);

	//a piece of armour
	Armour *breastPlate = new Armour("Lazarus Suit", 4, 3, Resistance(5, 0, 0, 0), ARMOUR_SPECIAL_REGEN, list<int>(3, SLOT_TORSO));
	pc.equip(breastPlate, SLOT_TORSO);

	//a weapon
	Weapon *sledge = new Weapon("Massive sledgehammer", 8, 6, 2, 4, 0, false);
	pc.equip(sledge, SLOT_HAND_LEFT);

	//and another
	Weapon *dagger = new Weapon("Assassin's blade", 3, 2, 2, 2, WEAPON_SPECIAL_POISON, true);
	pc.equip(dagger, SLOT_HAND_RIGHT);
	pc.dumpStats(0);
	pc.unequip(sledge);
	pc.equip(dagger, SLOT_HAND_RIGHT);
	pc.dumpStats(0);

	//add a shield
	Shield *aspis = new Shield("Bronze Aspis", 3, 2, NULL, 0, 5);
	pc.equip(aspis, SLOT_HAND_LEFT);

	pc.dumpStats(0);

	//spawn a goblin
	Character *goblin = new Character("Grizott", &races[2], FEMALE);
	goblin->dumpStats(0);
	pc.attackBasic(goblin);
	goblin->dumpStats(0);

	return 0;
}
