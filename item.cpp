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

const char* Item::getName(void)
{
	return name;
}

