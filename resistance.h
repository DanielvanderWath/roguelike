#ifndef __RESISTANCE_H__
#define __RESISTANCE_H__

class Resistance
{
protected:
	int fire, ice, lightning, poison;
public:
	Resistance(void);
	~Resistance(void);
	Resistance(const Resistance *r);
	Resistance(int _fire, int _ice, int _lightning, int _poison);
	int getFire(void);
	int getIce(void);
	int getLightning(void);
	int getPoison(void);
	void zero(void);
	void add(int _fire, int _ice, int _lightning, int _poison);
	void add(Resistance *other);
};

#endif

