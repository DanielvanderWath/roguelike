#ifndef __BUFF_H__
#define __BUFF_H__

class Character;

class Buff
{
private:
	std::string name;
	int hpPerTurn, mpPerTurn;
	int AV, damage;
	int timeLeft;//if timeleft is < 0 then it is a constant effect caused by an equipped items
public:
	Buff();
	~Buff();
	Buff(std::string _n, int _timeLeft);
	bool tick(Character* target);
	std::string *getName(void);
	void setHPPerTurn(int hp);
	void setMPPerTurn(int mp);
	void setAV(int _av);
	void setDamage(int _damage);
};

#endif

