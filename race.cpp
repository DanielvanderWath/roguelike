#include "race.h"
#include <string.h>
#include <cstdlib>

Race::Race(void){}
Race::~Race(void){}
Race::Race(std::string n, int hpm, int hpv, int mpm, int mpv):NamedThing(n)
{

	hp_min=hpm;
	hp_variance=hpv;
	mp_min=mpm;
	mp_variance=mpv;
}

int Race::hp(void)
{
	return hp_min + (rand() % hp_variance);
}

int Race::mp(void)
{
	return mp_min + (rand() % mp_variance);
}
