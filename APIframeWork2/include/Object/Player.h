#pragma once
#include "FightObj.h"

class Player :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Player();
	Player(const Player& player);
	~Player();

private:
	bool						m_bAttack;
	PLAYER_ANIMATION_TYPE		m_eType;
	bool						m_bColl;
	class UIBar*				m_pEnergyBar;
	class UIBar*				m_pHPBar;
	class Obj*					m_pWeapon;
	class UIInventory*			m_pInventory;
	int							m_iInvenCursor;

public:
	void SetType(PLAYER_ANIMATION_TYPE eType)
	{
		m_eType = eType;
	}

	void SetHPBar(class UIBar* pBar)
	{
		m_pHPBar = pBar;
	}

	void SetEnergyBar(class UIBar* pBar)
	{
		m_pEnergyBar = pBar;
	}
	void SetWeapon(class Obj* pObj);
	class UIInventory* GetInven()	const
	{
		return m_pInventory;
	}

	void SetInven(class UIInventory* pInven)
	{
		m_pInventory = pInven;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Player* Clone();

public:
	void Hit(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void HitStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void HitLeave(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);

private:
	void Fire();
};

