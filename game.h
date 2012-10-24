#ifndef __GAME_H__
#define __GAME_H__

#include "race.h"
#include "armour.h"
#include "character.h"

//TODO: make this safe
#define INDENTER(X, Y)	char *Y = new char[X+1]; \
			memset(Y, '\t', X); \
			Y[X] = 0;//NULL terminate


#endif

