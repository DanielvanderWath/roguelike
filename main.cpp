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
public:
	void init(void);
	void doActionFromUser(void);
	void kill(Character *killer, Character **killed);
	void moveCharacter(Character *c, DIRECTION dir);
	Floor *getFloor(void);
};

void Game::init(void)
{
	//seed random
	srand(time(0));

	//create a map
	floor = new Floor(MAP_WIDTH, MAP_HEIGHT);
	floor->generate();

	//temp race
	Race *tempRace = new Race("Temp", 28, 4, 8, 4);
	pc = new Character("Temp", tempRace, MALE);
	pc->moveTo(floor->getTile(MAP_WIDTH/2, MAP_HEIGHT/2));
}

void Game::moveCharacter(Character *c, DIRECTION dir)
{
	FloorTile *old = c->getPosition();
	//FloorTile *next = floor->getTile(c->
}

void Game::doActionFromUser(void)
{
	int key = getch();
	switch(key)
	{
		case 'q':
			quit = true;
			break;
		case KEY_UP:
		case 'h':
			moveCharacter(pc, DIRECTION_NORTH);
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

void Game::getFloor(void)
{
	return floor;
}

int main(int argc, char **argv)
{

	//Drawing class
	Display display;

	//initialise
	Game game;
	game.init();

	bool quit = false;
	while(!quit)
	{
		display.drawMap(game.getFloor());
		game.doActionFromUser();
	}

	return 0;
}
