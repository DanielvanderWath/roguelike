#include "game.h"
#include "armour.h"
#include <iostream>
#include <string.h>

using namespace std;

Armour::Armour(void){}
Armour::~Armour(void){}
Armour::Armour(const char *n, int av, Resistance _resistance, int _special, list<int> _allowed_slots)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);

	AV = av;
	resistance = Resistance(_resistance);
	special = _special;
	icon = '[';

	allowed_slots = list<int> (_allowed_slots);
}

void Armour::dumpResistances(Resistance r, int indent)
{
	INDENTER(indent, indenter)
	 
	//no new line at start or end, no explicit flush
	cout 	<< indenter << "Fire:\t\t" << r.getFire()
		<< "\n" << indenter << "Ice:\t\t" << r.getIce()
		<< "\n" << indenter << "Lightning\t" << r.getLightning()
		<< "\n" << indenter << "Poison\t\t" << r.getPoison();

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

	cout << "\n" << indenter << "Resistances:\n";
	dumpResistances(resistance, indent);

	if(special)
	{
		cout << "\n" << indenter << "Specials:\n";
		dumpSpecials(special, indent);
	}
	cout << endl;

	delete indenter;
}

int Armour::getAV(void)
{
	return AV;
}

Resistance* Armour::getResistance(void)
{
	return &resistance;
}

int Armour::getSpecial(void)
{
	return special;
}

