#ifndef __RACE_H__
#define __RACE_H__

class Race
{
private:
	int hp_min, hp_variance;
	int mp_min, mp_variance;
	char *_name;
public:
	Race(void);
	~Race(void);
	Race(const char *n, int hpm, int hpv, int mpm, int mpv);
	int hp(void);
	int mp(void);
	const char *name(void);
};

#endif

