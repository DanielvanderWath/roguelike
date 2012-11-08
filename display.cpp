#include "display.h"

Display::Display(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}

Display::~Display(void){endwin();}
void Display::drawMap(Floor *floor)
{
	int winWidth, winHeight, floorWidth, floorHeight;
	getmaxyx(stdscr, winHeight, winWidth);
	floorWidth = floor->getWidth();
	floorHeight = floor->getHeight();
	if( floorWidth <= winWidth && floorHeight <= winHeight)
	{
		for(int x = 0; x < floorWidth; x++)
		{
			for(int y = 0; y < floorHeight; y++)
			{
				mvaddch(y, x, getAppearance(floor->getTile(x, y)));
			}
		}
		refresh();
	}
	else
	{
		addstr("Window is too small to draw map");
	}

}

void Display::drawHUD(Character *pc)
{

}

char Display::getAppearance(FloorTile *tile)
{
	//any character on this tile takes priority when drawing
	if(tile->isOccupied())	
		return tile->getOccupier()->getAppearance();
	//then the item on top of the pile, if there is one
	if(!tile->getInventory()->empty())
		return tile->getInventory()->front()->getAppearance();

	//otherwise just return the tile's icon, . for now
	return '.';
}

void Display::output(std::string str)
{
	int width, height;
	int bufferSize = 5;
	static list<std::string> buffer(bufferSize, "");

	buffer.pop_back();
	buffer.push_front(std::string(str));
	
	getmaxyx(stdscr, height, width);

	list<std::string>::iterator it=buffer.begin();
	for(int i = 0; i < buffer.size(); i++)
	{
		move(height-1 - i, 0);
		clrtoeol();
		mvprintw(height-1 - i, 0, (*it++).c_str());
	}

	refresh();
	getch();
}
