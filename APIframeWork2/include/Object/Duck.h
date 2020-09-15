#pragma once
#include "Animal.h"
class Duck :
	public Animal
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

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);
};

