#pragma once
#include "MoveObj.h"
class Effect :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Effect();
	Effect(const Effect& effect);
	~Effect();

private:
	float	m_fLimitDist;
	float	m_fDist;
	float	m_fLimitTime;
	float	m_fTime;

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Effect* Clone();
};

