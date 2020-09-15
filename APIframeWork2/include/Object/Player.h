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
	vector<int>					m_vecAnimal;
	class Arm*					m_pArm;
	class Pants*				m_pPants;
	class Hair*					m_pHair;
	class Shirt*				m_pShirt;
	class CWeapon*				m_pWeapon;
	bool						m_bSilo;
	bool						m_bShooting;
	class Hat*					m_pHat;

public:
	void SetHPBar(class UIBar* pBar)
	{
		m_pHPBar = pBar;
	}

	void SetEnergyBar(class UIBar* pBar)
	{
		m_pEnergyBar = pBar;
	}

	class UIBar* GetHPBar()	const
	{
		return m_pHPBar;
	}

	class UIBar* GetEnergyBar()	const
	{
		return m_pEnergyBar;
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

		if (eType == BT_SILO)
			m_bSilo = true;
	}

	void BuildComplete(BUILDING_TYPE eType)
	{
		m_iBuildings ^= eType;
	}

	bool GetBuilding(BUILDING_TYPE eType)
	{
		return m_iBuildings & eType;
	}

	void BuyAnimal(ANIMAL_TYPE eType)
	{
		m_vecAnimal[eType] += 1;
	}

	void LocateAnimal(ANIMAL_TYPE eType,int iAnimal)
	{
		m_vecAnimal[eType] -= iAnimal;
	}

	int GetAnimal(ANIMAL_TYPE eType)
	{
		return m_vecAnimal[eType];
	}

	bool GetSilo()	const
	{
		return m_bSilo;
	}

	void SetArm(class Arm* pArm);
	void SetPants(class Pants* pPants);
	void SetHair(class Hair* pHair);
	void SetShirt(class Shirt* pShirt);
	void SetHat(class Hat* pHat);

	PLAYER_ACTION GetPlayerAction()	const
	{
		return m_eAction;
	}
	
	void SetPlayerAction(PLAYER_ACTION eAction)
	{
		m_eAction = eAction;
	}

	void SetWeapon(class CWeapon* pWeapon)
	{
		m_pWeapon = pWeapon;
	}

	class Obj* GetArm()	const
	{
		return (Obj*)m_pArm;
	}

	class Obj* GetPants()	const
	{
		return (Obj*)m_pPants;
	}

	class Obj* GetHair()	const
	{
		return (Obj*)m_pHair;
	}

	class Obj* GetShirt()	const
	{
		return (Obj*)m_pShirt;
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
	virtual void SetAngle(const POSITION& tAngle);
	virtual void SetAngle(float fX, float fY);
};

