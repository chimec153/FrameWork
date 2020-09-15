#pragma once
#include "FightObj.h"
class RockCrab :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	RockCrab();
	RockCrab(const RockCrab& rockcrab);
	virtual ~RockCrab();

private:
	float				m_fDist;
	bool				m_bRock;
	bool				m_bAwake;
	int					m_iRock;

public:
	void SetDist(float fDist)
	{
		m_fDist = fDist;
	}

	void SetRock(bool bRock)
	{
		m_bRock = bRock;
	}

	void SetAwake(bool bAwake)
	{
		m_bAwake = bAwake;
	}

	bool GetRock()	const
	{
		return m_bRock;
	}

	bool GetAwake()	const
	{
		return m_bAwake;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual RockCrab* Clone();

public:
	void Collision(class Collider* pSrc, class Collider* pDest, float fTime);

public:
	virtual void Hitted(int iAttack, const POSITION& tPos);
	virtual void DieMotion();
};

