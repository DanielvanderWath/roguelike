#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "character.h"

class Character;

enum EFFECT_TYPE
{
	EFFECT_NONE=0,
	WEAPON_EFFECT_POISON,
};

class Effect
{
protected:
	std::string name;
	std::string strStats;
	EFFECT_TYPE eType;
	int mA, mB;//depending on the nature of the effect. e.g. poison is damage and duration
	void (Effect::*cast)(Character*, Character*, bool);
public:
	Effect(void);
	~Effect(void);
	Effect(std::string n, EFFECT_TYPE _eType, int magnitudeA, int magnitudeB);
	//effects
	void callEffectFunc(Character *wielder, Character *target, bool crit);
	void poison(Character *wielder, Character *target, bool crit);
	std::string* getInvString(void);
};

#endif

