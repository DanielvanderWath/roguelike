#include "resistance.h"
#include "display.h"
#include "misc.h"
#include <sstream>

Resistance::Resistance(void){fire=0; ice=0; lightning=0;poison=0;}
Resistance::~Resistance(void){}

Resistance::Resistance(const Resistance *r)
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

// *** return a sting describing the resistances ***
std::string Resistance::getInvString(void)
{
	std::stringstream strRet;

	if(fire)
		strRet << "F" << fire;
	if(ice)
		strRet << "I" << ice;
	if(lightning)
		strRet << "L" << lightning;
	if(poison)
		strRet << "P" << poison;

	//Prepend the string with something descriptive only if there's something to describe
	if(strRet.str().length())
		strRet.str("Res: " + strRet.str());

	return strRet.str();
}

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

