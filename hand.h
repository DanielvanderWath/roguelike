#ifndef __HAND_H__
#define __HAND_H__
#include "item.h"

enum HAND_WHICH{
	HAND_LEFT=0,
	HAND_RIGHT,
	};

class Hand : public Item
{
protected:
	bool onehanded;
	bool shield;
public:
	Hand(void);
	virtual ~Hand(void);
	virtual void dumpStats(int indent)=0;
	bool isOneHanded(void);
	bool isShield(void);
};

#endif
