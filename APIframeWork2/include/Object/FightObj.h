#pragma once
#include "MoveObj.h"
class FightObj :
	public MoveObj
{
protected:
	FightObj();
	FightObj(const FightObj& fighter);
	virtual ~FightObj() = 0;

protected:
	int				m_iHP;
	int				m_iEnergy;
	int				m_iAttack;
	float			m_fHitTime;
	float			m_fHitSpeed;
	POSITION		m_tHitAngle;
	bool			m_bHitted;

public:
	void SetHitTime(float fTime)
	{
		m_bHitted = true;

		m_fHitTime = fTime;
	}

	void SetHitAngle(const POSITION& tHitPos)
	{
		POSITION tAngle = m_tPos - tHitPos;

		tAngle.Normalize();

		m_tHitAngle = tAngle;
	}

	void SetHitAngle(float HitPos_x, float HitPos_y)
	{
		POSITION tAngle = m_tPos - POSITION(HitPos_x, HitPos_y);

		tAngle.Normalize();

		m_tHitAngle = tAngle;
	}

	void SetHitSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	void SetAttack(int iAttack)
	{
		m_iAttack = iAttack;
	}

	int GetAttack()	const
	{
		return m_iAttack;
	}

	virtual void SetEnergy(int iEnergy)
	{
		m_iEnergy = iEnergy;
	}

	virtual void SetHP(int iHP)
	{
		m_iHP = iHP;
	}

	virtual bool AddEnergy(int iEnergy)
	{
		m_iEnergy += iEnergy;

		if (m_iEnergy <= 0)
			return true;

		return false;
	}

	virtual bool AddHP(int iHP)
	{
		m_iHP += iHP;

		if (m_iHP <= 0)
			return true;

		return false;
	}

	int GetHP()	const
	{
		return m_iHP;
	}

	int GetEnergy()	const
	{
		return m_iEnergy;
	}

	POSITION GetHitAngle()	const
	{
		return m_tHitAngle;
	}

	float GetHitTime()	const
	{
		return m_fHitTime;
	}

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual FightObj* Clone() = 0;

protected:
	virtual void Hitted(int iAttack, const POSITION& tPos);
	virtual void DieMotion();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
};

