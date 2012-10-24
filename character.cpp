#include <iostream>
#include <string.h>
#include "character.h"
#include "game.h"

using namespace std;

Character::Character(void){}
Character::~Character(void){}
Character::Character(const char *n, Race *r)
{
	name = new char[strlen(n)+1];
	strcpy(name, n);

	race = r;
	hpmax = race->hp();
	hp = hpmax;
	mpmax = race->mp();
	mp = mpmax;

	/*cclass = NULL;*/
	armour =NULL;	
	/*left = NULL;
	right = NULL;*/
}

void Character::dumpStats(int indent)
{
	INDENTER(indent, indenter)
	cout << indenter << "Name:\t" << name << "\n" << indenter << "Race:\t" << race->name() << "\n" << indenter << "HP:\t" << hpmax << "\n" << indenter << "MP:\t" << mpmax << "\n" << endl;

	cout << "Armour:";
	if(armour)
		armour->dumpStats(indent);
	else
		cout << "\tNone" << endl;

	delete indenter;
}

void Character::unequip(Item *a)
{
	inventory.push_back(a);
	a = NULL;
	
}

void Character::equip(Armour *a)
{
	if(armour)
		unequip(armour);

	armour = a;
}

