#include <iostream>
#include <string.h>
#include "buff.h"

Buff::Buff(void)
{
	name = NULL;
	hpPerTurn = 0;
	mpPerTurn = 0;
	AV = 0;
	damage = 0;
	timeLeft = 0;
}

Buff::~Buff(void)
{
	delete name;
}

Buff::Buff(const char* _n, int _timeLeft)
{
	name = new char[strlen(_n)+1];
	strcpy(name, _n);

	timeLeft = _timeLeft;
}

bool Buff::tick(void)
{
	if(--timeLeft > 0)
		return true;
	return false;
}

const char* Buff::getName(void)
{
	return (const char*)name;
}

void Buff::setHPPerTurn(int hp)
{
	hpPerTurn = hp;
}

void Buff::setMPPerTurn(int mp)
{
	mpPerTurn = mp;
}

void Buff::setAV(int _av)
{
	AV = _av;
}

void Buff::setDamage(int _damage)
{
	damage = _damage;
}

