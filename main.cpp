#include "game.h"
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

	Race *tempRace = new Race("Human", 28, 4, 8, 4);

	pCNewCharacter = new Character(pstrName, tempRace, MALE);

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
}

// *** try to move character to the adjacent space in the given direction. return true if it moved, false otherwise ***
bool Game::moveCharacter(Character *c, DIRECTION dir)
{
	FloorTile *old = c->getPosition();
	FloorTile *next = floor->getTile(old, dir);

	//if next is still NULL then we were unable to move
	if(next)
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

// *** allow the character to pick up items chosen through a dialogue presented to the player. Pass a greater-than-zero value in maxAllowed to limit the number of items the player is allowed to take ***
void Game::pickUpItems(Character *c, list<Item*>* items, int maxAllowed)
{
	bool bFinished = false, bUnlimited = maxAllowed > 0 ? false : true;
	list<std::string*> itemNames;
	int selection;

	while(!bFinished)
	{
		//build a list of item names. This is done every iteration, so that the list the player sees is always correct
		for(list<Item*>::iterator it = items->begin(); it != items->end(); it++)
		{
			itemNames.push_back((*it)->getName());
		}

		selection = Display::dialogue(std::string("What would you like to pick up?"), &itemNames);
		if(selection < 0)
		{
			bFinished = true;
		}
		else
		{
			list<Item*>::iterator it = items->begin();
			for(int i = 0; it != items->end(); i++, it++)
			{
				if(i == selection)
				{
					c->pickUp(*it);
					it = items->erase(it);
					break;
				}
			}
		}

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
						while(floor->getTile(pc->getPosition(), dir))
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
			case 'i':
				//go to inventory screen. for now just list inventory
				pc->listInventory();
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

void Game::kill(Character *killer, Character **killed)
{
	//take all of its items
	list<Item*> lstDrop;
	(*killed)->giveInventory(&lstDrop);
	//drop them on the floor
	if(!lstDrop.empty())
	{
		OUTPUT((*killed)->getName() << " drops:" );
		while(!lstDrop.empty())
		{
			OUTPUT("\t" << lstDrop.front()->getName() );
			(*killed)->getPosition()->getInventory()->push_back(lstDrop.front());
			lstDrop.pop_front();
		}
	}

	killer->addXP((*killed)->getXPValue());
	delete *killed;
	(*killed) = NULL;
}

// *** spawn an item on a floor tile ***
void Game::spawnItem(FloorTile *tile)
{
	static int i =0;//TODO REWRITE THIS WHOLE FUNCTION

	switch(i)
	{
		case 0:
			tile->dropItem(new Armour("Lazarus Suit", 4, 3, NULL, NULL, list<int>(1, SLOT_TORSO)));
			break;
		case 1:
			tile->dropItem(new Weapon("Massive sledgehammer", 8, 6, 2, 4, false));
			break;
		case 2:
			tile->dropItem(new Weapon("Assassin's blade", 3, 2, 2, 2, true));
			break;
		case 3:
			tile->dropItem(new Shield("Bronze Aspis", 3, 2, NULL, 0, 5));
			break;
		case 4:
			tile->dropItem(new Weapon("Pointy stick", 2, 2, 2, 2, true));
			break;
		default:
			break;
	}
	i++;
}

Floor* Game::getFloor(void)
{
	return floor;
}

void Game::mainLoop(void)
{
	while(!quit)
	{
		display.drawMap(getFloor());
		display.drawHUD(pc, getFloor()->getWidth());
		doActionFromUser();
		display.setUserInputTrue();
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
