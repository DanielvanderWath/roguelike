#ifndef __FLOOR_H__
#define __FLOOR_H__

#include <vector>
#include <list>
#include "item.h"
#include "character.h"

class FloorTile
{
protected:
	std::list<Item*> inventory;
	Character *occupier;
public:
	FloorTile(void);
	~FloorTile(void);
	void dumpInventory(void);
	std::list<Item*>* getInventory(void);
	bool occupy(Character* _occupier);
	void leave(void);
	bool isOccupied(void);
	Character* getOccupier(void);
};

class Floor
{
protected:
	std::vector<FloorTile*> tiles;
	int width, height;
public:
	Floor(void);
	~Floor(void);
	Floor(int w, int h);
	void generate();
	void setDim(int w, int h);
	int getWidth(void);
	int getHeight(void);
	FloorTile* getTile(int x, int y);
};

#endif

