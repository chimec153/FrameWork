#pragma once
#include "MiniGameMonster.h"
class Mummy :
	public MiniGameMonster
{
private:
	friend class Obj;
	friend class Scene;

private:
	Mummy();
	Mummy(const Mummy& mummy);
	virtual ~Mummy();

private:


public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Mummy* Clone();

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

