#ifndef __GAME_H__
#define __GAME_H__

#include "race.h"
#include "armour.h"
#include "hand.h"
#include "weapon.h"
#include "character.h"

//TODO: make this safe
#define INDENTER(X, Y)	char *Y = new char[X+1]; \
			memset(Y, '\t', X); \
			Y[X] = 0;//NULL terminate

#define OUTPUT(X) cout << "\n---" << X << endl


#endif

