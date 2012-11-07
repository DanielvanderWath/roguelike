#ifndef __WEAPON_H__
#define __WEAPON_H__
#include "hand.h"
#include "effect.h"
#include "game.h"

class Character;

#define WEAPON_SPECIAL_POISON 1 << 0
class Weapon : public Hand
{
private:
	int damage_min, damage_var;
	list<Effect*> effects;
	Character *wielder;
public:
	Weapon(void);
	~Weapon(void);
	Weapon(const char *n, int dmin, int dminvar, int dvar, int dvarvar, int s, bool one);
	void setWielder(Character *w);
	void dumpSpecials(int s, int indent);
	void dumpStats(int indent);
	bool attack(Character *target, bool offHand);
	void addEffect(Effect *effect);
};
#endif

