#pragma once
#include "MiniGameMonster.h"
class Orge :
	public MiniGameMonster
{
private:
	friend class Obj;
	friend class Scene;

private:
	Orge();
	Orge(const Orge& orge);
	virtual ~Orge();

private:


public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Orge* Clone();

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

