#include <iostream>
#include "game.h"
#include <ctime>
#include <cstdlib>

using namespace std;

void init(void)
{
	srand(time(0));
}

void kill(Character *killer, Character *killed, list<Item*> *lstFloor)
{
	//take all of its items
	list<Item*> lstDrop;
	killed->giveInventory(&lstDrop);
	//drop them on the floor
	if(!lstDrop.empty())
	{
		OUTPUT(killed->getName() << " drops:" );
		while(!lstDrop.empty())
		{
			OUTPUT("\t" << lstDrop.front()->getName() );
			lstFloor->push_back(lstDrop.front());
			lstDrop.pop_front();
		}
	}

	killer->addXP(killed->getXPValue());
	delete killed;
}

int main(int argc, char **argv)
{
	init();

	//this list will serve as the inventory of a floor tile. Later each floor tile will have its own.
	list<Item*> lstFloor;

	//TODO: load this from a file later
	//int num_races = 3;
	Race races[]={	Race("Human", 28, 4, 8, 4),
			Race("Dwarf", 10, 6, 4, 2),
			Race("Goblin", 24, 4, 10, 4),
			};
	//player, TODO: ask them for name, class and race
	Character *pc = new Character("Dan", &races[0], MALE);

	//a piece of armour
	Armour *breastPlate = new Armour("Lazarus Suit", 4, 3, NULL, NULL, list<int>(1, SLOT_TORSO));
	{
		Buff *regen = new Buff("Regeneration: 1", -1);
		regen->setHPPerTurn(1);
		breastPlate->addBuff(regen);
	}
	{
		Resistance *tmp = breastPlate->getResistance();
		tmp->add(5, 0, 0, 0);
	}
	pc->equip(breastPlate, SLOT_TORSO);

	//a weapon
	Weapon *sledge = new Weapon("Massive sledgehammer", 8, 6, 2, 4, 0, false);
	pc->equip(sledge, SLOT_HAND_LEFT);

	//and another
	Weapon *dagger = new Weapon("Assassin's blade", 3, 2, 2, 2, WEAPON_SPECIAL_POISON, true);
	pc->equip(dagger, SLOT_HAND_RIGHT);
	pc->dumpStats(0);
	pc->unequip(sledge);
	pc->equip(dagger, SLOT_HAND_RIGHT);
	pc->dumpStats(0);

	//add a shield
	Shield *aspis = new Shield("Bronze Aspis", 3, 2, NULL, 0, 5);
	pc->equip(aspis, SLOT_HAND_LEFT);

	pc->dumpStats(0);

	//spawn a goblin
	Character *goblin = new Character("Grizott", &races[2], FEMALE);
	goblin->setXPValue((rand() % 20) + 20);

	//give the goblin a weapon
	Weapon *stick = new Weapon("Pointy stick", 2, 2, 2, 2, 0, true);
	goblin->equip(stick, SLOT_HAND_RIGHT);


	//fight!
	bool bOver = false, bPlayerTurn = true;
	while(!bOver)
	{
		(bPlayerTurn ? pc : goblin)->attackBasic(bPlayerTurn ? goblin : pc);
		if((bPlayerTurn ? goblin: pc)->isDead())
		{
			kill((bPlayerTurn ? pc : goblin), (bPlayerTurn ? goblin: pc), &lstFloor);
			bOver = true;
		}

		usleep(500000);

		bPlayerTurn = !bPlayerTurn;
	}

	//see if the goblin died, if so reward the pc

	return 0;
}
