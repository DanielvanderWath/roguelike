#include "hand.h"
#include "game.h"
#include <iostream>

using namespace std;

Hand::Hand(void){ onehanded=true; shield=false;}
Hand::~Hand(void){}

void Hand::setWielder(Character *w)
{
	wielder = w;
}

bool Hand::isOneHanded(void)
{
	return onehanded;
}

bool Hand::isShield(void)
{
	return shield;
}

bool Hand::isWeapon(void)
{
	return weapon;
}

void Hand::dumpStats(int indent)
{
	OUTPUTI("This shouldn't have been called: " << __func__ << ":" << __LINE__, indent);
}
