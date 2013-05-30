#include "item.h"
#include "game.h"
#include <iostream>

std::string Item::astrSlotNames[] = {	"Inventory",
						"Torso",
						"Left Hand",
						"Right Hand",
						};

Item::Item(void):NamedThing(){slot=0;}
Item::~Item(void){}
Item::Item(std::string n, char a):NamedThing(n, a){slot=0;}

void Item::dumpStats(void)
{
	OUTPUT("Name:\t" << getName() << "\nIcon:\t" << getAppearance());
}

void Item::addBuff(Buff *_buff)
{
	buffs.push_back(_buff);
}

int Item::getSlot(void)
{
	return slot;
}

void Item::setSlot(int s)
{
	slot = s;
}

bool Item::isAllowedInSlot(int _slot)
{
	for(list<int>::iterator it=allowed_slots.begin(); it != allowed_slots.end(); it++)
		if((int)*it == _slot)
			return true;
	return false;
}

std::string* Item::getName(void)
{
	return &name;
}

// *** return a string, primarily used for describing the item's important stats ***
std::string* Item::getInvString(void)
{
	return &name;
}
