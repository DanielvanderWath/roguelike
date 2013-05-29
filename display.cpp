#include "display.h"

int Display::bufferSize=0;//static, need to allocate storage somewhere
int Display::iWinWidth=0, Display::iWinHeight=0;
bool Display::bUserInputSinceLastMessage = false;
bool Display::bAskingPlayer = false;
bool Display::bMessageFillWindow = true;//if there isn't a floor, the message buffer can fill the whole window
bool Display::bHandlingColour = false;//true when output is expected to handle colour codes

#define NC_COLOR_DEFAULT 	1
#define NC_COLOR_RED 		2
#define NC_COLOR_YELLOW 	3
#define NC_COLOR_GREEN 		4
#define NC_COLOR_BLUE 		5

Display::Display(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	//make sure we support colours
	if(!has_colors())
	{
		cout << "Error: colour not supported" << endl;
	}
	else
	{
		start_color();
		use_default_colors();
		init_pair(NC_COLOR_DEFAULT, -1, -1);
		init_pair(NC_COLOR_RED, COLOR_RED, -1);
		init_pair(NC_COLOR_YELLOW, COLOR_YELLOW, -1);
		init_pair(NC_COLOR_GREEN, COLOR_GREEN, -1);
		init_pair(NC_COLOR_BLUE, COLOR_BLUE, -1);
		attron(COLOR_PAIR(NC_COLOR_DEFAULT));
	}

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

void Display::hudStatLine(std::string strLabel, int iCurrent, int iMax, int y, int x)
{
	move(y, x);

	//print the label
	printw(strLabel.c_str());

	//decide what colour the stats should be
	if(3 * iMax / 4 <= iCurrent)
	{
		attron(COLOR_PAIR(NC_COLOR_GREEN));
	}
	else if(iMax / 3 <= iCurrent)
	{
		attron(COLOR_PAIR(NC_COLOR_YELLOW));
	}
	else
	{
		attron(COLOR_PAIR(NC_COLOR_RED));
	}

	//print the actual stats
	printw("%d/%d", iCurrent, iMax);

	//and finally reset the colours to default
	attron(COLOR_PAIR(NC_COLOR_DEFAULT));
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


	//clear the hud, in case any strings in this frame are shorter than they were last frame
	for(iCursorY = 1; iCursorY < iHeight; iCursorY++)
	{
		move(iCursorY, iLeft);
		clrtoeol();
	}
	iCursorY = 1;

	mvprintw(iCursorY++, iLeft, std::string("Name: " + pc->getName()).substr(0, iWidth).c_str());
	if(iCursorY >= iHeight)
		return;
	hudStatLine("HP: ", pc->getHP(), pc->getHPMax(), iCursorY++, iLeft);
	if(iCursorY >= iHeight)
		return;
	mvprintw(iCursorY++, iLeft, "AV: %d", pc->getAV());
	if(iCursorY >= iHeight)
		return;
	mvprintw(iCursorY++, iLeft, "XP: %d", pc->getXP());
	if(iCursorY >= iHeight)
		return;

	//empty line
	iCursorY++;

	if(pc->getTorso())
	{
		mvprintw(iCursorY++, iLeft, std::string("Amr: " + *pc->getTorso()->getName()).substr(0, iWidth).c_str());
		if(iCursorY >= iHeight)
			return;
	}
	if(pc->getRightHand())
	{
		mvprintw(iCursorY++, iLeft, std::string("RH: " + *pc->getRightHand()->getName()).substr(0, iWidth).c_str());
		if(iCursorY >= iHeight)
			return;
	}
	if(pc->getLeftHand())
	{
		mvprintw(iCursorY++, iLeft, std::string("LH: " + *pc->getLeftHand()->getName()).substr(0, iWidth).c_str());
		if(iCursorY >= iHeight)
			return;
	}
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

void Display::clearMessageBuffer(void)
{
	for(int i = 0; i < bufferSize; i++)
	{
		bUserInputSinceLastMessage = true;
		OUTPUT("");
	}
}

// *** print a string containing colour codes (beginning with ¬) ***
void Display::printw_colour(std::string str)
{
	std::string::iterator it = str.begin();

	for(it; it != str.end(); it++)
	{
		if(*it == '`')
		{
			//get the next character, which will describe the colour
			switch(*(++it))
			{
				case 'R':
					attron(COLOR_PAIR(NC_COLOR_RED));
					break;
				case 'Y':
					attron(COLOR_PAIR(NC_COLOR_YELLOW));
					break;
				case 'G':
					attron(COLOR_PAIR(NC_COLOR_GREEN));
					break;
				case 'B':
					attron(COLOR_PAIR(NC_COLOR_BLUE));
					break;
				case 'D':
					attron(COLOR_PAIR(NC_COLOR_DEFAULT));
					break;
				default:
					//invalid colour code. OUTPUT here might cause problems, so report the error with cout
					cout << "ERROR: invalid colour code ¬" << *it << endl;
					break;
			}
		}
		else
		{
			addch(*it);
		}
	}

	//set the colour back to default
	attron(COLOR_PAIR(NC_COLOR_DEFAULT));
}

void Display::output(std::string str)
{
	int iWidth, iHeight;
	static list<std::string> buffer(bufferSize, "");
	int i = bAskingPlayer ? 1 : 0;
	
	getmaxyx(stdscr, iHeight, iWidth);

	if(bMessageFillWindow)
	{
		bufferSize = iHeight;
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
		//clear line
		move(iHeight-1 - i, 0);
		clrtoeol();

		//ensure cursor is at the start of the line
		move(iHeight-1 - i, 0);

		if(bHandlingColour)
		{
			printw_colour((*it++).c_str());
		}
		else
		{
			printw((*it++).c_str());
		}
	}

	refresh();

	if(!bUserInputSinceLastMessage)
	{
		std::string strPressSpacebar = "(Space)";
		mvprintw(iHeight - 1, iWidth - strPressSpacebar.length(), strPressSpacebar.c_str());

		waitForKey(' ');//spacebar, I can't find it in curses.h for some reason

		move(iHeight - 1, iWidth - strPressSpacebar.length());
		clrtoeol();
	}

	if(bAskingPlayer)
	{
		move(iHeight - 1, 0);
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

// *** Tell output whether or not to parse colour codes. ***
void Display::setUseColour(bool b)
{
	bHandlingColour = b;
}

// *** present the player with a question and a list of potential answers ***
int Display::dialogue(std::string strQuestion, std::list<std::string*> *lChoices)
{
	int iChoice=0, iKey=0, iSelection=0;
	char cZero='a', cChoice=cZero, cMore, cFinish;
	bool bFinished = false;
	std::list<std::string*>::iterator it = lChoices->begin();

#define NUM_FLUFF 3 //this is the number of menu items added here that don't belong to lChoices, + 1

	while(!bFinished)
	{
		//ask the question
		OUTPUT(strQuestion);

		//if we've "seen more" past the end of the list, then wrap around
		if(it == lChoices->end())
		{
			iChoice = 0;
			it = lChoices->begin();
		}

		while(cChoice + NUM_FLUFF - cZero < bufferSize && it != lChoices->end())
		{
			bUserInputSinceLastMessage = true;
			OUTPUT(cChoice++ << ") " << *(*it++));
		}

		//if we haven't reached the end of the list, then we've filled the buffer, so offer to print out another page
		bUserInputSinceLastMessage = true;
		cMore = cChoice - cZero;
		OUTPUT(cChoice++ << ") see more");

		bUserInputSinceLastMessage = true;
		cFinish = cChoice - cZero;
		OUTPUT(cChoice << ") finished");

		//reset cChoice
		cChoice = cZero;

		//get the player's choice, ignore invalid keys
		while(true)
		{
			iKey = getch();	
			iSelection = iKey - cZero;
			if(iSelection < cMore & iSelection >= 0)
			{
				//they chose an option from the list on screen
				iChoice += iSelection;
				bFinished = true;
				break;
			}
			else if(iSelection == cFinish)
			{
				//they've finished
				clearMessageBuffer();
				bFinished = true;
				iChoice = -1;
				break;
			}
			else if(iSelection == cMore)
			{
				//they want to see more
				iChoice+= bufferSize - NUM_FLUFF;
				break;
			}
			else
			{
				//they made an invalid choice
			}
		}
	}
	return iChoice;
}

// *** ask user a question, get a string back ***
std::string* Display::askUserForString(std::string strQuestion)
{
	std::string *pstrAnswer = new std::string;
	int iKey = 'x';
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
		iKey = getch();

		if(iKey == KEY_BACKSPACE && pstrAnswer->length() > 0)
		{
			pstrAnswer->erase(pstrAnswer->length() - 1, 1);
			clrtoeol();
		}
		//only accept alphanumeric characters and spaces
		else if(	(iKey >= 'a' && iKey <= 'z') ||
				(iKey >= 'A' && iKey <= 'Z') ||
				(iKey >= '1' && iKey <= '0') ||
				(iKey == ' ') || (iKey == '\n'))
		{
			if(iKey == '\n')
				break;

			pstrAnswer->push_back(iKey);
		}
		else
		{
			continue;
		}
	}
	noecho();

	//Add the answer to the message buffer
	bUserInputSinceLastMessage = true;
	output(*pstrAnswer);

	return pstrAnswer;
}


