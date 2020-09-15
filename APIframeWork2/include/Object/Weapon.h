#pragma once
#include "MoveObj.h"
class CWeapon :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	CWeapon();
	CWeapon(const CWeapon& weapon);
	~CWeapon();

private:
	class Player*	m_pPlayer;
	bool			m_bAction;

public:
	void SetPlayer(class Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CWeapon* Clone();

public:
	void Attack();
};

