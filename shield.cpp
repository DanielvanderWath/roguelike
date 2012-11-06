#include "shield.h"
#include <string.h>

Shield::Shield(void){}
Shield::~Shield(void){}

Shield::Shield(const char *n, int av, Resistance _resistance, int _special, int bash_force)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);
	
	AV = av;
	resistance = Resistance(_resistance);
	special = _special;
	icon = '[';

	allowed_slots.push_back(SLOT_HAND_LEFT);
	allowed_slots.push_back(SLOT_HAND_RIGHT);
}

