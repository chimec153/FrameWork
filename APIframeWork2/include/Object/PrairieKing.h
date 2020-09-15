#pragma once
#include "FightObj.h"

enum KING_STATE
{
	KS_NONE,
	KS_ZOMBIE,
	KS_STEALTH,
	KS_REVIVE = 4,
	KS_END
};

class PrairieKing :
	public MoveObj
{
	friend class Scene;
	friend class Obj;

private:
	PrairieKing();
	PrairieKing(const PrairieKing& king);
	virtual ~PrairieKing();

private:
	class PrairieKingLeg*		m_pLeg;
	PLAYER_ACTION				m_eAction;
	float						m_fLimitTime;
	float						m_fTime;
	MINI_ITEM_TYPE				m_eItemType;
	int							m_iLife;
	int							m_iMoney;
	float						m_fZombieTime;
	float						m_fStealthTime;
	unsigned int				m_iState;
	float						m_fReviveTime;
	float						m_fSmokeTime;

public:
	void SetLeg(class PrairieKingLeg* pLeg);
	PLAYER_ACTION GetAction()	const
	{
		return m_eAction;
	}

	bool IsState(KING_STATE eState)	const
	{
		return m_iState & eState;
	}

	int GetLifeCount()	const
	{
		return m_iLife;
	}

	int GetMoney()	const
	{
		return m_iMoney;
	}

	class PrairieKingLeg* GetLeg()	const
	{
		return m_pLeg;
	}

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual PrairieKing* Clone();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);

	void Fire(const POSITION& tAngle, float fTime);
	void ItemUse();
};

