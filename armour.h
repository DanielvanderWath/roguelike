#ifndef __ARMOUR_H__
#define __ARMOUR_H__
#include <list>
#include "item.h"
#include "resistance.h"

#define ARMOUR_SPECIAL_REGEN 1 << 0
#define ARMOUR_SPECIAL_RECHARGE 1 << 1

using namespace std;

class Armour : virtual public Item
{
protected:
	int AV;//armour value
	Resistance resistance;//packed resistances Fire8Ice8Lightning8Poison8. or something like that
public:
	Armour(void);
	~Armour(void);
	Armour(const char *n, int av_min, int av_var,  Resistance *_resistance, list<Buff*> *_buffs, list<int> _allowed_slots);
	void dumpResistances(Resistance r, int indent);
	void dumpBuffs(list<Buff*> s, int indent);
	void dumpStats(int indent);
	int getAV(void);
	Resistance* getResistance(void);
	list<Buff*>* getBuffs(void);
};


#endif

