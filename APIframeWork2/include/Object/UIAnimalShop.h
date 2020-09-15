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
	vector<class UIButton*>		m_vecButton;
	vector<class Text*>			m_vecAnimalDescription;

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
	void DisablePanel(int iNum, float fTime);

public:
	void AnimalInfoOn(int iIndex, float fTime);
	void BuyAnimal(int iIndex, float fTime);
	void AnimalInfoOff(int iIndex, float fTime);
};

