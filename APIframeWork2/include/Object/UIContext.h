#pragma once
#include "UI.h"
class UIContext :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIContext();
	UIContext(const UIContext& context);
	~UIContext();

private:
	class Obj*				m_pItem;
	class Text*				m_pName;
	class Text*				m_pPrice;
	class Obj*				m_pIcon;
	class UIInventory*		m_pInven;
	int						m_iIndex;

public:
	void SetInven(class UIInventory* pInven)
	{
		m_pInven = pInven;
	}

	void SetIndex(int iIndex);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIContext* Clone();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColEnd(class Collider* pSrc, class Collider* pDest, float fTime);
};

