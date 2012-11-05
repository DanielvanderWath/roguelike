#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include "weapon.h"
#include <list>

//these are used to tell the item which slot it is in
#define SLOT_ARMOUR 1
#define SLOT_LEFT_HAND 2
#define SLOT_RIGHT_HAND 3

using namespace std;

class Character
{
private:
	int hp, hpmax, mp, mpmax;
	char *name;
	Race *race;
	//Class *cclass;
	Armour *armour;
	Hand *left, *right;
	list<Item*> inventory;
public:
	Character(void);
	~Character(void);
	Character(const char *n, Race *r);
	void dumpStats(int indent);
	void equip(Armour *a);
	void equip(Hand *a, HAND_WHICH hand);
	void unequip(Item *a);
	Armour* getArmour(void);
	Hand* getLeftHand(void);
	Hand* getRightHand(void);
};


#endif

