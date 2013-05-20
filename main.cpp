#include "game.h"
#include <ctime>
#include <cstdlib>

#define MAP_WIDTH 60
#define MAP_HEIGHT 30

Character* Game::createPlayer(void)
{
	OUTPUT("Welcome traveller!");
	Race *tempRace = new Race("Temp", 28, 4, 8, 4);
	return new Character("Temp", tempRace, MALE);
}

void Game::init(void)
{
	//seed random
	srand(time(0));

	//create a map
	floor = new Floor(MAP_WIDTH, MAP_HEIGHT);
	floor->generate();

	display.setBufferSize(display.getWindowHeight() - MAP_HEIGHT);

	//temp race
	pc = createPlayer();
	pc->moveTo(floor->getTile(MAP_WIDTH/2, MAP_HEIGHT/2));
}

// *** try to move character to the adjacent space in the given direction. return true if it moved, false otherwise ***
bool Game::moveCharacter(Character *c, DIRECTION dir)
{
	FloorTile *old = c->getPosition();
	FloorTile *next = floor->getTile(old, dir);

#if 0
	switch(dir)
	{
		case DIRECTION_WEST:
		{
			//if we're at the left wall
			if(old->getX() == 0)
				pc->bump();
			else
				next = floor->getTile(old->getX() - 1, old->getY());
			break;
		}
		case DIRECTION_NORTH:
		{
			//if we're at the top wall
			if(old->getY() == 0)
				pc->bump();
			else
				next = floor->getTile(old->getX(), old->getY() - 1);
			break;
		}
		case DIRECTION_SOUTH:
		{
			//if we're at the bottom wall
			if(old->getY() == floor->getBottom())
				pc->bump();
			else
				next = floor->getTile(old->getX(), old->getY() + 1);
			break;
		}
		case DIRECTION_EAST:
		{
			//if we're at the right-hand wall
			if(old->getX() == floor->getRight())
				pc->bump();
			else
				next = floor->getTile(old->getX() + 1, old->getY());
			break;
		}
		default:
			OUTPUT("INVALID DIRECTION SPECIFIED 0x" << hex << dir << endl);
	}
#endif

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

Floor* Game::getFloor(void)
{
	return floor;
}

void Game::mainLoop(void)
{
	while(!quit)
	{
		display.drawMap(getFloor());
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
