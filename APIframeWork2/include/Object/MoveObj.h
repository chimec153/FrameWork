#pragma once
#include "Obj.h"
class MoveObj	:	
	public Obj
{
protected:
	MoveObj();
	MoveObj(const MoveObj& obj);
	virtual ~MoveObj();

protected:
	POSITION	m_tMoveDir;
	float		m_fSpeed;
	bool		m_bMove;
	bool		m_bFalling;
	float		m_fForce;
	float		m_fForceOrigin;
	bool		m_bTileEffect;

public:
	void SetForce(float fForce)
	{
		m_fForceOrigin = fForce;
	}

	virtual void SetAngle(const POSITION& tDir)
	{
		m_tMoveDir = tDir;

		if (m_tMoveDir != 0.f)
			m_tMoveDir.Normalize();
	}

	void SetAngle(float x, float y)
	{
		m_tMoveDir = POSITION(x, y);

		if(m_tMoveDir != 0.f)
			m_tMoveDir.Normalize();
	}

	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	POSITION GetAngle()	const
	{
		return m_tMoveDir;
	}

	float GetSpeed()	const
	{
		return m_fSpeed;
	}

	bool GetTileEffect()	const
	{
		return m_bTileEffect;
	}
	
	void SetTileEffect(bool bTile)
	{
		m_bTileEffect = bTile;
	}

public:
	void Move(float x, float y);
	void Move(float x, float y,float fDeltaTime);
	void Move(const POSITION& tMove);
	void Move(const POSITION& tMove,float fDeltaTime);
	void MoveX(float x);
	void MoveXFromSpeed(float fDeltaTime, float fScale);
	void MoveX(float x, float fDeltaTime);
	void MoveY(float y);
	void MoveYFromSpeed(float fDeltaTime, float fScale);
	void MoveY(float y, float fDeltaTime);
	void MoveAngle();
	bool MoveAngle(float fDeltaTime);
	void MoveAngle(float fSpeed, float fDeltaTime);
	void Jump();
	void JumpEnd();

public:
	virtual bool Init()=0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual MoveObj* Clone() = 0;
};

