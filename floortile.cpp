#include <iostream>
#include "floor.h"
#include "display.h"

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

// *** return true if this tile is adjacent to the one pointed to by pOther. Deliberately ignoring diagonals becuase characters can only move in four directions ***
bool FloorTile::isAdjacent(FloorTile *pOther)
{
	if(!pOther)
		return false;

	if(pOther->getX() == x)
		if(pOther->getY() - y == 1 || pOther->getY() - y == -1)
			return true;
	if(pOther->getY() == y)
		if(pOther->getX() - x == 1 || pOther->getX() - x == -1)
			return true;

	return false;
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

