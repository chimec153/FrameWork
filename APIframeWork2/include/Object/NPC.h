#pragma once
#include "FightObj.h"
class NPC :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	NPC();
	NPC(const NPC& npc);
	~NPC();

private:
	float		m_fLimitDist;
	float		m_fDist;
	bool		m_bWalkStart;

public:
	void Walk()
	{
		m_bWalkStart = true;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual NPC* Clone();

public:
	void ColEnter(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollEnd(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
};

