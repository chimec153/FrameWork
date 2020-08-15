#pragma once
#include "UI.h"
class Mouse :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	Mouse();
	Mouse(const Mouse& mouse);
	~Mouse();

private:
	POSITION	m_tMove;
	POSITION	m_tWorldPos;

public:
	POSITION GetMove()	const
	{
		return m_tMove;
	}

	POSITION GetWorldPos()	const
	{
		return m_tWorldPos;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Mouse* Clone();

public:
	void Hit(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
};