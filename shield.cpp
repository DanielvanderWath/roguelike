#include "shield.h"
#include <string.h>
#include <cstdlib>

Shield::Shield(void){}
Shield::~Shield(void){}

Shield::Shield(const char *n, int av_min, int av_var, Resistance *_resistance, int _special, int bash_force)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);
	
	AV = av_min + (rand() % ++av_var);
	if(_resistance)
		resistance = Resistance(_resistance);
	else
		resistance.zero();

	special = _special;
	icon = '[';
	shield = true;

	allowed_slots.push_back(SLOT_HAND_LEFT);
	allowed_slots.push_back(SLOT_HAND_RIGHT);
}

void Shield::dumpStats(int indent)
{
	Armour::dumpStats(indent);
}

