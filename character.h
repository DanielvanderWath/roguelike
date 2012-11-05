#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include "weapon.h"
#include "resistance.h"
#include <list>


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
	int AV;
	Resistance resistance;
	int armourSpecial;
	char *name;
	Gender gender;
	Race *race;
	//Class *cclass;
	Armour *torso;
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
	void calcDefence(void);
	int getAV(void);
	Armour* getTorso(void);
	Hand* getLeftHand(void);
	Hand* getRightHand(void);
};


#endif

