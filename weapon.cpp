#include "weapon.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

Weapon::Weapon(void){name = "";}
Weapon::~Weapon(void){}

Weapon::Weapon(std::string n, int dmin, int dminvar, int dvar, int dvarvar, bool one)
{
	//TODO: error check
	name = n;
	

	damage_min = dmin + (rand() % dminvar);
	damage_var = dvar + (rand() % dvarvar);

	onehanded = one;
	weapon = true;
	shield = false;

	icon = ')';

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
	OUTPUTI("Icon:\t" << icon, indent);
	OUTPUTI("Damage:\t" << damage_min << "-" << damage_min + damage_var, indent);
	//TODO: rewrite dumpSpecials to use effects
	//dumpSpecials(special, indent+1);
}

bool Weapon::attack(Character *target, bool offHand)
{
	//decide how much damage the weapon wants to do, and any special effects. Tell the target, and let it decide what to do.
	int baseDamage = damage_min + (rand() % (damage_var + 1));

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


