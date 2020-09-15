#pragma once
#include "MiniGameMonster.h"
class Boss1 :
	public MiniGameMonster
{
private:
	friend class Obj;
	friend class Scene;

private:
	Boss1();
	Boss1(const Boss1& boss);
	virtual ~Boss1();

protected:
	class UIPanel*	m_pScriptPanel;
	float			m_fScriptTime;

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Boss1* Clone();

public:
	void CollisionBullet(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollStay(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void CollLeave(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
};

