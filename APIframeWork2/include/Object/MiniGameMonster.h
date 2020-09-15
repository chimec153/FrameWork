#pragma once
#include "MoveObj.h"
  class MiniGameMonster :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

protected:
	MiniGameMonster();
	MiniGameMonster(const MiniGameMonster& mon);
	virtual ~MiniGameMonster();

protected:
	int								m_iHP;
	POSITION						m_tPrevMove;
	MINI_MONSTER_TYPE				m_eMonsterType;

public:
	int GetHP()	const
	{
		return m_iHP;
	}

	void AddHP(int iHP)
	{
		m_iHP -= iHP;
	}

	void SetPrevMove(const POSITION& tPos)
	{
		m_tPrevMove = tPos;
	}

	void SetHP(int iHP)
	{
		m_iHP = iHP;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual MiniGameMonster* Clone();

public:
	void CollisionBullet(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollLeave(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
public:
	void CreateItem();
};

