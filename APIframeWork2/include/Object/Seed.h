#pragma once
#include "Item.h"
class Seed :
	public Item
{
private:
	friend class Obj;
	friend class Scene;

private:
	Seed();
	Seed(const Seed& seed);
	~Seed();

private:


public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Seed* Clone();
};

