#pragma once
#include "Animal.h"

enum COW_COLOR
{
	COW_WHITE,
	COW_BROWN,
	COW_END
};

class Cow :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Cow();
	Cow(const Cow& cow);
	virtual ~Cow();

private:
	COW_COLOR		m_eColor;
	bool			m_bMilk;

public:
	void SetColor(COW_COLOR eColor)
	{
		m_eColor = eColor;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Cow* Clone();

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
	virtual bool AddDay(int iDay);

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
};

