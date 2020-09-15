#pragma once
#include "MiniGameMonster.h"
class Orc :
	public MiniGameMonster
{
private:
	friend class Obj;
	friend class Scene;

private:
	Orc();
	Orc(const Orc& orc);
	virtual ~Orc();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Orc* Clone();

public:
	void CollisionBullet(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollLeave(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
public:
	void CreateItem();
	void SetToPrevPos();

};

