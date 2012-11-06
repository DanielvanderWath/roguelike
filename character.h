#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include "weapon.h"
#include "resistance.h"
#include "buff.h"
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
	int xp, xpValue;
	Resistance resistance;
	int armourSpecial;
	char *name;
	Gender gender;
	Race *race;
	//Class *cclass;
	Armour *torso;
	Hand *left, *right;
	list<Item*> inventory;
	list<Buff*> buffs;
public:
	Character(void);
	~Character(void);
	Character(const char *n, Race *r, Gender g);
	void dumpStats(int indent);
	void listInventory(void);
	void giveInventory(list<Item*> *target);
	void equip(Item *a, int slot);
	void unequip(Item *a);
	void calcDefence(void);
	bool attackBasic(Character *target);
	bool hitPhysical(int damage);//attack* functions are this character attacking something else. hit* functions are this character being hit by something else
	void addBuff(Buff *b);
	void removeAllBuffs(void);
	const char* getName(void);
	int getAV(void);
	int getXP(void);
	void addXP(int _xp);
	int getXPValue(void);
	void setXPValue(int _xp);
	Armour* getTorso(void);
	Hand* getLeftHand(void);
	Hand* getRightHand(void);
	bool isDead(void);
};


#endif

