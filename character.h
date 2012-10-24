#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include <list>

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
	Armour* getArmour(void);
	void dumpStats(int indent);
	void equip(Armour *a);
	void unequip(Item *a);
};


#endif

