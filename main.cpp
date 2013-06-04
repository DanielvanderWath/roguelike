#include "game.h"
#include "shield.h"
#include <ctime>
#include <cstdlib>

#define MAP_WIDTH 60
#define MAP_HEIGHT 17

Character* Game::createPlayer(void)
{
	Character *pCNewCharacter;
	std::string *pstrName;

	OUTPUT("Welcome traveller!");
	pstrName = ASK("What is your name?");

	Race *tempRace = new Race("Human", 8, 4, 8, 4);

	pCNewCharacter = new Character(*pstrName, tempRace, MALE);

	//this lives on the heap
	delete pstrName;

	return pCNewCharacter;
}

void Game::init(void)
{
	//seed random
	srand(time(0));

	pc = createPlayer();

	//create a map
	floor = new Floor(MAP_WIDTH, MAP_HEIGHT);
	floor->generate();

	display.setBufferSize(display.getWindowHeight() - MAP_HEIGHT);

	pc->moveTo(floor->getTile(MAP_WIDTH/2, MAP_HEIGHT/2));

	//put some items on the floor. TODO: get rid of this
	spawnItem(floor->getTile(MAP_WIDTH/2 + 2, MAP_HEIGHT/2));
	spawnItem(floor->getTile(MAP_WIDTH/2 + 2, MAP_HEIGHT/2));
	spawnItem(floor->getTile(MAP_WIDTH/2 + 2, MAP_HEIGHT/2));
	spawnItem(floor->getTile(MAP_WIDTH/2 + 2, MAP_HEIGHT/2));
	spawnItem(floor->getTile(MAP_WIDTH/2 + 2, MAP_HEIGHT/2));

	//spawn a monster
	spawnMonster(floor->getTile(MAP_WIDTH/2 - 2, MAP_HEIGHT/2));
	spawnItem(floor->getTile(MAP_WIDTH/2 - 3, MAP_HEIGHT/2));
}

// *** have one character attack another, then handle the victim's death if need be ***
void Game::attack(Character *pAttacker, Character *pVictim)
{
	pAttacker->attackBasic(pVictim);
	if(pVictim->isDead())
	{
		kill(pAttacker, pVictim);
	}
}

// *** try to move character to the adjacent space in the given direction. return true if it moved, false otherwise ***
bool Game::moveCharacter(Character *c, DIRECTION dir)
{
	FloorTile *old = c->getPosition();
	FloorTile *next = floor->getTile(old, dir, 1);

	//If there is a character on the tile, then attack it
	if(next && next->getOccupier())
	{
		Character *pTarget = next->getOccupier();
		attack(c, pTarget);
		return false;
	}
	//if next is still NULL then we were unable to move
	else if(next)
	{
		c->moveTo(next);
		return true;
	}
	else
	{
		pc->bump();
		return false;
	}
}

// *** move character towards the target locaton. returns true if the character actually moved ***
bool Game::moveCharacterTowards(Character *c, FloorTile *pTile)
{
	bool bSuccess = false;

	if(c && pTile)
	{
		FloorTile *pCharTile = c->getPosition();

		//return false if the character is already at their destination
		if(pTile == pCharTile)
			return false;

		//work out the x and y distances
		int iXDistance = pTile->getX() - pCharTile->getX();
		int iYDistance = pTile->getY() - pCharTile->getY();

		//move in the direction of the longest one. If that fails, try the shortest
		//TODO: refine this, its too copy-pasted now
		if(abs(iXDistance) > abs(iYDistance))
		{
			if(iXDistance > 0)
				bSuccess = moveCharacter(c, DIRECTION_EAST);
			else
				bSuccess = moveCharacter(c, DIRECTION_WEST);

			if(bSuccess)
			{
				return true;
			}

			//we failed to move the character so try the other axis
			if(iYDistance > 0)
				bSuccess = moveCharacter(c, DIRECTION_SOUTH);
			else
				bSuccess = moveCharacter(c, DIRECTION_NORTH);
		}
		else
		{
			if(iYDistance > 0)
				bSuccess = moveCharacter(c, DIRECTION_SOUTH);
			else
				bSuccess = moveCharacter(c, DIRECTION_NORTH);

			if(bSuccess)
			{
				return true;
			}

			//we failed to move the character so try the other axis
			if(iXDistance > 0)
				bSuccess = moveCharacter(c, DIRECTION_EAST);
			else
				bSuccess = moveCharacter(c, DIRECTION_WEST);
		}
	}
	return bSuccess;
}

// *** allow the character to pick up items chosen through a dialogue presented to the player. Pass a greater-than-zero value in maxAllowed to limit the number of items the player is allowed to take ***
void Game::pickUpItems(Character *c, list<Item*>* items, int maxAllowed)
{
	bool bFinished = false, bUnlimited = maxAllowed > 0 ? false : true;
	list<std::string*> itemNames;
	int iSelection, iCount;

	while(!bFinished)
	{
		if(items->empty())
		{
			bFinished=true;
			OUTPUT("There's nothing here to take");
			continue;
		}

		//build a list of item names. This is done every iteration, so that the list the player sees is always correct
		for(list<Item*>::iterator it = items->begin(); it != items->end(); it++)
		{
			itemNames.push_back((*it)->getInvString());
		}

		display.setUseColour(true);
		iSelection = DIALOGUE("What would you like to pick up?", itemNames);
		if(iSelection < 0)
		{
			bFinished = true;
		}
		else
		{
			list<Item*>::iterator it = items->begin();
			for(int i = 0; it != items->end(); i++, it++)
			{
				if(i == iSelection)
				{
					c->pickUp(*it);
					it = items->erase(it);

					if(!bUnlimited && ++iCount == maxAllowed)
						bFinished=true;

					if(items->empty())
					{
						bFinished=true;
						OUTPUT("There's nothing left");
					}

					break;
				}
			}
		}
		display.setUseColour(false);

		//clear the list so it can be refilled without any removed items on the next iteration
		itemNames.clear();
	}
}

DIRECTION Game::getDirectionFromKey(int key)
{
	switch(key)
	{
		case KEY_LEFT:
		case 'h':
			return DIRECTION_WEST;
		case KEY_DOWN:
		case 'j':
			return DIRECTION_SOUTH;
		case KEY_UP:
		case 'k':
			return DIRECTION_NORTH;
		case KEY_RIGHT:
		case 'l':
			return DIRECTION_EAST;
		default:
			return DIRECTION_INVALID;
	}
}

// *** allow the character to equip items from their inventory, chosen through a dialogue presented to the player. ***
void Game::equipItemFromInventory(void)
{
	list<std::string*> slotNames, itemNames;
	list<Item*> items;
	int iSlot, iItem;
	bool bFinished = false;

	//create a list of pointers to the slot names. probably move this to Character at some point
	slotNames.push_back(&Item::astrSlotNames[SLOT_TORSO]);
	slotNames.push_back(&Item::astrSlotNames[SLOT_HAND_LEFT]);
	slotNames.push_back(&Item::astrSlotNames[SLOT_HAND_RIGHT]);

	while(!bFinished)
	{
		iSlot = DIALOGUE("Where would you like to equip an item?", slotNames) + SLOT_FIRST;

		if(iSlot < 0)
		{
			//they've changed their mind
			bFinished = true;
		}
		else
		{
			for(list<Item*>::iterator it = pc->getInventory()->begin(); it != pc->getInventory()->end(); it++)
			{
				//create a list of items allowed in this slot
				if((*it)->isAllowedInSlot(iSlot))
				{
					itemNames.push_back((*it)->getInvString());
					items.push_back((*it));
				}
			}

			//if the player has nothing to equip, tell them and exit
			if(itemNames.empty())
			{
				OUTPUT("You don't have anything to put there");
				break;
			}

			//if they do, present them with the choices
			display.setUseColour(true);
			iItem = DIALOGUE("What do you want to equip?", itemNames);

			if(iItem < 0)
			{
				//unequip the item
				pc->unequip(iSlot);
			}
			else
			{
				//equip the item they chose
				list<Item*>::iterator it = items.begin();
				for(int i = 0; it != items.end(); i++, it++)
				{
					if(i == iItem)
					{
						pc->equip(*it, iSlot);
						break;
					}
				}
				
			}
			bFinished = true;

			//turning colour off before equipping the item will display colour codes during the "player picked up item" message
			display.setUseColour(false);
		}
	}
}

void Game::doActionFromUser(void)
{
	int key = getch();
	DIRECTION dir = getDirectionFromKey(key);

	if(dir != DIRECTION_INVALID)
	{
		moveCharacter(pc, dir);
	}
	else
	{
		switch(key)
		{
			case 'q':
				quit = true;
				break;
			case 'w':
				//extended walk command. repeatedly walk until we're about to collide with something
				key = getch();
				dir = getDirectionFromKey(key);
				if(dir != DIRECTION_INVALID)
				{
					if( 	key == KEY_LEFT || key == 'h' ||
						key == KEY_DOWN || key == 'j' ||
						key == KEY_UP 	|| key == 'k' ||
						key == KEY_LEFT || key == 'l')
					{
						FloorTile* tmp;
						while((tmp = floor->getTile(pc->getPosition(), dir, 1)) && !tmp->isOccupied())
						{
							moveCharacter(pc, dir);
						}
					}
				}
				else
				{
					OUTPUT(key << "is not a valid argument for extended walk" << endl);
				}
				break;
			case 'e':
				//equip an item from the player's inventory
				equipItemFromInventory();
				break;
			case 'i':
				//go to inventory screen. for now just list inventory
				display.setUseColour(true);
				pc->listInventory();
				display.setUseColour(false);
				break;
			case ';':
				//pick up items from floor
				if(pc->getPosition())
				{
					pickUpItems(pc, pc->getPosition()->getInventory(), 0);
				}
				else
				{
					//if the pc isn't standing on a tile for some reason fail silently
				}
				break;
			default:
				break;//do nothing on unrecognised key
		}
	}
}

void Game::kill(Character *killer, Character *killed)
{
	//take all of its items
	list<Item*> lstDrop;

	OUTPUT(*killed->getName() << " is killed!");

	killed->giveInventory(&lstDrop);
	//drop them on the floor
	if(!lstDrop.empty())
	{
		OUTPUT(*killed->getName() << " drops:" );
		while(!lstDrop.empty())
		{
			OUTPUT("\t" << *lstDrop.front()->getName() );
			killed->getPosition()->getInventory()->push_back(lstDrop.front());
			lstDrop.pop_front();
		}
	}

	//unoccupy the tile
	killed->getPosition()->leave();
	
	killer->addXP(killed->getXPValue());

	if(killed == pc)
	{
		OUTPUT("GAME OVER :(");
		quit = true;
	}

	//don't delete the character here, leave that to the AI's destructor
}

// *** spawn an item on a floor tile ***
void Game::spawnItem(FloorTile *tile)
{
	static int i =0;//TODO REWRITE THIS WHOLE FUNCTION
	Weapon* tmp;

	switch(i)
	{
		case 0:
			tile->dropItem(new Armour("Lazarus Suit", 4, 3, new Resistance(5, 5, 5, 5), NULL, list<int>(1, SLOT_TORSO)));
			break;
		case 1:
			tile->dropItem(new Weapon("Massive sledgehammer", 8, 6, 2, 4, false));
			break;
		case 2:
			tmp = new Weapon("Assassin's blade", 3, 2, 2, 2, true);
			tmp->addEffect(new Effect("Red Viper's Venom", WEAPON_EFFECT_POISON, 3, 5));
			tile->dropItem(tmp);
			break;
		case 3:
			tile->dropItem(new Shield("Bronze Aspis", 3, 2, NULL, 0, 5));
			break;
		case 4:
			tile->dropItem(new Weapon("Pointy stick", 2, 2, 2, 2, true));
			break;
		case 5:
			tile->dropItem(new Weapon("Rotten club", 3, 5, 3, 3, true));
		default:
			break;
	}
	i++;
}

// *** spawn a monster on a floor tile ***
void Game::spawnMonster(FloorTile *tile)
{
	static int i =0;//TODO REWRITE THIS WHOLE FUNCTION
	Character* tmp = NULL;
	AI *ai = NULL;

	switch(i)
	{
		case 0:
			tmp = new Character("Gollum", new Race("Corrupted Hobbit", 1, 1, 1, 1), MALE);
			tmp->moveTo(tile);
			break;
		default:
			break;
	}
	i++;

	if(tmp)
	{
			//create an AI to control the character
			ai = new AI(tmp);
			floor->addAI(ai);
	}
}

Floor* Game::getFloor(void)
{
	return floor;
}

void Game::tickAI(void)
{
	std::list<AI*> *AIs = floor->getAIs();
	for(std::list<AI*>::iterator it = AIs->begin(); it != AIs->end(); it++)
	{
		//delete this AI if its character is dead
		if((*it)->getCharacter()->isDead())
		{
			delete (*it);
			it = AIs->erase(it);

			//if this was the last one in the list then finish the loop before trying to tick the end
			if(it == AIs->end())
				continue;
		}
		(*it)->tick(this);
	}
}

void Game::mainLoop(void)
{
	while(!quit)
	{
		//draw screen
		display.drawMap(getFloor());
		display.drawHUD(pc, getFloor()->getWidth());

		//take user action
		doActionFromUser();
		display.setUserInputTrue();

		//move NPCs
		tickAI();
	}
}

int main(int argc, char **argv)
{


	//initialise
	Game game;
	game.init();

	game.mainLoop();

	return 0;
}
