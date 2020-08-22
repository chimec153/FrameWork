#pragma once
#include "UI.h"
class UIShop :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIShop();
	UIShop(const UIShop& shop);
	~UIShop();

private:
	bool						m_bShopUIOn;
	class UIPanel*				m_pShopPanel;
	vector<class UIPanel*>		m_vecShopPanel;
	vector<class UIContext*>	m_vecShopItemPanel;
	class UIPanel*				m_pShopGoldPanel;
	class UIPanel*				m_pShopBackPanel;
	class UIButton*				m_pShopExitBtn;
	class UIButton*				m_pShopUpBtn;
	class UIButton*				m_pShopDownBtn;
	class UITilePanel*			m_pShopScrollBtn;
	int							m_iPage;
	class Text*					m_pShopGoldText;
	class UIPanel*				m_pShopPacePanel;
	vector<class UIPanel*>		m_vecShopCommentPanel;
	class UIPanel*				m_pShopCommentBack;
	vector<class Text*>			m_vecShopComment;
	string						m_strNPCName;
	class UIInventory*			m_pInven;

public:

	bool IsShopPanelOn()	const
	{
		return m_bShopUIOn;
	}

	void SetGoldText(const TCHAR* strNum);

	void SetInven(class UIInventory* pInven)
	{
		m_pInven = pInven;
	}

	POSITION GetShopPanelPos()	const;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIShop* Clone();

public:
	void CreateShopPanel(const string& strName);
	void InitShopPanel(const string& strName);

public:
	void ShopPageUp(int iNum, float fTime);
	void ShopPageDown(int iNum, float fTime);
	void DisableShopPanel(int iNum, float fTime);
	void UpdateShopItemPos(float fTime);
	void UpdateScrollBtnPos();

public:
	void BuyItem(int iIndex);

public:
	void AddObjectToLayer(class Layer* pLayer);
	void SetObjectLayer(class Layer* pLayer);
};

