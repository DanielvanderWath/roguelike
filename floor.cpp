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

FloorTile* Floor::getTile(int x, int y)
{
	if(x < width && y < height)
		return tiles[y*width + x];
	OUTPUT("ERROR: tile " << x << "," << y << " is out of bounds" );
	return NULL;
}

