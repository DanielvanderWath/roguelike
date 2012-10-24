#ifndef __ARMOUR_H__
#define __ARMOUR_H__
#include "item.h"

#define ARMOUR_SPECIAL_REGEN 1 << 0
#define ARMOUR_SPECIAL_RECHARGE 1 << 1

class Armour : public Item
{
private:
	int AV;//armour value
	int resistance;//packed resistances Fire8Ice8Lightning8Poison8. or something like that
	int special;//bitfield containing special effects
public:
	Armour(void);
	~Armour(void);
	Armour(const char *n, int av, int _resistance, int _special);
	void dumpResistances(int r, int indent);
	void dumpSpecials(int s, int indent);
	void dumpStats(int indent);
};


#endif

