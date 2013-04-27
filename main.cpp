#include "game.h"
#include <ctime>
#include <cstdlib>

#define MAP_WIDTH 60
#define MAP_HEIGHT 30

enum DIRECTION
{
	DIRECTION_NORTH,
	DIRECTION_EAST,
	DIRECTION_SOUTH,
	DIRECTION_WEST,
};

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
	void doActionFromUser(void);
	void kill(Character *killer, Character **killed);
	void moveCharacter(Character *c, DIRECTION dir);
	Floor *getFloor(void);
	void mainLoop(void);
};

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

void Game::moveCharacter(Character *c, DIRECTION dir)
{
	FloorTile *old = c->getPosition();
	FloorTile *next = NULL;

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

	//if next is still NULL then we were unable to move
	if(next)
		c->moveTo(next);
}

void Game::doActionFromUser(void)
{
	int key = getch();
	switch(key)
	{
		case 'q':
			quit = true;
			break;
		case KEY_LEFT:
		case 'h':
		{
			moveCharacter(pc, DIRECTION_WEST);
			break;
		}
		case KEY_DOWN:
		case 'j':
		{
			moveCharacter(pc, DIRECTION_SOUTH);
			break;
		}
		case KEY_UP:
		case 'k':
		{
			moveCharacter(pc, DIRECTION_NORTH);
			break;
		}
		case KEY_RIGHT:
		case 'l':
		{
			moveCharacter(pc, DIRECTION_EAST);
			break;
		}
		default:
			break;//do nothing on unrecognised key
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
