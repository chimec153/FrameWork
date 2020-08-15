#pragma once
#include "FightObj.h"
class Slime :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Slime();
	Slime(const Slime& slime);
	virtual ~Slime();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Slime* Clone();

public:
	void Collision(class Collider* pSrc, class Collider* pDest, float fTime);

};

