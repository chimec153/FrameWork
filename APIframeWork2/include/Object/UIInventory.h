#pragma once
#include "UI.h"

typedef struct _tagItemPanel
{
	class Obj*			pItem;
	class Obj*			pName;
	class Obj*			pPrice;
	class Obj*			pIcon;
	POSITION			tPos;

	_tagItemPanel() :
		pItem(nullptr),
		pName(nullptr),
		pPrice(nullptr),
		pIcon(nullptr)
	{

	}

	~_tagItemPanel()
	{
		SAFE_RELEASE(pItem);
		SAFE_RELEASE(pName);
		SAFE_RELEASE(pPrice);
		SAFE_RELEASE(pIcon);
	}
}ITEMPANEL, *PITEMPANEL;

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
	vector<class UIPanel*>		m_vecPanel;
	class UIPanel*				m_pBackPanel;
	int							m_iCountX;
	int							m_iCountY;
	class UIPanel*				m_pInventoryPanel;
	vector<class UIPanel*>		m_vecInvenPanel;
	class Texture*				m_pBackTexture;
	bool						m_bExtended;
	bool						m_bShopUIOn;
	class UIPanel*				m_pShopPanel;
	vector<class UIPanel*>		m_vecShopPanel;
	vector<PITEMPANEL>			m_vecShopItemPanel;
	class UIPanel*				m_pShopValancePanel;
	class UIPanel*				m_pShopBackPanel;
	class UIButton*				m_pShopExitBtn;
	class UIButton*				m_pShopUpBtn;
	class UIButton*				m_pShopDownBtn;
	class UITilePanel*			m_pShopScrollBtn;
	int							m_iPage;

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
	void DeleteItem();

	void SetCursor(int iCursor)
	{
		m_iCursor = iCursor;
	}

	void AddCursor(int iNum)
	{
		m_iCursor += iNum;
	}

	bool GetExtended()	const
	{
		return m_bExtended;
	}

	bool IsShopPanelOn()	const
	{
		return m_bShopUIOn;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIInventory* Clone();

public:
	void CreateInfoPanel(int iCountX, int iCountY);
	void InfoPanelOn(const POSITION& tPos);
	void InfoPanelUpdate(const POSITION& tPos);
	void InfoPanelOff();
	void CreateInventory();
	void SwapItem(class Item* pItem, const POSITION& tPos);
	void CreateShopPanel();

public:
	void ShopPageUp(int iNum, float fTime);
	void ShopPageDown(int iNum, float fTime);
	void DisableShopPanel(int iNum, float fTime);
};

