#pragma once
#include "MoveObj.h"
class Item :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Item();
	Item(const Item& item);
	virtual ~Item() = 0;

protected:
	ITEM_TYPE		m_eType;
	bool			m_bInventory;

public:
	void SetInventory(bool bInventory)
	{
		m_bInventory = bInventory;
	}

	ITEM_TYPE GetType()	const
	{
		return m_eType;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Item* Clone();

public:
	void CollEnter(class Collider* pSrc, class Collider* pDest, float fTime);
};

