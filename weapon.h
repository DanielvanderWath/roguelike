#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include "hand.h"
#include "effect.h"
#include "misc.h"
#include "display.h"

class Character;
class Effect;

using namespace std;

class Weapon : public Hand
{
private:
	int damage_min, damage_var;
	list<Effect*> effects;
public:
	Weapon(void);
	~Weapon(void);
	Weapon(std::string n, int dmin, int dminvar, int dvar, int dvarvar, bool one);
	void dumpSpecials(int s, int indent);
	void dumpStats(int indent);
	bool attack(Character *target, bool offHand);
	void addEffect(Effect *effect);
	std::string* getInvString(void);
};
#endif

