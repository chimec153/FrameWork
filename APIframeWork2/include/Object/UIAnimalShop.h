#pragma once
#include "UI.h"
class UIAnimalShop :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIAnimalShop();
	UIAnimalShop(const UIAnimalShop& animalshop);
	~UIAnimalShop();

private:
	bool						m_bShopUIOn;
	class UIInventory*			m_pInven;
	vector<class UIPanel*>		m_vecPanel;
	class UIButton*				m_pPrevBtn;
	class UIButton*				m_pNextBtn;
	class UIButton*				m_pExitBtn;
	class UIButton*				m_pBuildBtn;
	class Text*					m_pBuildingName;
	class Text*					m_pBuildingText;
	class Text*					m_pBuildingText2;
	vector<class Text*>			m_vecMaterialText;
	vector<class UINum*>		m_vecMaterialNum;
	int							m_iPage;

public:

	bool IsShopPanelOn()	const
	{
		return m_bShopUIOn;
	}

	void SetInven(class UIInventory* pInven)
	{
		m_pInven = pInven;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIAnimalShop* Clone();

public:
	void CreatePanel();
	void InitPanel();

public:
	void PagePrev(int iNum, float fTime);
	void PageNext(int iNum, float fTime);
	void DisablePanel(int iNum, float fTime);

public:
	void BuyAnimal(int iIndex, float fTime);
};

