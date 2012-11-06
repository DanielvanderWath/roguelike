#include "item.h"
#include <iostream>

using namespace std;

Item::Item(void){slot=0; name=NULL; icon=' ';}
Item::~Item(void){}

void Item::dumpStats(void)
{
	cout << "Name:\t" << name << "\nIcon:\t" << icon << endl;
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
	if(allowed_slots.empty())
		cout << "allowed_slots is empty" << endl;
	else
		cout << "allowed_slots is " << allowed_slots.size() << " elements long" << endl;
	for(list<int>::iterator it=allowed_slots.begin(); it != allowed_slots.end(); it++)
		if((int)*it == _slot)
			return true;
	return false;
}

const char* Item::getName(void)
{
	return name;
}

