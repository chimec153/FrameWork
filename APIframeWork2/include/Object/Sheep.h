#pragma once
#include "FightObj.h"
class Sheep :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Sheep();
	Sheep(const Sheep& sheep);
	virtual ~Sheep();

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
	virtual Sheep* Clone();
};

