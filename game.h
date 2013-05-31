#ifndef __GAME_H__
#define __GAME_H__

#include "ai.h"
#include "character.h"
#include "display.h"
#include "misc.h"

class Floor;
class Display;
class AI;

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
	void equipItemFromInventory(void);
	void doActionFromUser(void);
	void kill(Character *killer, Character *killed);
	void attack(Character *pAttacker, Character *pVictim);
	bool moveCharacter(Character *c, DIRECTION dir);
	bool moveCharacterTowards(Character *c, FloorTile *pTile);
	void pickUpItems(Character *c, list<Item*>* items, int maxAllowed);
	void spawnItem(FloorTile *tile);
	void spawnMonster(FloorTile *tile);
	Floor *getFloor(void);
	void tickAI(void);
	void mainLoop(void);
};

#endif

