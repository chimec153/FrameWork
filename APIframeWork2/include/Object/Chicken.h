#pragma once
#include "FightObj.h"
class Chicken :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Chicken();
	Chicken(const Chicken& chicken);
	virtual ~Chicken();

private:
	int				m_iDay;
	bool			m_bBaby;

public:
	void AddDay(int iDay)
	{
		m_iDay += iDay;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Chicken* Clone();
};

