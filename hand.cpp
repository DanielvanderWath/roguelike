#include "hand.h"

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

