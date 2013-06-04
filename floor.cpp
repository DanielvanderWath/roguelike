#include <iostream>
#include "floor.h"
#include "display.h"

Floor::Floor(void){}
Floor::~Floor(void){}

Floor::Floor(int w, int h)
{
	width = w;
	height = h;
}

void Floor::generate()
{
	int x=0, y=0;
	tiles.resize(width*height);
	for(vector<FloorTile*>::iterator it = tiles.begin(); it != tiles.end(); it++)
	{
		(*it) = new FloorTile(x, y);

		if(++x==width)
		{
			x = 0;
			y++;
		}
	}
}

void Floor::setDim(int w, int h)
{
	width = w;
	height = h;
}

int Floor::getWidth(void)
{
	return width;
}

int Floor::getHeight(void)
{
	return height;
}

int Floor::getRight(void)
{
	return width - 1;
}

int Floor::getBottom(void)
{
	return height - 1;
}

// *** add a new AI ***
void Floor::addAI(AI *newAI)
{
	//do nothing if newAI is NULL
	if(newAI)
	{
		AIs.push_back(newAI);
	}
}

// *** return a pointer to the list of AIs
std::list<AI*>* Floor::getAIs(void)
{
	return &AIs;
}

// *** return tile at given coordinates ***
FloorTile* Floor::getTile(int x, int y)
{
	if(x >= 0 && y >= 0 && x < width && y < height)
		return tiles[y*width + x];
	return NULL;
}

// *** return adjacent tile in given direction, or NULL if there isn't one (i.e. at the edge of the floor ***
FloorTile* Floor::getTile(FloorTile *tile, DIRECTION dir, int iDistance)
{
	FloorTile *ret = NULL;
	switch(dir)
	{
		case DIRECTION_WEST:
			if(tile->getX() != 0)
			{
				ret = getTile(tile->getX() - iDistance, tile->getY());
				//if it would be out of bounds, get the one adjacent to the wall instead
				if(!ret)
					ret = getTile(0, tile->getY());
			}
			break;
		case DIRECTION_SOUTH:
			if(tile->getY() != getBottom())
			{
				ret = getTile(tile->getX(), tile->getY() + iDistance);
				//if it would be out of bounds, get the one adjacent to the wall instead
				if(!ret)
					ret = getTile(tile->getX(), getBottom());
			}
			break;
		case DIRECTION_NORTH:
			if(tile->getY() != 0)
			{
				ret = getTile(tile->getX(), tile->getY() - iDistance);
				//if it would be out of bounds, get the one adjacent to the wall instead
				if(!ret)
					ret = getTile(tile->getX(), 0);
			}
			break;
		case DIRECTION_EAST:
			if(tile->getX() != getRight())
			{
				ret = getTile(tile->getX() + iDistance, tile->getY());
				//if it would be out of bounds, get the one adjacent to the wall instead
				if(!ret)
					ret = getTile(getRight(), tile->getY());
			}
			break;
		default:
			OUTPUT("ERROR: trying to get tile in invalid direction" << endl);
	}

	return ret;
}

