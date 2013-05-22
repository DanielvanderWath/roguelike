#ifndef __SHIELD_H__
#define __SHIELD_H__
#include "hand.h"
#include "armour.h"

class Shield : public Hand, public Armour
{
	int bash_force;//use this for a knockback attack
public:
	Shield(void);
	~Shield(void);
	Shield(std::string n, int av_min, int av_var, Resistance *_resistance, list<Buff*> *_buffs, int bash_force);
	virtual void dumpStats(int indent);
};

#endif

