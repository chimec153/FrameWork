#pragma once
#include "StaticObj.h"
class CBuilding :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

protected:
	CBuilding();
	CBuilding(const CBuilding& building);
	virtual ~CBuilding();

private:
	class Texture* m_pDoorTex;
	POSITION m_tDoorPivot;
	POSITION m_tDoorSize;

public:
	class Texture* GetDoorTexture()	const
	{
		return m_pDoorTex;
	}

	void SetTreeTexture(class Texture* pTex)
	{
		m_pDoorTex = pTex;
	}

	void SetTreePivot(float x, float y)
	{
		m_tDoorPivot.x = x;
		m_tDoorPivot.y = y;
	}

	void SetTreeSize(float x, float y)
	{
		m_tDoorSize.x = x;
		m_tDoorSize.y = y;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CBuilding* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void Hit(Collider* pSrc, Collider* pDest, float fDeltaTime);
	void HitStay(Collider* pSrc, Collider* pDest, float fDeltaTime);
};

