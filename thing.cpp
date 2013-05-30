#include "thing.h"

Thing::Thing(void){appearance = '.';} 
Thing::~Thing(void){}
Thing::Thing(char a){appearance = a;}
char Thing::getAppearance(void){return appearance;}
void Thing::setAppearance(char a){appearance = a;}

NamedThing::NamedThing(void){}
NamedThing::~NamedThing(void){}
NamedThing::NamedThing(std::string n): Thing()
{
	name = n;
}

NamedThing::NamedThing(std::string n, char a): Thing(a)
{
	name = n;
}

std::string* NamedThing::getName(void)
{
	return &name;
}

