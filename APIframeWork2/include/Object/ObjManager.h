#pragma once

#include "../game.h"

class ObjManager
{
private:
	class Player*			m_pPlayer;
	class UIInventory*		m_pInventory;
	class Obj*				m_pWeapon;
	class Obj*				m_pClockHand;

public:
	void SetPlayer(class Obj* pObj);
	class Obj* GetPlayer()	const;

	class UIInventory* GetInven()	const;

	void SetInven(class UIInventory* pInven);
	void SetWeapon(class Obj* pObj);
	class Obj* GetWeapon()	const;

	void SetClockHand(class Obj* pObj);
	class Obj* GetClockHand()	const
	{
		return m_pClockHand;
	}

public:
	bool Init();
	int Update(float fTime);

public:
	void AddObject(class Layer* pLayer);
	void AddInven(class Layer* pLayer);
	void AddClockHand(class Layer* pLayer);

	DECLARE_SINGLE(ObjManager)
};

