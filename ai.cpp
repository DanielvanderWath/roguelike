#include "ai.h"
#include "display.h"
#include "game.h"
#include <cstdlib>

AI::AI(void){}
AI::~AI(void){delete pCharacter;}

AI::AI(Character *pCh)
{
	pCharacter = pCh;
	eState = AI_WANDERING;
	iSightRange = 4;
}

// *** search for a specific target, or if pTarg is NULL return the first thing found
FloorTile* AI::search(Game *pGame, Character *pTarg)
{
	Floor *pFloor = pGame->getFloor();
	//find the nearest tile with something in it. Expand the search radius until something is
	//found or we've reached the limit of sight range
	for(int iRadius = 1; iRadius <= iSightRange; iRadius++)
	{
		int left = pCharacter->getPosition()->getX()-iRadius;
		int right = pCharacter->getPosition()->getX()+iRadius;
		int bottom = pCharacter->getPosition()->getY()+iRadius;
		int top = pCharacter->getPosition()->getY()-iRadius;

		//clamp to floor boundaries
		if(left < 0)
			left = 0;
		if(right > pFloor->getRight())
			right = pFloor->getRight();
		if(top < 0)
			top = 0;
		if(bottom > pFloor->getBottom())
			bottom = pFloor->getBottom();

		//top and bottom rows
		//1 2 3
		//4   5
		//6 7 8
		for(int y = top; y <= bottom; y++)
		{
			for(int x = left; x <= right; (y == top || y == bottom) ? x++ : (x+= right - left))
			{
				if(pFloor->getTile(x, y)->isOccupied())
				{
					//if pTarg is NULL we'll take the first thing we find
					if(!pTarg || pFloor->getTile(x, y)->getOccupier() == pTarg)
						return pFloor->getTile(x, y);
				}
			}
		}
	}

	//if we failed to find what we were looking for, return NULL
	return NULL;
}

// *** find a target for this character to pick up/chase ***
bool AI::searchForNewTarget(Game *pGame)
{
	FloorTile* pTile = search(pGame, NULL);

	if(pTile)
	{
		pTargetLocation = pTile;
		if(pTile->getOccupier())
		{
			//there's a character there, chase it!
			eState = AI_CHASING;
			pTarget = pTile->getOccupier();
		}
		else
		{
			//There must be an item here, pick it up
			eState = AI_GETTING;
		}
		return true;
	}
	return false;
}

// *** check if we can see a character ***
bool AI::canSee(Game *pGame, Character* pTarg)
{
	//search returns NULL if it didn't find what we asked it to
	if(search(pGame, pTarg))
	{
		return true;
	}
	return false;
}

// *** choose a new destination to wander to ***
void AI::wander(Game *pGame)
{
	DIRECTION eWanderDirection = (DIRECTION)(rand() % (int)DIRECTION_INVALID);
	int iFloorWidth = pGame->getFloor()->getWidth();
	int iFloorHeight = pGame->getFloor()->getHeight();
	int iDistance = rand()% ( iFloorWidth < iFloorHeight ? iFloorWidth : iFloorHeight) / 4;

	eState = AI_WANDERING;
}

// *** update the current objective ***
void AI::updateState(Game *pGame)
{
	switch(eState)
	{
		case AI_WANDERING:
			//is there anything interesting in our line of sight?
			if(searchForNewTarget(pGame))
			{
				break;
			}
			//have we arrived at our target location without finding anything interesting? if so, wander off somewhere else
			if(pCharacter->getPosition() == pTargetLocation)
			{
				wander(pGame);
			}
			break;
		case AI_CHASING:
			//update target location if we can still see it
			if(canSee(pGame, pTarget))
			{
				pTargetLocation = pTarget->getPosition();
			}

			//have we reached our target?
			if(pCharacter->getPosition()->isAdjacent(pTargetLocation))
			{
				eState = AI_ATTACKING;
			}
			break;
		case AI_ATTACKING:
			//be careful, we don't want to segfault if pTarget has been killed by something else
			if(	pTargetLocation->isAdjacent(pCharacter->getPosition()) &&
				(pTargetLocation->getOccupier() == pTarget))
			{
				//the target is still alive and standing next to us, so continue attacking
			}
			else
			{
				//our target is not standing there any more
				//is our target still in view?
				if(canSee(pGame, pTarget))
				{
					//so it still exists. chase it
					eState = AI_CHASING;
				}
				else
				{
					//oh well.
					pTarget = NULL;
					pTargetLocation = NULL;
					wander(pGame);
				}
			}
		case AI_GETTING:
			//have we reached our target?
			if(pCharacter->getPosition() == pTargetLocation)
			{
				eState = AI_PICKING_UP;
			}
			break;
		case AI_PICKING_UP:
			//this shouldn't happen, we should have picked up the item last turn and set eState to AI_WANDERING
		default:
			OUTPUT("ERROR: Invalid AI state" << endl);
			break;
	}
}

// *** do the character's action for this turn ***
void AI::tick(Game *pGame)
{
	Character *pEnemy;
	list<Item*> *lFloorInv;

	//first update the character's state
	updateState(pGame);

	switch(eState)
	{
		case AI_WANDERING:
			//move towards wander target, a location
			pGame->moveCharacterTowards(pCharacter, pTargetLocation);
			break;
		case AI_CHASING:
		case AI_GETTING:
			//move towards target, a character, and attack it
			pGame->moveCharacterTowards(pCharacter, pTargetLocation);
			break;
		case AI_ATTACKING:
			pEnemy = pTargetLocation->getOccupier();
			//make sure tile is still occupied
			if(pEnemy == pTarget)//this implies that pEnemy is not NULL
			{
				//then attack!
				pGame->attack(pCharacter, pEnemy);
			}
			break;
		case AI_PICKING_UP:
			//we're standing where we believe there is an item, pick it up
			lFloorInv = pCharacter->getPosition()->getInventory();
			while(!lFloorInv->empty())
			{
				pCharacter->pickUp(lFloorInv->back());
				lFloorInv->pop_back();
			}
			equipBestItems();
			wander(pGame);
			break;
	}
}

void AI::setState(AIState state)
{
	eState = state;
}

AIState AI::getState(void)
{
	return eState;
}

// *** return a pointer to the item with the highest quality score ***
Item* AI::getBestItem(Item* A, Item* B)
{
	// if this is the first item that fits in this slot select it
	if(!A)
	{
		return B;
	}
	else if(!B)
	{
		return A;
	}
	else//if not, then compare it to the current best choice
	{
		if(A->getQualityScore() < B->getQualityScore())
		{
			return B;
		}
		else return A;
	}

}

// *** go through the items in the character's inventory and equip the best ones
void AI::equipBestItems(void)
{
	list<Item*> *plInventory = pCharacter->getInventory(); 
	Item *pBestRight=pCharacter->getRightHand(), *pBestLeft=pCharacter->getLeftHand(), *pBestTorso=pCharacter->getTorso();

	for(list<Item*>::iterator it = plInventory->begin(); it != plInventory->end(); it++)
	{
		if((*it)->isAllowedInSlot(SLOT_HAND_RIGHT) )
		{
			//only equip a weapon in the right hand
			if(dynamic_cast<Hand*>(*it)->isWeapon())
			{
				pBestRight = getBestItem(pBestRight, (*it));
			}
		}
		else if((*it)->isAllowedInSlot(SLOT_HAND_LEFT))
		{
			pBestLeft = getBestItem(pBestLeft, (*it));
		}
		else if((*it)->isAllowedInSlot(SLOT_TORSO))
		{
			pBestLeft = getBestItem(pBestTorso, (*it));
		}
	}

	//equip the chosen items
	pCharacter->equip(pBestTorso, SLOT_TORSO);
	pCharacter->equip(pBestRight, SLOT_HAND_RIGHT);
	pCharacter->equip(pBestTorso, SLOT_HAND_LEFT);
}
