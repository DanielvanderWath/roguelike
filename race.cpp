#include "race.h"
#include <string.h>
#include <cstdlib>

Race::Race(void){}
Race::~Race(void){}
Race::Race(std::string n, int iStrMin, int iStrVar, int iConstMin, int iConstVar):NamedThing(n)
{
	iStrengthMin = iStrMin;
	iStrengthVar = iStrVar;
	iConstitutionMin = iConstMin;
	iConstitutionVar = iConstVar;
}

int Race::genStrength(void)
{
	return iStrengthMin + (rand() % iStrengthVar);
}

int Race::genConstition(void)
{
	return iConstitutionMin + (rand() % iConstitutionVar);
}
