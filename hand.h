#ifndef __HAND_H__
#define __HAND_H__
#include "item.h"

class Item;

enum HAND_WHICH{
	HAND_LEFT=0,
	HAND_RIGHT,
	};

class Hand : public virtual Item
{
protected:
	bool onehanded;
	bool shield;
	bool weapon;
	Character *wielder;
public:
	Hand(void);
	virtual ~Hand(void);
	virtual void dumpStats(int indent);
	void setWielder(Character *w);
	bool isOneHanded(void);
	bool isShield(void);
	bool isWeapon(void);
};

#endif

