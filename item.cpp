#include "item.h"
#include "game.h"
#include <iostream>

std::string Item::astrSlotNames[] = {	"Inventory",
						"Torso",
						"Left Hand",
						"Right Hand",
						};

Item::Item(void){slot=0; name=""; icon=' ';}
Item::~Item(void){}

void Item::dumpStats(void)
{
	OUTPUT("Name:\t" << name << "\nIcon:\t" << icon);
}

void Item::addBuff(Buff *_buff)
{
	buffs.push_back(_buff);
}

int Item::getSlot(void)
{
	return slot;
}

char Item::getAppearance(void)
{
	return icon;
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

