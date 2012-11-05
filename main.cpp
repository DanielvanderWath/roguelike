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
	Character pc = Character("Dan", &races[0]);

	//a piece of armour
	Armour *breastPlate = new Armour("Lazarus Suit", 5, 0x05000000, ARMOUR_SPECIAL_REGEN);
	pc.equip(breastPlate);

	//a weapon
	Weapon *dagger = new Weapon("Assassin's blade", 3, 2, 2, 2, WEAPON_SPECIAL_POISON, true);
	pc.equip(dagger, HAND_LEFT);
	pc.dumpStats(0);

	cout << "\nUnequipping left hand\n" << endl;
	pc.unequip(pc.getLeftHand());

	pc.dumpStats(0);
	return 0;
}
