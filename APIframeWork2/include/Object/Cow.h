#pragma once
#include "FightObj.h"
class Cow :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Cow();
	Cow(const Cow& cow);
	virtual ~Cow();

private:
	float			m_fTime;
	int				m_iLevel;

public:
	void AddTime(float fTime)
	{
		m_fTime += fTime;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Cow* Clone();
};

