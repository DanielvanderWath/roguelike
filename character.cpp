#include <iostream>
#include <string.h>
#include "character.h"
#include "game.h"

using namespace std;

const char *possessive_pronoun[]={	"his",
					"her",
					"its",
					"their",
					};

Character::Character(void){}
Character::~Character(void){}
Character::Character(const char *n, Race *r, Gender g)
{
	name = new char[strlen(n)+1];
	strcpy(name, n);

	gender = g;

	race = r;
	hpmax = race->hp();
	hp = hpmax;
	mpmax = race->mp();
	mp = mpmax;

	/*cclass = NULL;*/
	armour =NULL;	
	left = NULL;
	right = NULL;
}

#define dump(X, Y)	cout << X << "\n"; \
			if(Y) \
				Y->dumpStats(indent); \
			else \
				cout << "\tNone" << endl;

void Character::dumpStats(int indent)
{
	INDENTER(indent, indenter)
	cout << indenter << "Name:\t" << name << "\n" << indenter << "Race:\t" << race->name() << "\n" << indenter << "HP:\t" << hpmax << "\n" << indenter << "MP:\t" << mpmax << "\n" << endl;

	dump("Armour:", armour)
	dump("Right Hand:", right)
	dump("Left Hand:", left)

	listInventory();

	delete indenter;
}

void Character::listInventory(void)
{
	if(!inventory.empty())
	{
		cout << name << " has the following items in " << possessive_pronoun[gender] << " inventory:\n";

		for(list<Item*>::iterator it=inventory.begin(); it != inventory.end(); it++)
			cout << "\t" << (*it)->getName() << "\n";
	}
	else
		cout << name << " has nothing in " << possessive_pronoun[gender] << " inventory.\n";
}

void Character::unequip(Item *a)
{
	switch(a->getSlot())
	{
		case SLOT_ARMOUR:
			armour = NULL;		
			break;
		case SLOT_HAND_LEFT:
			left = NULL;
			break;
		case SLOT_HAND_RIGHT:
			right = NULL;
			break;
		default:
			cout << "Error: trying to unequip item from invalid slot" << endl;
			return;
	}
	inventory.push_back(a);// do this after so that we don't add an invalid item to the inventory in the event of an error
	a->setSlot(0);//remove the reference to the equipment slot from the item
}

void Character::equip(Item *a, int slot)
{
	switch(slot)
	{
		case SLOT_ARMOUR:
			if(armour)
				unequip(armour);

			armour = (Armour*)a;
			a->setSlot(SLOT_ARMOUR);
			break;
		case SLOT_HAND_LEFT:
			//if there is a two handed weapon in the other hand then refuse to equip anything in this hand
			if(right && !((Weapon*)right)->isOneHanded())
			{
				OUTPUT("Attempting to equip item in offhand while wielding twohander");
				return;
			}
				
			//if there is already something in this hand then unequip it
			if(left)
				unequip(left);

			left = (Hand*)a;
			a->setSlot(SLOT_HAND_LEFT);
			break;
		case SLOT_HAND_RIGHT:
			//if there is a two handed weapon in the other hand then refuse to equip anything in this hand
			if(left && !((Weapon*)left)->isOneHanded())
			{
				OUTPUT("Attempting to equip item in offhand while wielding twohander");
				return;
			}
				
			//if there is already something in this hand then unequip it
			if(right)
				unequip(right);

			right = (Hand*)a;
			a->setSlot(SLOT_HAND_RIGHT);
			break;
		default:
			cout << "Error: Trying to equip item in invalid slot" << endl;
			return;
	}
}

Armour* Character::getArmour(void)
{
	return armour;
}

Hand* Character::getLeftHand(void)
{
	return left;
}

Hand* Character::getRightHand(void)
{
	return right;
}

