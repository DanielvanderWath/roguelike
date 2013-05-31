#ifndef __AI_H__
#define __AI_H__

#include "character.h"
#include "floor.h"

class Game; 
class FloorTile; 

enum AIState
{
	AI_WANDERING,
	AI_CHASING,
	AI_ATTACKING,
	AI_GETTING,
	AI_PICKING_UP,
};

class AI
{
protected:
	Character *pCharacter;
	AIState eState;
	FloorTile* pTargetLocation;
	Character* pTarget;
	int iSightRange;
public:
	AI(void);
	~AI(void);
	AI(Character *pCh);
	FloorTile* search(Game *pGame, Character *pTarg);
	bool searchForNewTarget(Game *pGame);
	bool canSee(Game *pGame, Character* pTarg);
	void wander(Game *pGame);
	void updateState(Game *pGame);
	void tick(Game *pGame);
	void setState(AIState state);
	AIState getState(void);
	Item* getBestItem(Item *A, Item *B);
	void equipBestItems(void);
};

#endif
