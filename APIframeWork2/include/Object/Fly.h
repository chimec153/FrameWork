#pragma once
#include "FightObj.h"
class Fly :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Fly();
	Fly(const Fly& slime);
	virtual ~Fly();

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
	virtual Fly* Clone();

public:
	void Collision(class Collider* pSrc, class Collider* pDest, float fTime);
};

