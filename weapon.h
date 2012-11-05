#ifndef __WEAPON_H__
#define __WEAPON_H__
#include "hand.h"
#include "game.h"

#define WEAPON_SPECIAL_POISON 1 << 0
class Weapon : public Hand
{
private:
	int damage_min, damage_var;
	int special;//bitfield containing special effects
	bool onehanded;
public:
	Weapon(void);
	~Weapon(void);
	Weapon(const char *n, int dmin, int dminvar, int dvar, int dvarvar, int s, bool one);
	bool isOneHanded(void);
	void dumpStats(int indent);
	void dumpSpecials(int s, int indent);
};
#endif

