#include <iostream>
#include <string.h>
#include "buff.h"
#include "character.h"
#include "display.h"

class Character;

Buff::Buff(void)
{
	name = "";
	hpPerTurn = 0;
	mpPerTurn = 0;
	AV = 0;
	damage = 0;
	timeLeft = 0;
}

Buff::~Buff(void)
{
}

Buff::Buff(std::string _n, int _timeLeft)
{
	name = _n;

	timeLeft = _timeLeft;
}

bool Buff::tick(Character* target)
{
	if(hpPerTurn)
	{
		target->addHP(hpPerTurn);

		if(hpPerTurn < 0)
			OUTPUT( name << " did " << -hpPerTurn << " damage to " << target->getName() )
		else
			OUTPUT( target->getName() << " was healed " << hpPerTurn << "HP by " << name )
	}
	if(mpPerTurn)
	{
		target->addMP(mpPerTurn);

		if(mpPerTurn < 0)
			OUTPUT( name << " did " << -mpPerTurn << " damage to " << target->getName() )
		else
			OUTPUT( target->getName() << " was recharged " << mpPerTurn << "MP by " << name )
	}
	if(--timeLeft > 0)
		return false;
	return true;
}

std::string* Buff::getName(void)
{
	return &name;
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

