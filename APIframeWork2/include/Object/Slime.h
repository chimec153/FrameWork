#pragma once
#include "FightObj.h"
class Slime :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Slime();
	Slime(const Slime& slime);
	virtual ~Slime();

private:
	class Texture*		m_pPaceTexture;
	POSITION			m_tPaceImageOffset;
	float				m_fDist;
	class SlimeHead*	m_pHead;

public:
	void SetPaceImageOffset(const POSITION& tOffset)
	{
		m_tPaceImageOffset = tOffset;
	}

	void SetHead(class SlimeHead* pHead);

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Slime* Clone();

public:
	virtual void DieMotion();

public:
	void Collision(class Collider* pSrc, class Collider* pDest, float fTime);

};

