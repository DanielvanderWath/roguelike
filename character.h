#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "race.h"
#include "armour.h"
#include "weapon.h"
#include "resistance.h"
#include "buff.h"
#include "floor.h"
#include <list>
#include <string>

class Armour;
class Item;
class Buff;
class FloorTile;

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
	FloorTile *position;
	Resistance resistance;
	int armourSpecial;
	string name;
	char appearance;
	Gender gender;
	Race *race;
	//Class *cclass;
	Armour *torso;
	Hand *left, *right;
	list<Item*> inventory;
	list<Buff*> buffs;

	void unequip_post(Item *a);
public:
	Character(void);
	~Character(void);
	Character(std::string *n, Race *r, Gender g);
	void dumpStats(int indent);
	void listInventory(void);
	void giveInventory(list<Item*> *target);
	list<Item*>* getInventory(void);
	void equip(Item *a, int slot);
	void unequip(Item *a);
	void unequip(int iSlot);
	void calcDefence(void);
	bool attackBasic(Character *target);
	bool hitPhysical(int damage);//attack* functions are this character attacking something else. hit* functions are this character being hit by something else
	void pickUp(Item* item);
	void addBuff(Buff *b);
	void addMP(int m);
	void addHP(int h);
	void tickBuffs(void);
	void removeAllBuffs(void);
	void moveTo(FloorTile *tile);
	FloorTile* getPosition(void);
	std::string getName(void);
	char getAppearance(void);
	int getAV(void);
	int getHP(void);
	int getHPMax(void);
	int getXP(void);
	void addXP(int _xp);
	int getXPValue(void);
	void setXPValue(int _xp);
	Armour* getTorso(void);
	Hand* getLeftHand(void);
	Hand* getRightHand(void);
	bool isDead(void);
	void bump(void);
};


#endif

