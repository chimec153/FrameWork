#pragma once
#include "StaticObj.h"
class CTree :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

protected:
	CTree();
	CTree(const CTree& obj);
	virtual ~CTree();

private:
	class Texture*		m_pTreeSprite;
	bool				m_bCutted;
	POSITION			m_tTreePivot;
	POSITION			m_tTreeSize;
	POSITION			m_tTreeOffset;
	bool				m_bMouseOn;
	bool				m_bLeavesRender;

public:
	class Texture* GetTreeTexture()	const
	{
		return m_pTreeSprite;
	}

	void SetTreeTexture(class Texture* pTex)
	{
		m_pTreeSprite = pTex;
	}

	void SetTreePivot(float x, float y)
	{
		m_tTreePivot.x = x;
		m_tTreePivot.y = y;
	}

	void SetTreeSize(float x, float y)
	{
		m_tTreeSize.x = x;
		m_tTreeSize.y = y;
	}

	void SetTreeOffset(float x, float y)
	{
		m_tTreeOffset.x = x;
		m_tTreeOffset.y = y;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CTree* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void AddCollisionFunc();

public:
	void Hit(Collider* pSrc, Collider* pDest, float fDeltaTime);
	void HitStay(Collider* pSrc, Collider* pDest, float fDeltaTime);
};

