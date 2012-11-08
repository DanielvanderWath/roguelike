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
	tiles.resize(width*height);
	for(vector<FloorTile*>::iterator it = tiles.begin(); it != tiles.end(); it++)
	{
		(*it) = new FloorTile();
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

FloorTile* Floor::getTile(int x, int y)
{
	if(x < width && y < height)
		return tiles[x*height + width];
	OUTPUT("ERROR: tile " << x << "," << y << " is out of bounds" );
	return NULL;
}

