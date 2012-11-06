#include <iostream>
#include <cstdlib>
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
	torso =NULL;	
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
	indent++;
	cout 	<< indenter << "Name:\t" << name << "\n" 
		<< indenter << "Race:\t" << race->name() << "\n" 
		<< indenter << "HP:\t" << hp << "/" << hpmax << "\n" 
		<< "AV:\t" << AV  << "\n" 
		<< indenter << "MP:\t" << mpmax << "\n" 
		<< endl;

	dump("Torso:", torso)
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
		case SLOT_TORSO:
			torso = NULL;		
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
	calcDefence();//recalculate defensive stats after unequipping an item
	OUTPUT( name << " unequipped " << a->getName());
}

void Character::equip(Item *a, int slot)
{
	if(a->isAllowedInSlot(slot))
	{
		switch(slot)
		{
			case SLOT_TORSO:
				if(torso)
					unequip(torso);

				torso = dynamic_cast<Armour*>(a);
				a->setSlot(SLOT_TORSO);
				break;
			case SLOT_HAND_LEFT:
				//if there is a two handed weapon in the other hand then refuse to equip anything in this hand
				if(right && !((Hand*)right)->isOneHanded())
				{
					OUTPUT("Attempting to equip item in offhand while wielding twohander");
					return;
				}
					
				//if there is already something in this hand then unequip it
				if(left)
					unequip(left);

				//if this is a twohander then unequip the item in the offhand
				if(!dynamic_cast<Hand*>(a)->isOneHanded() && right)
					unequip(right);

				left = dynamic_cast<Hand*>(a);
				((Item*)a)->setSlot(SLOT_HAND_LEFT);
				break;
			case SLOT_HAND_RIGHT:
				//if there is a two handed weapon in the other hand then refuse to equip anything in this hand
				if(left && !((Hand*)left)->isOneHanded())
				{
					OUTPUT("Attempting to equip item in offhand while wielding twohander");
					return;
				}
					
				//if there is already something in this hand then unequip it
				if(right)
					unequip(right);

				//if this is a twohander then unequip the item in the offhand
				if(!dynamic_cast<Hand*>(a)->isOneHanded() && left)
					unequip(left);

				right = dynamic_cast<Hand*>(a);
				((Item*)a)->setSlot(SLOT_HAND_RIGHT);
				break;
			default:
				cout << "Error: Trying to equip item in invalid slot" << endl;
				return;
		}
		//recalculate defensive stats after equipping an item
		calcDefence();
	}
}

void Character::calcDefence(void)
{
	//store the sum of all armours' AVs in AV
	AV = 0;
	if(torso)
		AV += torso->getAV();
	if(left && left->isShield())
		AV += ((Armour*)left)->getAV();//TODO: change these Armours to Shields
	if(right && right->isShield())
		AV += ((Armour*)right)->getAV();

	//and the resistances in resistance
	resistance.zero();
	if(torso)
	{
		resistance.add(torso->getResistance());

	}
}

bool Character::attackBasic(Character *target)
{
	bool bDual = false, bRight = false, bNoWeapon = false, bCrit = false;
	
	//first work out if this is going to be one handed (and if so which hand) or dual handed
	if (right && right->isWeapon())
	{
		bRight = true;
		if (left && left->isWeapon()) 
			bDual == true;
	}
	else if (left && left->isWeapon()) 
	{
		bRight = false;
	}
	else
	{
		//not holding a weapon in either hand
		bNoWeapon = true;
	}

	if(!bDual)
	{
		//attacking with one weapon. TODO: accuracy and dodge
		bCrit = dynamic_cast<Weapon*>(bRight ? right : left)->attack(target, false);
		
	}
	else if(!bNoWeapon)
	{
		//attacking with both weapons. first decide which is the main hand. TODO: should the character have a preference, or should we go off heaviest/largest/most powerful? Assume right handed for the time being
		bCrit = dynamic_cast<Weapon*>(right)->attack(target, false);
		if(!bCrit)
			bCrit = dynamic_cast<Weapon*>(left)->attack(target, true);
	}
	else
	{
		//TODO: decide what to do when attacking without a weapon
	}

	//tell the caller if we scored a critical hit
	return bCrit;
}

bool Character::hitPhysical(int damage)
{
	bool crit = false;
	int chanceBlock = 250, chanceCrit = 25, roll = (rand() % 1001);
	int AV10 = AV * 10, damage10 = damage * 10;


	//if the damage is equal to our AV, we want about a 25% chance (clamped between 1% and 50%) of the blow being blocked completely, and a 2.5% chance of critting (TODO: make crit chance weightable by weapon)
	chanceBlock += AV10 - damage10;
	chanceBlock = CLAMP( 10, chanceBlock, 500);

	if(roll < chanceBlock)
	{
		//blocked, no damage! there might still be effects though. TODO: report this
		damage10 = 0;
		cout << name << " blocked the attack and recieved no damage" << endl;
	}
	else if( roll < chanceBlock + chanceCrit )
	{
		//critical hit! 
		crit = true;
		//TODO: how massive should the damage be? double for now
		damage10 *=2;
		cout << name << " was hit critically and recieved "  << damage10/10 << " damage" << endl;
	}
	else
	{
		//armour reduces the damage (by no more than 90%)
		//here, if damage is AV*2 there is no reduction
		damage10 = CLAMP(damage, 3*damage10/2 - AV10, damage10);
		cout << name << " recieved "  << damage10/10 << " damage" << endl;
	}

	damage = damage10/10;

	//apply the damage done
	hp -= damage;

	return crit;
}

int Character::getAV(void)
{
	return AV;
}

Armour* Character::getTorso(void)
{
	return torso;
}

Hand* Character::getLeftHand(void)
{
	return left;
}

Hand* Character::getRightHand(void)
{
	return right;
}

