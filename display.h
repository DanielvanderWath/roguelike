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
public:
	Display(void);
	~Display(void);
	static void drawMap(Floor *floor);
	static void drawHUD(Character *pc);
	static char getAppearance(FloorTile *tile);
	static int getBufferSize(void);
	static int getWindowHeight(void);
	static void setBufferSize(int size);
	static void waitForKey(int key);
	static void output(std::string str);
	static void setUserInputTrue(void);
	static void dialogue(std::string str, const char *choices);
};

#endif

