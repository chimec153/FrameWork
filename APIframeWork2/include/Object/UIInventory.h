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
	vector<class UIPanel*>		m_vecPanel;
	class UIPanel*				m_pBackPanel;
	int							m_iCountX;
	int							m_iCountY;
	class UIPanel*				m_pInventoryPanel;
	vector<class UIPanel*>		m_vecInvenPanel;
	class Texture*				m_pBackTexture;
	bool						m_bExtended;
	class Text*					m_pInfoName;
	vector<class Text*>			m_vecInfoDescription;
	class UIPanel*				m_pInfoIconHealth;
	class UIPanel*				m_pInfoIconEnergy;
	class Text*					m_pGoldText;
	class UIShop*				m_pShop;
	class UIButton*				m_pPrevColorBtn;
	class UIButton*				m_pNextColorBtn;
	int							m_iColorIndex;
	vector<COLORREF>			m_vecColor;
	class UIButton*				m_pHairChangeBtn;
	class UIButton*				m_pPantChangeBtn;
	int							m_iPantsColorIndex;
	vector<COLORREF>			m_vecPantsColor;
	class UIButton*				m_pPrevPantsColorBtn;
	class UIButton*				m_pNextPantsColorBtn;
	int							m_iCurrentHairColorIndex;

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
	void DeleteItem(class Item* pItem);

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

	class UIShop* GetShop()	const
	{
		return m_pShop;
	}

	int GetItemCount()	const
	{
		return m_iCount;
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
	void InfoPanelOn(const POSITION& tPos, const ITEMINFO& tInfo);
	void InfoPanelUpdate(const POSITION& tPos);
	void InfoPanelOff();
	void CreateInventory();
	void SwapItem(class Item* pItem, const POSITION& tPos);
	void SetGoldText(int iGold);
	void CreateGoldText();
	void CreateShop(const string& strName);
	void CreateBuildShop();
	void CreateAnimalShop();
	void ChangeHairColor(int, float);
	void GetColorPrev(int , float);
	void GetColorNext(int , float);

public:
	void AddObjectToLayer(class Layer* pLayer);
	void SetObjectLayer(class Layer* pLayer);
	void AddInfoPanelToLayer(class Layer* pLayer);
	void SetInfoPanelLayer(class Layer* pLayer);
	void AddItemToLayer(class Layer* pLayer);

	public:
		void ReadColorFromFile(char* pFileName);
};

