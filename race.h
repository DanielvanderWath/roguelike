#ifndef __RACE_H__
#define __RACE_H__

#include <string>
#include "thing.h"

class Race : public NamedThing
{
private:
	int iStrengthMin, iStrengthVar;
	int iConstitutionMin, iConstitutionVar;
public:
	Race(void);
	~Race(void);
	Race(std::string n, int iStrMin, int iStrVar, int iConstMin, int iConstVar);
	int genStrength(void);
	int genConstition(void);
};

#endif

