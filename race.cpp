#include "race.h"
#include <string.h>
#include <cstdlib>

Race::Race(void){}
Race::~Race(void){}
Race::Race(const char *n, int hpm, int hpv, int mpm, int mpv)
{
	_name = new char[strlen(n) +1];
	strcpy(_name, n);

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

const char *Race::name(void)
{
	return _name;
}
