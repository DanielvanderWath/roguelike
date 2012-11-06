#include "weapon.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;

Weapon::Weapon(void){name = NULL;}
Weapon::~Weapon(void){if(name) delete name;}

Weapon::Weapon(const char *n, int dmin, int dminvar, int dvar, int dvarvar, int s, bool one)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);

	damage_min = dmin + (rand() % dminvar);
	damage_var = dvar + (rand() % dvarvar);

	special = s;

	onehanded = one;
	weapon = true;
	shield = false;

	icon = ')';

	allowed_slots.push_back(SLOT_HAND_LEFT);
	allowed_slots.push_back(SLOT_HAND_RIGHT);
}

void Weapon::dumpSpecials(int s, int indent)
{
	INDENTER(indent, indenter)

	cout << indenter << "Specials:";
	if( WEAPON_SPECIAL_POISON & s )
		cout << "\n" << indenter << "Poison";

	cout << endl;

	delete indenter;
}

void Weapon::dumpStats(int indent)
{
	indent++;
	INDENTER(indent, indenter)

	cout << indenter << "Name:\t" << name << "\n" << indenter << "Icon:\t" << icon << "\n" << indenter << "Damage:\t" << damage_min << "-" << damage_min + damage_var << "\n";

	dumpSpecials(special, indent);

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

	return crit;
}

