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
	bool						m_bColl;
	class UIBar*				m_pEnergyBar;
	class UIBar*				m_pHPBar;
	bool						m_bWalk;
	PLAYER_ACTION				m_eAction;
	int							m_iGold;
	unsigned int				m_iBuildings;
	class Arm*					m_pArm;
	class Pants*				m_pPants;
	class Hair*					m_pHair;
	class Shirt*				m_pShirt;
	class Weapon*				m_pWeapon;

public:
	void SetHPBar(class UIBar* pBar)
	{
		m_pHPBar = pBar;
	}

	void SetEnergyBar(class UIBar* pBar)
	{
		m_pEnergyBar = pBar;
	}

	void AddGold(int iGold);

	int GetGold()	const
	{
		return m_iGold;
	}

	virtual bool AddEnergy(int iEnergy);
	virtual bool AddHP(int iHP);

	virtual void SetEnergy(int iEnergy);
	virtual void SetHP(int iHP);

	void Build(BUILDING_TYPE eType)
	{
		m_iBuildings |= eType;
	}

	void BuildComplete(BUILDING_TYPE eType)
	{
		m_iBuildings ^= eType;
	}

	bool GetBuilding(BUILDING_TYPE eType)
	{
		return m_iBuildings & eType;
	}

	void SetArm(class Arm* pArm);
	void SetPants(class Pants* pPants);
	void SetHair(class Hair* pHair);
	void SetShirt(class Shirt* pShirt);

	PLAYER_ACTION GetPlayerAction()	const
	{
		return m_eAction;
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

public:
	void SetBarLayer(class Layer* pLayer);
	void AddObjectToLayer(class Layer* pLayer);
};

