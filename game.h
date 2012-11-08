#ifndef __GAME_H__
#define __GAME_H__

#include <string>
#include <iostream>
#include <sstream>

#include "race.h"
#include "display.h"
#include "armour.h"
#include "hand.h"
#include "weapon.h"
#include "character.h"
#include "shield.h"
#include "floor.h"

//TODO: make this safe
#define INDENTER(X, Y)	char *Y = new char[X+1]; \
			memset(Y, '\t', X); \
			Y[X] = 0;//NULL terminate

//#define OUTPUT(X) Display::output( (std::ostringstream str << "---" << X).str() )
#define OUTPUT(X) \
		{ std::ostringstream str ; \
		str << "---" << X ; \
		Display::output(str.str() );}

//#define OUTPUT(X) std::cout << "\n---" << X << endl

#define CLAMP(low, X, high) (low > X ? low : high < X ? high : X)

#endif

