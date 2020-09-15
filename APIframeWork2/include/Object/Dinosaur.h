#pragma once
#include "Animal.h"
class Dinosaur :
	public Animal
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

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);
};

