#ifndef __RACE_H__
#define __RACE_H__

#include <string>
#include "thing.h"

class Race : public NamedThing
{
private:
	int hp_min, hp_variance;
	int mp_min, mp_variance;
public:
	Race(void);
	~Race(void);
	Race(std::string n, int hpm, int hpv, int mpm, int mpv);
	int hp(void);
	int mp(void);
};

#endif

