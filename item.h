#ifndef __ITEM_H__
#define __ITEM_H__

class Item
{
protected:
	char *name;
	char icon;
public:
	Item(void);
	virtual ~Item(void);
	virtual void dumpStats(void);
	
};

#endif

