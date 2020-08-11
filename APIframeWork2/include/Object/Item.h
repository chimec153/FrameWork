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
	ITEM_TYPE					m_eType;
	bool						m_bInventory;
	bool						m_bMouseOn;
	class UIInventory*			m_pInventory;

public:
	void SetInventory(class UIInventory* pInven)
	{
		m_pInventory = pInven;

		if(m_pInventory)
			m_bInventory = true;
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
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColEnd(class Collider* pSrc, class Collider* pDest, float fTime);
};

