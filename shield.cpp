#include "shield.h"
#include <string.h>
#include <cstdlib>

Shield::Shield(void){}
Shield::~Shield(void){}

Shield::Shield(const char *n, int av_min, int av_var, Resistance *_resistance, list<Buff*> *_buffs, int bash_force): Armour(n, av_min, av_var, _resistance, _buffs, list<int>())
{
	shield = true;

	allowed_slots.push_back(SLOT_HAND_LEFT);
	allowed_slots.push_back(SLOT_HAND_RIGHT);
}

void Shield::dumpStats(int indent)
{
	Armour::dumpStats(indent);
}

