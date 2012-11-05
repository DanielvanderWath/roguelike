#ifndef __HAND_H__
#define __HAND_H__
#include "item.h"

enum HAND_WHICH{
	HAND_LEFT,
	HAND_RIGHT,
	};

class Hand : public virtual Item
{
private:
public:
	Hand(void);
	virtual ~Hand(void);
	virtual void dumpStats(int indent)=0;
};

#endif

