#pragma once
#include "Item.h"

class Egg :
	public Item
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Egg();
	Egg(const Egg& egg);
	virtual ~Egg();

private:
	int				m_iDay;
	EGG_TYPE		m_eEggType;

public:
	bool AddDay()
	{
		if (m_bInventory)
			return false;

		++m_iDay;

		if (m_iDay >= 2)
		{
			Birth();
			return true;
		}

		return false;
	}

	void Birth();
	void SetEggType(EGG_TYPE eEggType)
	{
		m_eEggType = eEggType;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Egg* Clone();
};

