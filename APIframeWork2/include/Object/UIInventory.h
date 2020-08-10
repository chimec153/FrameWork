#pragma once
#include "UI.h"
class UIInventory :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIInventory();
	~UIInventory();

private:
	vector<class Item*>			m_vecItem;
	int							m_iCount;
	int							m_iCursor;
	class Texture*				m_pCursorTex;

public:
	class Item* GetItem(int iIndex)	const
	{
		return m_vecItem[iIndex];
	}
	class Item* GetItem()	const
	{
		return m_vecItem[m_iCursor];
	}
	void AddItem(class Item* pItem);

	void SetCursor(int iCursor)
	{
		m_iCursor = iCursor;
	}

	void AddCursor(int iNum)
	{
		m_iCursor += iNum;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIInventory* Clone();
};

