#pragma once
#include "Collider.h"
class ColliderRect :
	public Collider
{
protected:
	friend class Obj;

protected:
	ColliderRect();
	ColliderRect(const ColliderRect& coll);
	virtual ~ColliderRect();

private:
	RECTANGLE	m_tInfo;
	RECTANGLE	m_tWorldInfo;

public:
	void SetRect(float l, float t, float r, float b);
	RECTANGLE GetInfo()	const
	{
		return m_tInfo;
	}

	RECTANGLE GetWorldInfo()	const
	{
		return m_tWorldInfo;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(Collider* pDest);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual ColliderRect* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

