#include "display.h"

int Display::bufferSize=0;//static, need to allocate storage somewhere

Display::Display(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	//hide the cursor
	curs_set(0);

	bufferSize = 10;
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

int Display::getBufferSize(void)
{
	return bufferSize;
}

void Display::setBufferSize(int size)
{
	//TODO: if we're reducing the size of the buffer, clear the now empty lines
	bufferSize = size;
}

void Display::waitForKey(int key)
{
	while(getch() != key){}
}

//void Display::output_common

void Display::output(std::string str)
{
	int width, height;
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
	waitForKey(' ');//spacebar, I can't find it in curses.h for some reason
}

void Display::dialogue(std::string str, const char *choices)
{
	OUTPUT(str);
}


