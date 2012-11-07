#include <iostream>
#include <string.h>
#include "buff.h"
#include "character.h"

class Character;

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

bool Buff::tick(Character* target)
{
	if(hpPerTurn)
	{
		target->addHP(hpPerTurn);

		if(hpPerTurn < 0)
			OUTPUT( name << " did " << -hpPerTurn << " damage to " << target->getName() );
		else
			OUTPUT( target->getName() << " was healed " << hpPerTurn << "HP by " << name );
	}
	if(mpPerTurn)
	{
		target->addMP(mpPerTurn);

		if(mpPerTurn < 0)
			OUTPUT( name << " did " << -mpPerTurn << " damage to " << target->getName() );
		else
			OUTPUT( target->getName() << " was recharged " << mpPerTurn << "MP by " << name );
	}
	if(--timeLeft > 0)
		return false;
	return true;
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

