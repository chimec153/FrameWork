#pragma once
#include "FightObj.h"
class Rabbit :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Rabbit();
	Rabbit(const Rabbit& rabbit);
	virtual ~Rabbit();

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
	virtual Rabbit* Clone();
};

