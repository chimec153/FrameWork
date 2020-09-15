#pragma once
#include "Animal.h"
class Rabbit :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Rabbit();
	Rabbit(const Rabbit& rabbit);
	virtual ~Rabbit();

private:
	int			m_iLevel;

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Rabbit* Clone();

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
};

