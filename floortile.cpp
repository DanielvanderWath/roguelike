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
	return occupier != NULL || !inventory.empty();
}

Character* FloorTile::getOccupier(void)
{
	return occupier;
}

// *** get the appearance of a tile. It will be the character there if there is one, otherwise the last item dropped here ***
char FloorTile::getAppearance(void)
{
	if(occupier)
	{
		return occupier->getAppearance();
	}
	else if(!inventory.empty())
	{
		return inventory.back()->getAppearance();
	}
	else
	{
		return appearance;
	}
}

int FloorTile::getX(void)
{
	return x;
}

int FloorTile::getY(void)
{
	return y;
}

