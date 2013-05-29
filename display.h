#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>
#include "floor.h"
#include "character.h"

class Floor;
class FloorTile;

class Display
{
protected:
	static int bufferSize;
	static int iWinWidth, iWinHeight;
	static bool bUserInputSinceLastMessage;
	static bool bAskingPlayer;
	static bool bMessageFillWindow;
	static bool bHandlingColour;
public:
	Display(void);
	~Display(void);
	static void drawMap(Floor *floor);
	static void hudStatLine(std::string strLabel, int iCurrent, int iMax, int y, int x);
	static void drawHUD(Character *pc, int iLeft);
	static char getAppearance(FloorTile *tile);
	static int getBufferSize(void);
	static int getWindowHeight(void);
	static void setBufferSize(int size);
	static void waitForKey(int key);
	static void clearMessageBuffer(void);
	static void printw_colour(std::string str);
	static void output(std::string str);
	static void setUserInputTrue(void);
	static void setUserInputFalse(void);
	static void setUseColour(bool b);
	static int dialogue(std::string str, std::list<std::string*> *lChoices);
	static std::string* askUserForString(std::string strQuestion);
};

#endif

