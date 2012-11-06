#ifndef __BUFF_H__
#define __BUFF_H__

class Buff
{
private:
	char *name;
	int hpPerTurn, mpPerTurn;
	int AV, damage;
	int timeLeft;
public:
	Buff();
	~Buff();
	Buff(const char* _n, int _timeLeft);
	bool tick(void);
	const char *getName(void);
	void setHPPerTurn(int hp);
	void setMPPerTurn(int mp);
	void setAV(int _av);
	void setDamage(int _damage);
};

#endif

