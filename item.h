#ifndef __ITEM_H__
#define __ITEM_H__

class Item
{
protected:
	char *name;
	char icon;
	int slot; //slot 0 is the character's inventory. Everything else is character specific
public:
	Item(void);
	virtual ~Item(void);
	virtual void dumpStats(void);
	int getSlot(void);
	void setSlot(int s);
	const char* getName(void);
};

#endif

