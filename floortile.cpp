#include <iostream>
#include "floor.h"

FloorTile::FloorTile(void)
{
	occupier = NULL;
}

FloorTile::~FloorTile(void){}

FloorTile::FloorTile(int _x, int _y)
{
	occupier = NULL;
	x = _x;
	y = _y;
}

void FloorTile::dumpInventory(void)
{
	if(!inventory.empty())
	{
		OUTPUT( "The following items are here:");
		for(std::list<Item*>::iterator it = inventory.begin(); it != inventory.end(); it++)
		{
			OUTPUT("\t" << (*it)->getName());
		}
	}
	else
	{
		OUTPUT("There is nothing here");
	}
}

std::list<Item*>* FloorTile::getInventory(void)
{
	return &inventory;
}

void FloorTile::dropItem(Item* item)
{
	inventory.push_back(item);
}

bool FloorTile::occupy(Character* _occupier)
{
	//fail if already occupied
	if(occupier)
		return false;

	occupier = _occupier;
	return true;
}

void FloorTile::leave(void)
{
	occupier = NULL;
}

bool FloorTile::isOccupied(void)
{
	return occupier != NULL;
}

Character* FloorTile::getOccupier(void)
{
	return occupier;
}

int FloorTile::getX(void)
{
	return x;
}

int FloorTile::getY(void)
{
	return y;
}

