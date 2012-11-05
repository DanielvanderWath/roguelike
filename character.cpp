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

	/*cout << "Armour:";
	if(armour)
		armour->dumpStats(indent);
	else
		cout << "\tNone" << endl;

	cout <<"Right Hand:";
	if(right)
		right->dump*/

	dump("Armour:", armour)
	dump("Right Hand:", right)
	dump("Left Hand:", left)

	delete indenter;
}

void Character::unequip(Item *a)
{
	switch(a->getSlot())
	{
		case SLOT_ARMOUR:
			armour = NULL;		
			break;
		case SLOT_LEFT_HAND:
			left = NULL;
			break;
		case SLOT_RIGHT_HAND:
			right = NULL;
			break;
		default:
			cout << "Error: trying to unequip item from invalid slot" << endl;
			return;
	}
	inventory.push_back(a);// do this after so that we don't add an invalid item to the inventory in the event of an error
}

void Character::equip(Armour *a)
{
	if(armour)
		unequip(armour);

	armour = a;
	a->setSlot(SLOT_ARMOUR);
}

void Character::equip(Hand *a, HAND_WHICH hand)
{
	if(hand ? right : left )
		unequip(hand ? right : left);

	hand ? right : left = a;
	a->setSlot(hand ? SLOT_RIGHT_HAND : SLOT_LEFT_HAND);
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

