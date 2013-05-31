#include <iostream>
#include <cstdlib>
#include <string.h>
#include "character.h"
#include "game.h"
#include "shield.h"

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

Character::Character(void):NamedThing(){}
Character::~Character(void)
{
	//inventory should have already been dropped
	if(!inventory.empty())
		OUTPUT("ERROR: " << name << "'s inventory was not dropped before " << personal_pronoun[gender] << " was deleted");

	if(!buffs.empty())
	{
		removeAllBuffs();
	}
}

Character::Character(std::string n, Race *r, Gender g):NamedThing(n, '@')
{
	OUTPUT("name = " << name << endl);

	gender = g;

	race = r;
	iStrength = race->genStrength();
	iConstitution = race->genConstition();
	xp = 0;
	xpValue = 0;

	/*cclass = NULL;*/
	torso =NULL;	
	left = NULL;
	right = NULL;

	position = NULL;

	calcStats();

	hp=hpmax;
}

#define dump(X, Y)	OUTPUTI(X, indent); \
			if(Y) \
				Y->dumpStats(indent+1); \
			else \
				OUTPUTI("None", indent);

void Character::dumpStats(int indent)
{
	OUTPUTI("Name:\t" << name, indent);
	OUTPUTI("Race:\t" << race->getName(), indent);
	OUTPUTI("HP:\t" << hp << "/" << hpmax, indent);
	OUTPUTI("AV:\t" << AV, indent);
	OUTPUTI("MP:\t" << mpmax, indent);
	OUTPUTI("XP:\t" << xp, indent);

	dump("Torso:", torso)
	dump("Right Hand:", right)
	dump("Left Hand:", left)

	listInventory();

}

void Character::listInventory(void)
{
	if(!inventory.empty())
	{
		OUTPUT(name << " has the following items in " << possessive_pronoun[gender] << " inventory:");

		for(list<Item*>::iterator it=inventory.begin(); it != inventory.end(); it++)
			OUTPUT(*(*it)->getInvString());
	}
	else
		OUTPUT(name << " has nothing in " << possessive_pronoun[gender] << " inventory.");
}

// *** post unequip housekeeping ***
void Character::unequip_post(Item *a)
{
	//do nothing if passed a NULL item
	if(a)
	{
		inventory.push_back(a);// do this after so that we don't add an invalid item to the inventory in the event of an error
		a->setSlot(0);//remove the reference to the equipment slot from the item
		calcStats();//recalculate defensive stats after unequipping an item
	}
}

// *** unequip an item ***
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
			dynamic_cast<Hand*>(left)->setWielder(NULL);
			left = NULL;
			break;
		case SLOT_HAND_RIGHT:
			dynamic_cast<Hand*>(right)->setWielder(NULL);
			right = NULL;
			break;
		default:
			OUTPUT("Error: trying to unequip item from invalid slot");
			return;
	}
	unequip_post(a);
}

// *** unquip item in slot ***
void Character::unequip(int iSlot)
{
	Item *a = NULL;

	switch(iSlot)
	{
		case SLOT_TORSO:
			if(torso)
				a = torso;
			torso = NULL;
			break;
		case SLOT_HAND_LEFT:
			if(left)
				a = left;
			left = NULL;
			break;
		case SLOT_HAND_RIGHT:
			if(right)
				a = right;
			right = NULL;
			break;
		default:
			OUTPUT("Error: invalid slot " <<  iSlot);
			return;
	}
	unequip_post(a);
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

list<Item*>* Character::getInventory(void)
{
	return &inventory;
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
		calcStats();
	}
}

void Character::calcStats(void)
{
	//Calculate effective stats first, others are based on them
	iStrengthEffective = iStrength;
	iConstitutionEffective = iConstitution;

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

	//calculate max hp
	hpmax = iStrengthEffective * 2 + iConstitutionEffective * 5;

	//clamp current hp to hpmax
	if(hp > hpmax)
		hp = hpmax;
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

	if(bNoWeapon)
	{
		//unarmed attack
		//TODO: decide what to do when attacking without a weapon
		OUTPUT("TODO: nothing happens when you attack without a weapon" << endl);
	}
	else if(!bDual)
	{
		//attacking with one weapon. TODO: accuracy and dodge
		OUTPUT( name << " attacks " << *target->getName() << " with " << possessive_pronoun[gender] << " " << *(bRight ? right : left)->getName());
		bCrit = dynamic_cast<Weapon*>(bRight ? right : left)->attack(target, false, iStrengthEffective);
	}
	else
	{
		//attacking with both weapons. first decide which is the main hand. TODO: should the character have a preference, or should we go off heaviest/largest/most powerful? Assume right handed for the time being
		bCrit = dynamic_cast<Weapon*>(right)->attack(target, false, iStrengthEffective);
		if(!bCrit)
			bCrit = dynamic_cast<Weapon*>(left)->attack(target, true, iStrengthEffective);
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
		OUTPUT(name << " picked up the " << *(item->getName()) );
	}
}

void Character::addBuff(Buff *b)
{
	buffs.push_back(b);
	OUTPUT( name << " is now under the effect of " << *b->getName() );
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

// *** Move Character from one tile to another ***
void Character::moveTo(FloorTile *tile)
{
	//we might not have a position right now, e.g. at the start of the game
	if(position)
		position->leave();

	//returns true on success. 
	if(tile->occupy(this))
		position = tile;
}

// *** Return the FloorTile the Character is currently standing on ***
FloorTile* Character::getPosition(void)
{
	return position;
}

// *** return the Character's armour value ***
int Character::getAV(void)
{
	return AV;
}

// *** return the Character's current hp ***
int Character::getHP(void)
{
	return hp;
}

// *** return the Character's max hp ***
int Character::getHPMax(void)
{
	return hpmax;
}

// *** return the Character's current XP count ***
int Character::getXP(void)
{
	return xp;
}

// *** increase the Character's XP ***
void Character::addXP(int _xp)
{
	xp += _xp;
	OUTPUT(name << " gained " << _xp << " XP!" );
	//TODO: level up
}

// *** return the Character's current XP count ***
int Character::getXPValue(void)
{
	return xpValue;
}

// *** Force the Character's current XP count to a specific value ***
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
		return true;
	}
	return false;
}

void Character::bump(void)
{
	OUTPUT(name << " walks into a wall!" << endl);
}
