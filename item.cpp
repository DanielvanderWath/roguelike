#include "item.h"
#include <iostream>

using namespace std;

Item::Item(void){}
Item::~Item(void){}

void Item::dumpStats(void)
{
	cout << "Name:\t" << name << "\nIcon:\t" << icon << endl;
}
