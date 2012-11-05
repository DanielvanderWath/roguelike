#ifndef __ARMOUR_H__
#define __ARMOUR_H__
#include <list>
#include "item.h"
#include "resistance.h"

#define ARMOUR_SPECIAL_REGEN 1 << 0
#define ARMOUR_SPECIAL_RECHARGE 1 << 1

using namespace std;

class Armour : public Item
{
protected:
	int AV;//armour value
	Resistance resistance;//packed resistances Fire8Ice8Lightning8Poison8. or something like that
	int special;//bitfield containing special effects
public:
	Armour(void);
	~Armour(void);
	Armour(const char *n, int av, Resistance _resistance, int _special, list<int> _allowed_slots);
	void dumpResistances(Resistance r, int indent);
	void dumpSpecials(int s, int indent);
	void dumpStats(int indent);
	int getAV(void);
	Resistance* getResistance(void);
	int getSpecial(void);
};


#endif

