#pragma once
#include "Animal.h"
class Pig :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Pig();
	Pig(const Pig& pig);
	virtual ~Pig();

private:
	float		m_fTime;
	int			m_iLevel;

public:
	void AddTime(float fTime)
	{
		m_fTime = fTime;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Pig* Clone();

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);
};

