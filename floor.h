#ifndef __FLOOR_H__
#define __FLOOR_H__

#include <vector>
#include <list>
#include "item.h"
#include "character.h"
#include "ai.h"
#include "misc.h"

class AI;

class FloorTile : public Thing
{
protected:
	std::list<Item*> inventory;
	Character *occupier;
	int x, y;
public:
	FloorTile(void);
	~FloorTile(void);
	FloorTile(int _x, int _y);
	void dumpInventory(void);
	std::list<Item*>* getInventory(void);
	void dropItem(Item *item);
	bool occupy(Character* _occupier);
	void leave(void);
	bool isOccupied(void);
	bool isAdjacent(FloorTile *pOther);
	Character* getOccupier(void);
	char getAppearance(void);
	int getX(void);
	int getY(void);
};

class Floor
{
protected:
	std::vector<FloorTile*> tiles;
	int width, height;
	std::list<AI*> AIs;
public:
	Floor(void);
	~Floor(void);
	Floor(int w, int h);
	void generate();
	void setDim(int w, int h);
	int getWidth(void);
	int getHeight(void);
	int getRight(void);
	int getBottom(void);
	void addAI(AI *newAI);
	std::list<AI*> *getAIs(void);
	FloorTile* getTile(int x, int y);
	FloorTile* getTile(FloorTile *tile, DIRECTION dir, int iDistance);
};

#endif

