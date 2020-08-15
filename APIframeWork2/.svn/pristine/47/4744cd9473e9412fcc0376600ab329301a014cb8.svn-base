#pragma once
#include "FightObj.h"
class Bug :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Bug();
	Bug(const Bug& slime);
	virtual ~Bug();

private:
	float				m_fDist;

public:
	void SetDist(float fDist)
	{
		m_fDist = fDist;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Bug* Clone();

public:
	void Collision(class Collider* pSrc, class Collider* pDest, float fTime);
};

