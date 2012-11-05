#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include "weapon.h"
#include <list>

//these are used to tell the item which slot it is in
#define SLOT_ARMOUR 1
#define SLOT_HAND_LEFT 2
#define SLOT_HAND_RIGHT 3

using namespace std;

enum Gender
{
	MALE=0,
	FEMALE,
	IT,
	THEM,
};

class Character
{
private:
	int hp, hpmax, mp, mpmax;
	char *name;
	Gender gender;
	Race *race;
	//Class *cclass;
	Armour *armour;
	Hand *left, *right;
	list<Item*> inventory;
public:
	Character(void);
	~Character(void);
	Character(const char *n, Race *r, Gender g);
	void dumpStats(int indent);
	void listInventory(void);
	void equip(Item *a, int slot);
	void unequip(Item *a);
	Armour* getArmour(void);
	Hand* getLeftHand(void);
	Hand* getRightHand(void);
};


#endif

