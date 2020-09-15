#pragma once
#include "Animal.h"
class Sheep :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Sheep();
	Sheep(const Sheep& sheep);
	virtual ~Sheep();

private:
	int			m_iLevel;

public:


public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Sheep* Clone();

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
};

