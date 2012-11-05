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
	Armour *breastPlate = new Armour("Lazarus Suit", 5, 0x05000000, ARMOUR_SPECIAL_REGEN);
	pc.equip(breastPlate, SLOT_ARMOUR);

	//a weapon
	Weapon *sledge = new Weapon("Massive sledgehammer", 8, 6, 2, 4, 0, false);
	pc.equip(sledge, SLOT_HAND_LEFT);

	//and another
	Weapon *dagger = new Weapon("Assassin's blade", 3, 2, 2, 2, WEAPON_SPECIAL_POISON, true);
	pc.equip(dagger, SLOT_HAND_RIGHT);
	cout << "Dagger has been equipped in slot: " <<  dagger->getSlot() << endl;
	
	pc.dumpStats(0);

	cout << "\nUnequipping dagger\n" << endl;
	pc.unequip(dagger);

	pc.dumpStats(0);
	return 0;
}
