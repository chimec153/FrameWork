#pragma once
#include "FightObj.h"
class Dinosaur :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Dinosaur();
	Dinosaur(const Dinosaur& dinosaur);
	virtual ~Dinosaur();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Dinosaur* Clone();
};

