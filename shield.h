#ifndef __SHIELD_H__
#define __SHIELD_H__
#include "hand.h"
#include "armour.h"

class Shield : public Hand, public Armour
{
	int bash_force;//use this for a knockback attack
public:
	Shield(void);
	~Shield(void);
	Shield(const char *n, int av, Resistance _resistance, int special, int bash_force);
};

#endif

