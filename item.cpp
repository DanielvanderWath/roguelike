#include "item.h"
#include <iostream>

using namespace std;

Item::Item(void){slot=0; name=NULL; icon=' ';}
Item::~Item(void){}

void Item::dumpStats(void)
{
	cout << "Name:\t" << name << "\nIcon:\t" << icon << endl;
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

const char* Item::getName(void)
{
	return name;
}

