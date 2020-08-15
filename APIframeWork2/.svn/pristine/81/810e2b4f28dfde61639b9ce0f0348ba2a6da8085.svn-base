#pragma once
#include "MoveObj.h"
class Bullet :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Bullet();
	Bullet(const Bullet& bullet);
	~Bullet();

private:
	float	m_fLimitDist;
	float	m_fDist;


public:
	void SetBulletDistance(float fDist)
	{
		m_fLimitDist = fDist;
	}
public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Bullet* Clone();

public:
	void Hit(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
};

