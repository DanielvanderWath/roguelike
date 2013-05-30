#ifndef __ITEM_H__
#define __ITEM_H__
#include <list>
#include <string>
#include "buff.h"
#include "thing.h"

using namespace std;

//these are used to tell the item which slot it is in
#define SLOT_INVENTORY 0
#define SLOT_TORSO 1
#define SLOT_FIRST SLOT_TORSO
#define SLOT_HAND_LEFT 2
#define SLOT_HAND_RIGHT 3

class Item : public NamedThing
{
protected:
	std::string strStats;
	int slot; //slot 0 is the character's inventory. Everything else is character specific
	list<int> allowed_slots;
	list<Buff*> buffs;
public:
	static std::string astrSlotNames[];

	Item(void);
	Item(std::string n, char a);
	virtual ~Item(void);
	virtual void dumpStats(void);
	void addBuff(Buff *_buff);
	int getSlot(void);
	void setSlot(int s);
	bool isAllowedInSlot(int _slot);
	std::string* getName(void);
	virtual std::string* getInvString(void);
};

#endif

