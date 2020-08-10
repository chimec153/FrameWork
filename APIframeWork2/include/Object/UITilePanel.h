#pragma once
#include "UI.h"
class UITilePanel :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UITilePanel();
	UITilePanel(const UITilePanel& tilepanel);
	~UITilePanel();

private:
	bool			m_bMousePress;
	POSITION		m_tMousePos;
	vector<class Obj*>		m_vecButton;

public:
	bool GetMouseOn()	const
	{
		return m_bMousePress;
	}

	void AddButton(class Obj* pObj)
	{
		m_vecButton.push_back(pObj);
	}

	class Obj* GetButton(int iIndex)
	{
		return m_vecButton[iIndex];
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UITilePanel* Clone();

public:
	void MouseOn(class Collider* pSrc, class Collider* pDest, float fTime);
	void MouseOut(class Collider* pSrc, class Collider* pDest, float fTime);
};

