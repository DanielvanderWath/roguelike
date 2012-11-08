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
public:
	Display(void);
	~Display(void);
	void drawMap(Floor *floor);
	void drawHUD(Character *pc);
	char getAppearance(FloorTile *tile);
	static void output(std::string str);
};

#endif

