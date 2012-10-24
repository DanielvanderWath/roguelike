#include "game.h"
#include "armour.h"
#include <iostream>
#include <string.h>

using namespace std;

Armour::Armour(void){}
Armour::~Armour(void){}
Armour::Armour(const char *n, int av, int _resistance, int _special)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);

	AV = av;
	resistance = _resistance;
	special = _special;
	icon = '[';
}



void Armour::dumpResistances(int r, int indent)
{
	INDENTER(indent, indenter)
	 
	//no new line at start or end, no explicit flush
	cout 	<< indenter << "Fire:\t\t" << ((r & 0xFF000000) >> 6)
		<< "\n" << indenter << "Ice:\t\t" << ((r & 0x00FF0000) >> 4)
		<< "\n" << indenter << "Lightning\t" << ((r & 0x0000FF00) >> 2)
		<< "\n" << indenter << "Poison\t\t" << (r & 0x000000FF);

	delete indenter;
}

void Armour::dumpSpecials(int s, int indent)
{
	INDENTER(indent, indenter)

	cout << indenter << "Specials:";
	if( ARMOUR_SPECIAL_REGEN & s )
		cout << "\n" << indenter << "Regen health";
	if( ARMOUR_SPECIAL_RECHARGE & s )
		cout << "\n" << indenter << "Recharge mana";

	delete indenter;
}

void Armour::dumpStats(int indent)
{
	INDENTER(indent++, indenter)
	cout << indenter << "Name:\t" << name << "\n" << indenter << "Icon:\t" << icon << "\n" << indenter << "AV:\t" << AV ;

	if(resistance)
	{
		cout << "\n" << indenter << "Resistances:\n";
		dumpResistances(resistance, indent);
	}
	if(special)
	{
		cout << "\n" << indenter << "Specials:\n";
		dumpSpecials(special, indent);
	}
	cout << endl;

	delete indenter;
}
