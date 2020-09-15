#pragma once
#include "Animal.h"
class Goat :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Goat();
	Goat(const Goat& goat);
	virtual ~Goat();

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
	virtual Goat* Clone();

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);
};

