#pragma once

#include "Animal.h"

class Chicken :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Chicken();
	Chicken(const Chicken& chicken);
	virtual ~Chicken();

private:
	CHICKEN_COLOR	m_eColor;

public:
	void SetColor(CHICKEN_COLOR eColor)
	{
		m_eColor = eColor;
	}

	virtual bool AddDay(int iDay);

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Chicken* Clone();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColLeave(class Collider* pSrc, class Collider* pDest, float fTime);

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
};

