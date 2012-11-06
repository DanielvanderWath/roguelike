#include "hand.h"
#include <iostream>

using namespace std;

Hand::Hand(void){ onehanded=true; shield=false;}
Hand::~Hand(void){}

bool Hand::isOneHanded(void)
{
	return onehanded;
}

bool Hand::isShield(void)
{
	return shield;
}

void Hand::dumpStats(int indent)
{
	cout << "This shouldn't have been called: " << __func__ << ":" << __LINE__ << endl;
}
