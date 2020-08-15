#pragma once
#include "Collider.h"
class ColliderSphere :
	public Collider
{
protected:
	friend class Obj;

protected:
	ColliderSphere();
	ColliderSphere(const ColliderSphere& coll);
	~ColliderSphere();

private:
	SPHERE	m_tInfo;
	SPHERE	m_tWorldInfo;

public:
	void SetSphere(const POSITION& tCenter, float fRadius)
	{
		m_tInfo.tCenter = tCenter;
		m_tInfo.fRadius = fRadius;
	}
	SPHERE GetInfo()	const
	{
		return m_tInfo;
	}

	SPHERE GetWorldInfo()	const
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
	virtual ColliderSphere* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};


