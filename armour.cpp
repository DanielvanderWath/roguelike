#include "game.h"
#include "armour.h"
#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;

Armour::Armour(void){}
Armour::~Armour(void){}
Armour::Armour(const char *n, int av_min, int av_var, Resistance *_resistance,  list<Buff*> *_buffs, list<int> _allowed_slots)
{
	//TODO: error check
	name = new char[strlen(n)+1];
	strcpy(name, n);

	AV = av_min + (rand() % ++av_var);
	if(_resistance)
		resistance = Resistance(_resistance);
	else
		resistance.zero();
	icon = '[';

	allowed_slots = _allowed_slots;

	if(_buffs)
		for(list<Buff*>::iterator it = _buffs->begin(); it != _buffs->end(); it++)
			addBuff(*it);
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

void Armour::dumpBuffs(list<Buff*> s, int indent)
{
	indent++;
	INDENTER(indent, indenter)

	for(list<Buff*>::iterator it = buffs.begin(); it != buffs.end(); it++)
	{
		cout << "\n" << indenter << (*it)->getName(); 
	}

	delete indenter;
}

void Armour::dumpStats(int indent)
{
	INDENTER(indent, indenter)
	++indent;
	cout << indenter << "Name:\t" << name << "\n" << indenter << "Icon:\t" << icon << "\n" << indenter << "AV:\t" << AV ;

	cout << "\n" << indenter << "Resistances:\n";
	dumpResistances(resistance, indent);

	if(!buffs.empty())
	{
		cout << "\n" << indenter << "Buffs:\n";
		dumpBuffs(buffs, indent);
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

list<Buff*>* Armour::getBuffs(void)
{
	return &buffs;
}

