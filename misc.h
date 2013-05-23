#ifndef __MISC_H__
#define __MISC_H__

enum DIRECTION
{
	DIRECTION_NORTH,
	DIRECTION_EAST,
	DIRECTION_SOUTH,
	DIRECTION_WEST,
	DIRECTION_INVALID,
};

//TODO: make this safe
#define INDENTER(X, Y)	char *Y = new char[X+1]; \
			memset(Y, '\t', X); \
			Y[X] = 0;//NULL terminate

//#define OUTPUT(X) Display::output( (std::ostringstream str << "---" << X).str() )
#define OUTPUT(X) \
		{ std::ostringstream str ; \
		str << "---" << X ; \
		Display::output(str.str() );}

#define OUTPUTI(X, I) \
		{ std::ostringstream str; \
		for(int i = 0; i < I ; i++) \
			str << "---" ; \
		str << X ; \
		Display::output(str.str() );}

#define ASK(X) \
		Display::askUserForString(X)

#define DIALOGUE(X, Y) \
		Display::dialogue(std::string(X), &Y)

#define CLAMP(low, X, high) (low > X ? low : high < X ? high : X)


#endif
