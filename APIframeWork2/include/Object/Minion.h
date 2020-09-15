#pragma once
#include "FightObj.h"
class Minion	:
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Minion();
	Minion(const Minion& minion);
	~Minion();

private:
	bool		m_bWake;
	POSITION	m_tTurnPivot;

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Minion* Clone();

public:
	void CollisionBullet(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);

public:
	virtual void DieMotion();

};

