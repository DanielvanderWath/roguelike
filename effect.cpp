#include <iostream>
#include <string.h>
#include "effect.h"

using namespace std;

Effect::Effect(void){}
Effect::~Effect(void){}

Effect::Effect(const char *n, EFFECT_TYPE _eType, int magnitudeA, int magnitudeB)
{
	name = new char[strlen(n)+1];
	strcpy(name, n);
	eType = _eType;
	mA = magnitudeA;
	mB = magnitudeB;

	switch(eType)
	{
		case WEAPON_EFFECT_POISON:
			cast = &Effect::poison;
			break;
		default:
			OUTPUT( "ERROR: invalid effect type" );
	}
}

void Effect::callEffectFunc(Character *wielder, Character *target, bool crit)
{
	if(cast && wielder && target)
	{
		(this->*((Effect*)this)->Effect::cast)(wielder, target, crit);
	}
}

void Effect::poison(Character *wielder, Character *target, bool crit)
{
	if(crit)//poison target on crit
	{
		Buff *buffPoison = new Buff(name, mB);
		buffPoison->setHPPerTurn(-mA);
		target->addBuff(buffPoison);
	}
}

