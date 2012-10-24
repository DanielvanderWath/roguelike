#include <iostream>
#include "game.h"

using namespace std;

int main(int argc, char **argv)
{
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
	pc.dumpStats(0);
	return 0;
}
