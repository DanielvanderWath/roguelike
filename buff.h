#ifndef __BUFF_H__
#define __BUFF_H__

class Character;

class Buff
{
private:
	char *name;
	int hpPerTurn, mpPerTurn;
	int AV, damage;
	int timeLeft;//if timeleft is < 0 then it is a constant effect caused by an equipped items
public:
	Buff();
	~Buff();
	Buff(const char* _n, int _timeLeft);
	bool tick(Character* target);
	const char *getName(void);
	void setHPPerTurn(int hp);
	void setMPPerTurn(int mp);
	void setAV(int _av);
	void setDamage(int _damage);
};

#endif

