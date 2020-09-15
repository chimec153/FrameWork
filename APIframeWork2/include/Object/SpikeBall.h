#pragma once
#include "MiniGameMonster.h"
class SpikeBall :
	public MiniGameMonster
{
private:
	friend class Obj;
	friend class Scene;

private:
	SpikeBall();
	SpikeBall(const SpikeBall& ball);
	virtual ~SpikeBall();

private:
	bool		m_bBall;

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual SpikeBall* Clone();

public:
	void CollisionBullet(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollLeave(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
public:
	void CreateItem();

};

