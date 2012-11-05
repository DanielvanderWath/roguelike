#include "resistance.h"

Resistance::Resistance(void){fire=0; ice=0; lightning=0;poison=0;}
Resistance::~Resistance(void){}

Resistance::Resistance(const Resistance &r)
{
	zero();
	add((Resistance*)&r);
}

Resistance::Resistance(int _fire, int _ice, int _lightning, int _poison)
{
	fire=_fire;
	ice=_ice;
	lightning=_lightning;
	poison=_poison;
}

int Resistance::getFire(void){return fire;}
int Resistance::getIce(void){return ice;}
int Resistance::getLightning(void){return lightning;}
int Resistance::getPoison(void){return poison;}

void Resistance::zero(void)
{
	fire=0;
	ice=0;
	lightning=0;
	poison=0;
}

void Resistance::add(int _fire, int _ice, int _lightning, int _poison)
{
	fire+=_fire;
	ice+=_ice;
	lightning+=_lightning;
	poison+=_poison;
}

void Resistance::add(Resistance *other)
{
	fire+=other->getFire();
	ice+=other->getIce();
	lightning+=other->getLightning();
	poison+=other->getPoison();
}

