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

const char *personal_pronoun[]={	"he",
					"she",
					"it",
					"they",
				};

Character::Character(void){}
Character::~Character(void)
{
	//inventory should have already been dropped
	if(!inventory.empty())
		OUTPUT("ERROR: " << name << "'s inventory was not dropped before " << personal_pronoun[gender] << " was deleted");

	if(!buffs.empty())
	{
		removeAllBuffs();
	}

	delete name;
}

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
	xp = 0;
	xpValue = 0;

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
		<< indenter << "XP:\t" << xp << "\n" 
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
	if(!a)
	{
		//there is no item in this slot
		return;
	}

	switch(a->getSlot())
	{
		case SLOT_TORSO:
			torso = NULL;		
			break;
		case SLOT_HAND_LEFT:
			dynamic_cast<Weapon*>(left)->setWielder(NULL);
			left = NULL;
			break;
		case SLOT_HAND_RIGHT:
			dynamic_cast<Weapon*>(right)->setWielder(NULL);
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

void Character::giveInventory(list<Item*> *target)
{
	//unequip everything
	unequip(torso);
	unequip(left);
	unequip(right);

	while(!inventory.empty())
	{
		target->push_back(inventory.front());
		inventory.pop_front();
	}
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

				dynamic_cast<Hand*>(a)->setWielder(this);
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
				dynamic_cast<Hand*>(a)->setWielder(this);
				break;
			default:
				OUTPUT( "Error: Trying to equip item in invalid slot");
				return;
		}
		//recalculate defensive stats after equipping an item
		calcDefence();
	}
}

void Character::calcDefence(void)
{
	//store the sum of all armours' AVs and resistances in AV
	AV = 0;
	resistance.zero();
	if(torso)
	{
		AV += torso->getAV();
		resistance.add(torso->getResistance());
	}
	if(left && left->isShield())
	{
		AV += dynamic_cast<Shield*>(left)->getAV();
		resistance.add(dynamic_cast<Shield*>(left)->getResistance());
	}
	if(right && right->isShield())
	{
		AV += dynamic_cast<Shield*>(right)->getAV();
		resistance.add(dynamic_cast<Shield*>(right)->getResistance());
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
		OUTPUT( name << " attacks " << target->getName() << " with " << possessive_pronoun[gender] << " " << (bRight ? right : left)->getName());
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
		OUTPUT( name << " blocked the attack and recieved no damage" );
	}
	else if( roll < chanceBlock + chanceCrit )
	{
		//critical hit! 
		crit = true;
		//TODO: how massive should the damage be? double for now
		damage10 *=2;
		OUTPUT( name << " was hit critically and recieved "  << damage10/10 << " damage" );
	}
	else
	{
		//armour reduces the damage (by no more than 90%)
		//here, if damage is AV*2 there is no reduction
		damage10 = CLAMP(damage, 3*damage10/2 - AV10, damage10);
		OUTPUT( name << " recieved "  << damage10/10 << " damage" );
	}

	damage = damage10/10;

	//apply the damage done
	hp -= damage;

	return crit;
}

void Character::pickUp(Item* item)
{
	if(item)
	{
		inventory.push_back(item);
		OUTPUT(name << " picked up the " << item->getName() );
	}
}

void Character::addBuff(Buff *b)
{
	buffs.push_back(b);
	OUTPUT( name << " is now under the effect of " << b->getName() );
}

void Character::addMP(int m)
{
	mp += m;
	if(mp > mpmax)
		mp = mpmax;
}

void Character::addHP(int h)
{
	hp += h;
	if(hp > hpmax)
		hp = hpmax;
}

void Character::tickBuffs(void)
{
	if(!buffs.empty())
	{
		OUTPUT ( name << " has " << buffs.size() << " active effects" );
		for(list<Buff*>::iterator it = buffs.begin(); it != buffs.end(); it++)
		{
			if((*it)->tick(this))
			{
				OUTPUT( name << "'s "  << (*it)->getName() << " has worn off" );
				delete (*it);
				buffs.erase(it);
				if( it != buffs.begin())
					it--;
			}
		}
	}
}

void Character::removeAllBuffs(void)
{
	while(!buffs.empty())
	{
		buffs.pop_front();
	}
}

void moveTo(FloorTile *tile)
{
	tile->occupy(this);
	position = tile;
}

FloorTile* getPosition(void)
{
	return position;
}

const char* Character::getName(void)
{
	return (const char*) name;
}

int Character::getAV(void)
{
	return AV;
}

int Character::getXP(void)
{
	return xp;
}

void Character::addXP(int _xp)
{
	xp += _xp;
	OUTPUT(name << " gained " << _xp << " XP!" );
	//TODO: level up
}

int Character::getXPValue(void)
{
	return xpValue;
}

void Character::setXPValue(int _xp)
{
	//use this for setting the XP value for NPCs
	xpValue = _xp;
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

bool Character::isDead(void)
{
	if(hp < 1)
	{
		//this character has just been killed
		OUTPUT( name << " is Dead" );
		return true;
	}
	return false;
}

