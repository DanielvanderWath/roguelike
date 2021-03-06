#include "weapon.h"
#include "display.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

Weapon::Weapon(void):Item(){}
Weapon::~Weapon(void){}

Weapon::Weapon(std::string n, int dmin, int dminvar, int dvar, int dvarvar, bool one):Item(n, '(')
{
	damage_min = dmin + (rand() % dminvar);
	damage_var = dvar + (rand() % dvarvar);

	onehanded = one;
	weapon = true;
	shield = false;

	allowed_slots.push_back(SLOT_HAND_LEFT);
	allowed_slots.push_back(SLOT_HAND_RIGHT);
}

void Weapon::dumpSpecials(int s, int indent)
{
	OUTPUTI("Specials:", indent)
	if( WEAPON_EFFECT_POISON & s )
		OUTPUTI("Poison", indent+1);
}

void Weapon::dumpStats(int indent)
{
	OUTPUTI("Name:\t" << name, indent);
	OUTPUTI("Icon:\t" << getAppearance(), indent);
	OUTPUTI("Damage:\t" << damage_min << "-" << damage_min + damage_var, indent);
	//TODO: rewrite dumpSpecials to use effects
	//dumpSpecials(special, indent+1);
}

bool Weapon::attack(Character *target, bool offHand, int iWielderStrength)
{
	//decide how much damage the weapon wants to do, and any special effects. Tell the target, and let it decide what to do.
	int baseDamage = iWielderStrength + damage_min + (rand() % (damage_var + 1));

	//if the character is dual wielding and this weapon is in the offhand, half the damage TODO: refine this
	if(offHand)
		baseDamage /= 2;

	//return true on critical hit so the character can trigger any procs
	bool crit = target->hitPhysical(baseDamage);

	for(list<Effect*>::iterator it = effects.begin(); it != effects.end(); it++)
	{
		(*it)->callEffectFunc(wielder, target, crit);
	}

	return crit;
}

void Weapon::addEffect(Effect *effect)
{
	effects.push_back(effect);
}

// *** return a string, primarily used for describing the item's important stats ***
std::string* Weapon::getInvString(void)
{
	if(strStats.length() == 0)
	{
		std::stringstream strStream;

		strStream << name << "(" << COLOR_CODE_GREEN << damage_min << COLOR_CODE_DEFAULT << "-" << COLOR_CODE_RED << damage_min + damage_var << COLOR_CODE_DEFAULT << ")";
		for(list<Effect*>::iterator it=effects.begin(); it != effects.end(); it++)
			strStream << " " << *(*it)->getInvString();
		strStats = strStream.str();
	}
	return &strStats;
}

// *** quantify weapon desirability for AI equipping decisions ***
int Weapon::getQualityScore(void)
{
	//TODO: take more into account than max possible damage
	return damage_min + damage_var;
}

