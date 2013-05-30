#ifndef __THING_H__
#define __THING_H__

#include <string>

class Thing
{
	protected:
		char appearance;
	public:
		Thing(void);
		Thing(char a);
		~Thing(void);
		virtual char getAppearance(void);
		void setAppearance(char a);
};

class NamedThing : public Thing
{
	protected:
		std::string name;
	public:
		NamedThing(void);
		~NamedThing(void);
		NamedThing(std::string n);	
		NamedThing(std::string n, char a);	
		std::string* getName(void);
};

#endif
