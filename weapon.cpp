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

	icon = ')';
}

bool Weapon::isOneHanded(void)
{
	return onehanded;
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
