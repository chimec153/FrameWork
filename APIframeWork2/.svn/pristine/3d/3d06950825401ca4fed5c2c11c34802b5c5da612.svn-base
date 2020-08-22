#pragma once
#include "FightObj.h"
class Duck :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Duck();
	Duck(const Duck& duck);
	virtual ~Duck();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Duck* Clone();
};

