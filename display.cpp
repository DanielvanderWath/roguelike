#include "display.h"

int Display::bufferSize=0;//static, need to allocate storage somewhere
int Display::iWinWidth=0, Display::iWinHeight=0;
bool Display::bUserInputSinceLastMessage = false;
bool Display::bAskingPlayer = false;
bool Display::bMessageFillWindow = true;//if there isn't a floor, the message buffer can fill the whole window

Display::Display(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	//hide the cursor
	curs_set(0);

	//initialise to zero, game class is responsible for calling setBufferSize
	bufferSize = 0;

	getmaxyx(stdscr, iWinHeight, iWinWidth);
}

Display::~Display(void){endwin();}
void Display::drawMap(Floor *floor)
{
	int winWidth, winHeight, floorWidth, floorHeight;

	getmaxyx(stdscr, winHeight, winWidth);

	floorWidth = floor->getWidth();
	floorHeight = floor->getHeight();

	setBufferSize(winHeight - floorHeight);
	bMessageFillWindow = false;

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

#define HUDLINE(Y, X, H, T) \
	mvprintw(Y, X, T); \
	if(++Y >= H) \
	{ \
		return; \
	} 

// *** draw the pc's HUD ***
void Display::drawHUD(Character *pc, int iLeft)
{
	int iWidth, iHeight, iCursorY;

	getmaxyx(stdscr, iHeight, iWidth);

	iWidth = iWidth - iLeft;

	if(iWidth < 1)
	{
		OUTPUT("ERROR: trying to draw HUD outside of window" << endl);
		return;
	}

	iCursorY = 1;

	mvprintw(iCursorY++, iLeft, "Name: %s", pc->getName().c_str());
	if(iCursorY >= iHeight)
		return;
	mvprintw(iCursorY++, iLeft, "HP: %d/%d", pc->getHP(), pc->getHPMax());
	if(iCursorY >= iHeight)
		return;
	mvprintw(iCursorY++, iLeft, "AV: %d", pc->getAV());
	if(iCursorY >= iHeight)
		return;
	mvprintw(iCursorY++, iLeft, "XP: %d", pc->getXP());
	if(iCursorY >= iHeight)
		return;
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

// *** Return buffer size ***
int Display::getBufferSize(void)
{
	return bufferSize;
}

// *** Return height ***
int Display::getWindowHeight(void)
{
	return iWinHeight;
}

// *** Set the size of the message output buffer ***
void Display::setBufferSize(int iSize)
{
	//return early if we don't need to do anything
	if(iSize == bufferSize)
	{
		return;
	}

	if(iSize < 1)
	{
		OUTPUT("TRYING TO SET OUTPUT BUFFER SIZE TO LESS THAN 1");
		return;
	}

	/*if(iSize < bufferSize)
	{
		int i;
		for(i = height - bufferSize - 1; i < iSize; i++)
		{
			move(i, 0);
			clrtoeol();
		}
	}*/

	bufferSize = iSize;
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
	int i = bAskingPlayer ? 1 : 0;
	
	getmaxyx(stdscr, height, width);

	if(bMessageFillWindow)
	{
		bufferSize = height;
	}

	if(bufferSize == 0)
	{
		cout << "Calling " << __func__ << " before initialising buffer size" << endl;
		return;
	}

	if(bufferSize != buffer.size())
	{
		buffer.resize(bufferSize);
	}

	buffer.pop_back();
	buffer.push_front(std::string(str));

	list<std::string>::iterator it=buffer.begin();
	for(i; i < buffer.size(); i++)
	{
		move(height-1 - i, 0);
		clrtoeol();
		mvprintw(height-1 - i, 0, (*it++).c_str());
	}

	refresh();

	if(!bUserInputSinceLastMessage)
	{
		waitForKey(' ');//spacebar, I can't find it in curses.h for some reason
	}

	if(bAskingPlayer)
	{
		move(height - 1, 0);
		clrtoeol();
		bAskingPlayer = false;
	}

	bUserInputSinceLastMessage = false;
}

void Display::setUserInputTrue(void)
{
	bUserInputSinceLastMessage = true;
}

void Display::setUserInputFalse(void)
{
	bUserInputSinceLastMessage = false;
}

void Display::dialogue(std::string str, const char *choices)
{
	OUTPUT(str);
}

// *** ask user a question, get a string back ***
std::string* Display::askUserForString(std::string strQuestion)
{
	std::string *pstrAnswer = new std::string;
	char cKey = 'x';
	int width, height;

	//make sure output won't wait for a keypress before returning
	bUserInputSinceLastMessage = true;
	bAskingPlayer = true;

	//print out the question
	output(strQuestion);

	//put the cursor at the end of the question. output prints the message log from bottom to top, and we want to be at the bottom
	getmaxyx(stdscr, height, width);

	move(height - 1, 0);

	//get the player's response. Turn on echoing
	echo();
	while(true)
	{
		cKey = getch();

		if(cKey == '\n')
			break;

		pstrAnswer->push_back(cKey);
	}
	noecho();

	//Add the answer to the message buffer
	bUserInputSinceLastMessage = true;
	output(*pstrAnswer);

	return pstrAnswer;
}


