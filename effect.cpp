#include <iostream>
#include <string>
#include "effect.h"
#include "display.h"

using namespace std;

Effect::Effect(void){}
Effect::~Effect(void){}

Effect::Effect(std::string n, EFFECT_TYPE _eType, int magnitudeA, int magnitudeB)
{
	name = n;
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

// *** return a string, primarily used for describing the item's important stats ***
std::string *Effect::getInvString(void)
{
	if(strStats.length() == 0)
	{
		std::stringstream strStream;
		switch(eType)
		{
			case EFFECT_NONE:
				break;
			case WEAPON_EFFECT_POISON:
				strStream << name << ": " << COLOR_CODE_RED << mA << COLOR_CODE_DEFAULT << " damage over " << COLOR_CODE_BLUE << mB << COLOR_CODE_DEFAULT << " turns";
				break;
			default:
				OUTPUT("ERROR: Invalid effect " << eType);
		}
		strStats = strStream.str();
	}
	return &strStats; 
}
