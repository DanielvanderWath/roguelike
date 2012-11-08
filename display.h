#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

class Display
{
protected:
public:
	Display(void);
	~Display(void);
	void drawMap(Floor *floor);
	void drawHUD(Character *pc);
};

#endif

