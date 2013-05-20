#include <iostream>
#include "floor.h"

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

// *** return tile at given coordinates ***
FloorTile* Floor::getTile(int x, int y)
{
	if(x < width && y < height)
		return tiles[y*width + x];
	OUTPUT("ERROR: tile " << x << "," << y << " is out of bounds" );
	return NULL;
}

// *** return adjacent tile in given direction, or NULL if there isn't one (i.e. at the edge of the floor ***
FloorTile* Floor::getTile(FloorTile *tile, DIRECTION dir)
{
	FloorTile *ret = NULL;
	switch(dir)
	{
		case DIRECTION_WEST:
			if(tile->getX() != 0)
				ret = getTile(tile->getX() - 1, tile->getY());
			break;
		case DIRECTION_SOUTH:
			if(tile->getY() != getBottom())
				ret = getTile(tile->getX(), tile->getY() + 1);
			break;
		case DIRECTION_NORTH:
			if(tile->getY() != 0)
				ret = getTile(tile->getX(), tile->getY() - 1);
			break;
		case DIRECTION_EAST:
			if(tile->getX() != getRight())
				ret = getTile(tile->getX() + 1, tile->getY());
			break;
		default:
			OUTPUT("ERROR: trying to get tile in invalid direction" << endl);
	}

	return ret;
}

