#include "game.h"
#include "armour.h"
#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;

Armour::Armour(void){}
Armour::~Armour(void){}
Armour::Armour(std::string n, int av_min, int av_var, Resistance *_resistance,  list<Buff*> *_buffs, list<int> _allowed_slots)
{
	//TODO: error check
	name = n;

	AV = av_min + (rand() % ++av_var);
	if(_resistance)
		resistance = _resistance;
	else
		resistance = new Resistance();
	icon = '[';

	allowed_slots = _allowed_slots;

	if(_buffs)
		for(list<Buff*>::iterator it = _buffs->begin(); it != _buffs->end(); it++)
			addBuff(*it);
}

void Armour::dumpResistances(Resistance r, int indent)
{
	indent++;
	//no new line at start or end, no explicit flush
	OUTPUTI( "Fire:\t" << r.getFire(), indent );
	OUTPUTI( "Ice:\t" << r.getIce(), indent );
	OUTPUTI( "Lightning\t" << r.getLightning(), indent );
	OUTPUTI( "Poison\t" << r.getPoison(), indent );
}

void Armour::dumpBuffs(list<Buff*> s, int indent)
{
	indent++;

	for(list<Buff*>::iterator it = buffs.begin(); it != buffs.end(); it++)
	{
		OUTPUTI((*it)->getName(), indent);
	}

}

void Armour::dumpStats(int indent)
{
	OUTPUTI("Name:\t" << name, indent);
	OUTPUTI("Icon:\t" << icon, indent);
	OUTPUTI("AV:\t" << AV, indent);

	OUTPUTI("Resistances:\n", indent);
		dumpResistances(resistance, indent);

	if(!buffs.empty())
	{
		OUTPUTI("Buffs:", indent);
		dumpBuffs(buffs, indent);
	}
}

int Armour::getAV(void)
{
	return AV;
}

Resistance* Armour::getResistance(void)
{
	return resistance;
}

list<Buff*>* Armour::getBuffs(void)
{
	return &buffs;
}

// *** return a string, primarily used for describing the item's important stats ***
std::string *Armour::getInvString(void)
{
	if(strStats.length() == 0)
	{
		std::stringstream strStream;
		std::string strRes = resistance->getInvString();
		strStream << name << " (AV:" << AV << (strRes.length() == 0 ? "" : (" " + strRes)) << ")";
		strStats = strStream.str();
	}
	return &strStats; 
}
