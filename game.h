#ifndef __GAME_H__
#define __GAME_H__

#include "race.h"
#include "display.h"
#include "armour.h"
#include "hand.h"
#include "weapon.h"
#include "character.h"
#include "shield.h"
#include "misc.h"

class Floor;

class Game
{
protected:
	Floor *floor;
	Character *pc;
	bool quit;
	//Drawing class
	Display display;
public:
	void init(void);
	Character *createPlayer(void);
	DIRECTION getDirectionFromKey(int key);
	void doActionFromUser(void);
	void kill(Character *killer, Character **killed);
	bool moveCharacter(Character *c, DIRECTION dir);
	Floor *getFloor(void);
	void mainLoop(void);
};

#endif

