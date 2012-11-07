#ifndef __WEAPON_H__
#define __WEAPON_H__
#include <list>
#include "hand.h"
#include "effect.h"
#include "game.h"

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
	Weapon(const char *n, int dmin, int dminvar, int dvar, int dvarvar, bool one);
	void dumpSpecials(int s, int indent);
	void dumpStats(int indent);
	bool attack(Character *target, bool offHand);
	void addEffect(Effect *effect);
};
#endif

