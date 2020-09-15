#include "UIAnimalShop.h"
#include "UIPanel.h"
#include "UIButton.h"
#include "../Scene/Scene.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "Text.h"
#include "ObjManager.h"
#include "Player.h"
#include "UIInventory.h"
#include "../Core/Input.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/NPC.h"

UIAnimalShop::UIAnimalShop()	:
	m_bShopUIOn(false),
	m_pInven(nullptr)
{
	
}

UIAnimalShop::UIAnimalShop(const UIAnimalShop& animalshop)	:
	UI(animalshop)
{
	m_bShopUIOn = animalshop.m_bShopUIOn;
	m_pInven = animalshop.m_pInven;
}

UIAnimalShop::~UIAnimalShop()
{
	SAFE_RELEASE_VECLIST(m_vecButton);
	SAFE_RELEASE_VECLIST(m_vecAnimalDescription);
	SAFE_RELEASE_VECLIST(m_vecPanel);
}

bool UIAnimalShop::Init()
{
	return true;
}

void UIAnimalShop::Input(float fDeltaTime)
{
}

int UIAnimalShop::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);
	return 0;
}

int UIAnimalShop::LateUpdate(float fDeltaTime)
{
	UI::Update(fDeltaTime);
	return 0;
}

void UIAnimalShop::Collision(float fDeltaTime)
{
	UI::Update(fDeltaTime);
}

void UIAnimalShop::Render(HDC hDC, float fDeltaTime)
{
	UI::Update(fDeltaTime);

	size_t iSize = m_vecPanel.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecPanel[i]->Render(hDC, fDeltaTime);

	size_t iTextSize = m_vecAnimalDescription.size();

	for (size_t i = 0; i < iTextSize; ++i)
		m_vecAnimalDescription[i]->Render(hDC, fDeltaTime);

	size_t iBtnSize = m_vecButton.size();

	for (size_t i = 0; i < iBtnSize; ++i)
		m_vecButton[i]->Render(hDC, fDeltaTime);
}

UIAnimalShop* UIAnimalShop::Clone()
{
	return new UIAnimalShop(*this);
}

void UIAnimalShop::CreatePanel()
{
	m_bShopUIOn = true;

	if (m_vecPanel.size() == 0)
	{
		int iTileNumX = 7;
		int iTileNumY = 7;

		Layer* pLayer = m_pScene->FindLayer("Default");

		UIPanel* pPanel = CreateObj<UIPanel>("AnimalPanel", pLayer);	//	상점 배경을 만든다.

		pPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

		pPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

		POSITION tSize = pPanel->GetSize();

		pPanel->SetPos(tRS.iW / 2.f - (tSize.x * 2) / 2.f, tRS.iH / 2.f - tSize.y / 2.f);

		pPanel->SetEnable(false);

		m_vecPanel.push_back(pPanel);

		m_vecPanel.reserve(132);

		POSITION tPos = pPanel->GetPos();

		for (int i = 0; i < iTileNumY; ++i)		//	상점 틀을 만든다.
		{
			for (int j = 0; j < iTileNumX; ++j)
			{
				if (i == 0 || i == iTileNumY - 1 || j == 0 || j == iTileNumX - 1)
				{
					UIPanel* pPanel = CreateObj<UIPanel>("Panel", pLayer);

					pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

					pPanel->SetColorKey(255, 255, 255);

					pPanel->SetSize(32.f, 32.f);

					pPanel->SetPos(32.f * j + tPos.x - 16.f, 32.f * i + tPos.y - 16.f);

					if (i == 0 && j != 0 && j != iTileNumX - 1)
						pPanel->SetImageOffset(64.f, 0.f);

					else if (i == iTileNumY - 1 && j != 0 && j != iTileNumX - 1)
						pPanel->SetImageOffset(64.f, 96.f);

					else if (j == 0 && i != 0 && i != iTileNumY - 1)
						pPanel->SetImageOffset(0.f, 64.f);

					else if (j == iTileNumX - 1 && i != 0 && i != iTileNumY - 1)
						pPanel->SetImageOffset(96.f, 64.f);

					else if (i == iTileNumY - 1 && j == 0)
						pPanel->SetImageOffset(0.f, 96.f);

					else if (i == 0 && j == iTileNumX - 1)
						pPanel->SetImageOffset(96.f, 0.f);

					else if (i == iTileNumY - 1 && j == iTileNumX - 1)
						pPanel->SetImageOffset(96.f, 96.f);

					pPanel->SetEnable(false);

					m_vecPanel.push_back(pPanel);
				}
			}
		}

		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		UIButton* pExitBtn = Obj::CreateObj<UIButton>("ExitButton", pLayer);	//	상점 판넬 종료 버튼을 생성한다.

		pExitBtn->SetSize(64.f, 64.f);
		pExitBtn->SetImageOffset(1344.f, 192.f);
		pExitBtn->SetTexture("Mouse");

		pExitBtn->SetCallback(CS_STAY, this, &UIAnimalShop::DisablePanel);

		pExitBtn->SetAlpha(255);
		pExitBtn->EnableAlpha(true);
		pExitBtn->SetPos(tPos.x + tSize.x + 20.f, tPos.y + tSize.y - pExitBtn->GetSize().y);
		pExitBtn->SetEnable(true);

		ColliderRect* pExitButtonRC = (ColliderRect*)pExitBtn->GetCollider("ButtonBody");

		pExitButtonRC->SetRect(0.f, 0.f, 64.f, 64.f);

		SAFE_RELEASE(pExitButtonRC);
		
		m_vecButton.push_back(pExitBtn);

		UIButton* pChickenBtn = Obj::CreateObj<UIButton>("ChickenButton", pLayer);	//	닭 버튼을 생성한다.

		pChickenBtn->SetSize(52.f, 32.f);
		pChickenBtn->SetImageOffset(6.f, 896.f);
		pChickenBtn->SetTexture("Mouse");

		pChickenBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pChickenBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pChickenBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pChickenBtn->SetAlpha(255);
		pChickenBtn->EnableAlpha(true);

		POSITION tChickenBtnSize = pChickenBtn->GetSize();

		pChickenBtn->SetPos(tPos.x + tSize.x / 4.f - tChickenBtnSize.x/2.f , tPos.y + tSize.y / 4.f - tChickenBtnSize.y/2.f);
		pChickenBtn->SetEnable(true);
		pChickenBtn->SetIndex(0);

		ColliderRect* pChickenButtonRC = (ColliderRect*)pChickenBtn->GetCollider("ButtonBody");

		pChickenButtonRC->SetRect(0.f, 0.f, 52.f, 32.f);

		SAFE_RELEASE(pChickenButtonRC);

		m_vecButton.push_back(pChickenBtn);

		UIButton* pMilkCowBtn = Obj::CreateObj<UIButton>("MilkCowButton", pLayer);	//	젖소 버튼을 생성한다.

		pMilkCowBtn->SetSize(34.f, 28.f);
		pMilkCowBtn->SetImageOffset(80.f, 898.f);
		pMilkCowBtn->SetTexture("Mouse");

		pMilkCowBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pMilkCowBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pMilkCowBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pMilkCowBtn->SetAlpha(255);
		pMilkCowBtn->EnableAlpha(true);

		POSITION tMilkCowBtnSize = pMilkCowBtn->GetSize();

		pMilkCowBtn->SetPos(tPos.x + tSize.x * 2.f / 4.f - tMilkCowBtnSize.x/2.f, tPos.y + tSize.y / 4.f - tMilkCowBtnSize.y/2.f);
		pMilkCowBtn->SetEnable(true);
		pMilkCowBtn->SetIndex(1);

		ColliderRect* pMilkCowButtonRC = (ColliderRect*)pMilkCowBtn->GetCollider("ButtonBody");

		pMilkCowButtonRC->SetRect(0.f, 0.f, 34.f, 28.f);

		SAFE_RELEASE(pMilkCowButtonRC);

		m_vecButton.push_back(pMilkCowBtn);

		UIButton* pGoatBtn = Obj::CreateObj<UIButton>("GoatButton", pLayer);	//	염소 버튼을 생성한다.

		pGoatBtn->SetSize(42.f, 32.f);
		pGoatBtn->SetImageOffset(140.f, 896.f);
		pGoatBtn->SetTexture("Mouse");

		pGoatBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pGoatBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pGoatBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pGoatBtn->SetAlpha(255);
		pGoatBtn->EnableAlpha(true);

		POSITION tGoatBtnSize = pGoatBtn->GetSize();

		pGoatBtn->SetPos(tPos.x + tSize.x * 3.f / 4.f - tGoatBtnSize.x / 2.f, tPos.y + tSize.y / 4.f - tGoatBtnSize.y / 2.f);
		pGoatBtn->SetEnable(true);
		pGoatBtn->SetIndex(2);

		ColliderRect* pGoatButtonRC = (ColliderRect*)pGoatBtn->GetCollider("ButtonBody");

		pGoatButtonRC->SetRect(0.f, 0.f, 42.f, 32.f);

		SAFE_RELEASE(pGoatButtonRC);

		m_vecButton.push_back(pGoatBtn);

		UIButton* pDuckBtn = Obj::CreateObj<UIButton>("DuckButton", pLayer);	//	오리 버튼을 생성한다.

		pDuckBtn->SetSize(42.f, 30.f);
		pDuckBtn->SetImageOffset(12.f, 930.f);
		pDuckBtn->SetTexture("Mouse");

		pDuckBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pDuckBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pDuckBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pDuckBtn->SetAlpha(255);
		pDuckBtn->EnableAlpha(true);

		POSITION tDuckSize = pDuckBtn->GetSize();

		pDuckBtn->SetPos(tPos.x + tSize.x / 4.f - tDuckSize.x / 2.f, tPos.y + tSize.y * 2.f / 4.f - tDuckSize.y / 2.f);
		pDuckBtn->SetEnable(true);
		pDuckBtn->SetIndex(3);

		ColliderRect* pDuckButtonRC = (ColliderRect*)pDuckBtn->GetCollider("ButtonBody");

		pDuckButtonRC->SetRect(0.f, 0.f, 42.f, 30.f);

		SAFE_RELEASE(pDuckButtonRC);

		m_vecButton.push_back(pDuckBtn);

		UIButton* pSheepBtn = Obj::CreateObj<UIButton>("SheepButton", pLayer);	//	양 버튼을 생성한다.

		pSheepBtn->SetSize(30.f, 24.f);
		pSheepBtn->SetImageOffset(82.f, 934.f);
		pSheepBtn->SetTexture("Mouse");

		pSheepBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pSheepBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pSheepBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pSheepBtn->SetAlpha(255);
		pSheepBtn->EnableAlpha(true);

		POSITION tSheepBtnSize = pSheepBtn->GetSize();

		pSheepBtn->SetPos(tPos.x + tSize.x * 2.f / 4.f - tSheepBtnSize.x / 2.f, tPos.y + tSize.y * 2.f / 4.f - tSheepBtnSize.y / 2.f);
		pSheepBtn->SetEnable(true);
		pSheepBtn->SetIndex(4);

		ColliderRect* pSheepButtonRC = (ColliderRect*)pSheepBtn->GetCollider("ButtonBody");

		pSheepButtonRC->SetRect(0.f, 0.f, 30.f, 24.f);

		SAFE_RELEASE(pSheepButtonRC);

		m_vecButton.push_back(pSheepBtn);

		UIButton* pRabbitBtn = Obj::CreateObj<UIButton>("RabbitButton", pLayer);	//	토끼 버튼을 생성한다.

		pRabbitBtn->SetSize(30.f, 32.f);
		pRabbitBtn->SetImageOffset(144.f, 928.f);
		pRabbitBtn->SetTexture("Mouse");

		pRabbitBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pRabbitBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pRabbitBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pRabbitBtn->SetAlpha(255);
		pRabbitBtn->EnableAlpha(true);

		POSITION tRabbitBtnSize = pRabbitBtn->GetSize();

		pRabbitBtn->SetPos(tPos.x + tSize.x * 3.f / 4.f - tRabbitBtnSize.x/2.f, tPos.y + tSize.y * 2.f/4.f - tRabbitBtnSize.y/2.f);
		pRabbitBtn->SetEnable(true);
		pRabbitBtn->SetIndex(5);

		ColliderRect* pRabbitButtonRC = (ColliderRect*)pRabbitBtn->GetCollider("ButtonBody");

		pRabbitButtonRC->SetRect(0.f, 0.f, 30.f, 32.f);

		SAFE_RELEASE(pRabbitButtonRC);

		m_vecButton.push_back(pRabbitBtn);

		UIButton* pPigBtn = Obj::CreateObj<UIButton>("PigButton", pLayer);	//	돼지 버튼을 생성한다.

		pPigBtn->SetSize(34.f, 26.f);
		pPigBtn->SetImageOffset(16.f, 964.f);
		pPigBtn->SetTexture("Mouse");

		pPigBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pPigBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pPigBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pPigBtn->SetAlpha(255);
		pPigBtn->EnableAlpha(true);

		POSITION tPigBtnSize = pPigBtn->GetSize();

		pPigBtn->SetPos(tPos.x + tSize.x / 4.f - tPigBtnSize.x / 2.f, tPos.y + tSize.y * 3.f / 4.f - tPigBtnSize.y / 2.f);
		pPigBtn->SetEnable(true);
		pPigBtn->SetIndex(6);

		ColliderRect* pPigButtonRC = (ColliderRect*)pPigBtn->GetCollider("ButtonBody");

		pPigButtonRC->SetRect(0.f, 0.f, 34.f, 26.f);

		SAFE_RELEASE(pPigButtonRC);

		m_vecButton.push_back(pPigBtn);

		UIButton* pBrownCowBtn = Obj::CreateObj<UIButton>("BrownButton", pLayer);	//	황소 버튼을 생성한다.

		pBrownCowBtn->SetSize(34.f, 30.f);
		pBrownCowBtn->SetImageOffset(144.f, 962.f);
		pBrownCowBtn->SetTexture("Mouse");

		pBrownCowBtn->SetCallback(CS_ENTER, this, &UIAnimalShop::AnimalInfoOn);
		pBrownCowBtn->SetCallback(CS_STAY, this, &UIAnimalShop::BuyAnimal);
		pBrownCowBtn->SetCallback(CS_LEAVE, this, &UIAnimalShop::AnimalInfoOff);

		pBrownCowBtn->SetAlpha(255);
		pBrownCowBtn->EnableAlpha(true);

		POSITION tBrownCowBtnSize = pBrownCowBtn->GetSize();

		pBrownCowBtn->SetPos(tPos.x + tSize.x * 2.f/ 4.f - tBrownCowBtnSize.x/2.f, tPos.y + tSize.y *3.f / 4.f - tBrownCowBtnSize.y/2.f);
		pBrownCowBtn->SetEnable(true);
		pBrownCowBtn->SetIndex(7);

		ColliderRect* pBrownCowButtonRC = (ColliderRect*)pBrownCowBtn->GetCollider("ButtonBody");

		pBrownCowButtonRC->SetRect(0.f, 0.f, 34.f, 30.f);

		SAFE_RELEASE(pBrownCowButtonRC);

		m_vecButton.push_back(pBrownCowBtn);

		pPanel = Obj::CreateObj<UIPanel>("AnimalShopNamePanelLeft", pLayer);	//	제목 판넬 왼쪽을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(650.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tTitlePanelLeftSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x + 20.f , tPos.y - tTitlePanelLeftSize.y - 20.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		Text* pTitle = Obj::CreateObj<Text>("AnimalTitle", pLayer);

		pTitle->SetText(TEXT("Livestock:"));

		pTitle->SetColor(102, 27, 27);

		POSITION tOffset = POSITION(10.f, 10.f);

		pTitle->SetPos(tPos.x + 20.f + tOffset.x + 10.f, tPos.y - tTitlePanelLeftSize.y - 20.f + tOffset.y);

		pTitle->SetEnable(false);
		
		m_vecAnimalDescription.push_back(pTitle);

		pPanel = Obj::CreateObj<UIPanel>("AnimalShopNamePanel", pLayer);	//	제목 판넬을 생성한다.

		pPanel->SetSize(122.f, 34.f);
		pPanel->SetImageOffset(1203.f, 978.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tTitlePanelSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x + tTitlePanelLeftSize.x + 20.f, tPos.y - tTitlePanelLeftSize.y - 20.f + 2.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("AnimalShopNamePanelRight", pLayer);	//	제목 판넬 오른쪽을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(675.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);
		pPanel->SetPos(tPos.x + tTitlePanelLeftSize.x + tTitlePanelSize.x + 20.f, tPos.y - tTitlePanelLeftSize.y - 20.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("AnimalNameLeftPanel", pLayer);	//	동물 이름 판넬을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(650.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tNamePanelLeftSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x, tPos.y + tSize.y + 20.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		Text* pAnimalName = Obj::CreateObj<Text>("AnimalName", pLayer);

		pAnimalName->SetText(TEXT("Chicken"));

		pAnimalName->SetColor(102, 27, 27);

		pAnimalName->SetPos(tPos.x + tOffset.x + 10.f, tPos.y + tSize.y + 20.f + tOffset.y);

		pAnimalName->SetEnable(false);

		m_vecAnimalDescription.push_back(pAnimalName);

		pPanel = Obj::CreateObj<UIPanel>("AnimalNamePanel", pLayer);	//	동물 이름 판넬을 생성한다.

		pPanel->SetSize(154.f, 34.f);
		pPanel->SetImageOffset(1203.f, 978.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tNamePanelSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x+ tNamePanelLeftSize.x, tPos.y + tSize.y + 20.f + 2.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("AnimalNameRightPanel", pLayer);	//	동물 이름 오른쪽을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(675.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);
		pPanel->SetPos(tPos.x + tNamePanelLeftSize.x + tNamePanelSize.x, tPos.y + tSize.y + 20.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("MaterialLeftPanel", pLayer);	//	재료 판넬 왼쪽을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(650.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tMaterialPanelLeftSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x + 50.f, tPos.y + tSize.y + tNamePanelSize.y + 40.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		Text* pText = Obj::CreateObj<Text>("MaterialText", pLayer);

		pText->SetText(TEXT("400g"));

		pText->SetPos(tPos.x + 50.f  + tOffset.x + pPanel->GetSize().x + 10.f, tPos.y + tSize.y + tNamePanelSize.y + 40.f + tOffset.y);

		pText->SetEnable(false);

		m_vecAnimalDescription.push_back(pText);

		pPanel = Obj::CreateObj<UIPanel>("MaterialPanel", pLayer);	//	재료 판넬을 생성한다.

		pPanel->SetSize(122.f, 34.f);
		pPanel->SetImageOffset(1203.f, 978.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		POSITION tMaterialPanelSize = pPanel->GetSize();

		pPanel->SetPos(tPos.x + 50.f + tMaterialPanelLeftSize.x, tPos.y + tSize.y + tNamePanelSize.y + 40.f + 2.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("MaterialRightPanel", pLayer);	//	재료 판넬 오른쪽을 생성한다.

		pPanel->SetSize(25.f, 36.f);
		pPanel->SetImageOffset(675.f, 636.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);

		pPanel->SetPos(tPos.x + 50.f + tMaterialPanelLeftSize.x + tMaterialPanelSize.x, tPos.y + tSize.y + tNamePanelSize.y + 40.f);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("Icon", pLayer);	//	아이콘을 생성한다.

		pPanel->SetTexture("Mouse");

		pPanel->SetImageOffset(386.f, 746.f);

		pPanel->SetEnable(false);

		pPanel->SetSize(18.f, 18.f);

		pPanel->SetPos(tPos.x + 50.f + tOffset.x + 4.f, tPos.y + tSize.y + tNamePanelSize.y + 40.f + tOffset.y);

		m_vecPanel.push_back(pPanel);
	}

	else
	{
		size_t iButtonSize = m_vecButton.size();

		for (size_t i = 0; i < iButtonSize; ++i)
			m_vecButton[i]->SetEnable(true);
	}
}

void UIAnimalShop::InitPanel()
{
}

void UIAnimalShop::DisablePanel(int iNum, float fTime)
{
	m_bShopUIOn = false;

	size_t iBtnSize = m_vecButton.size();

	for (size_t i = 0; i < iBtnSize; ++i)
		m_vecButton[i]->SetEnable(false);

	Obj* pObj = FindObject("Haley");

	pObj->SetAnimationCurrentClip("HaleyLike");
	((NPC*)pObj)->Walk();

	SAFE_RELEASE(pObj);

	Die();
}

void UIAnimalShop::AnimalInfoOn(int iIndex, float fTime)
{
	POSITION tMousePos = GET_SINGLE(Input)->GetMouseClientPos();

	PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindAnimalInfo(iIndex);

	if (m_pInven)
		m_pInven->InfoPanelOn(tMousePos, *pInfo);

	char strName[256] = {};

	strcpy_s(strName, pInfo->strName.c_str());

	TCHAR pName[256] = {};

#ifdef _UNICODE
	int iLength = MultiByteToWideChar(CP_ACP, NULL, strName, -1, pName, 0);
	MultiByteToWideChar(CP_ACP, NULL, strName, -1, pName, iLength);
#else
	strcpy_s(pName, strName);
#endif

	m_vecAnimalDescription[1]->SetText(pName);

	TCHAR strPrice[32] = {};

	swprintf_s(strPrice, TEXT("%d"), pInfo->vecPrice[0]);

	m_vecAnimalDescription[2]->SetText(strPrice);
}

void UIAnimalShop::BuyAnimal(int iIndex, float fTime)
{
	PITEMINFO pInfo  = GET_SINGLE(ResourcesManager)->FindAnimalInfo(iIndex);

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	if (pInfo->vecPrice[0] <= pPlayer->GetGold())
	{
		pPlayer->BuyAnimal(ANIMAL_TYPE(iIndex));

		pPlayer->AddGold(-pInfo->vecPrice[0]);
	}

	SAFE_RELEASE(pPlayer);
}

void UIAnimalShop::AnimalInfoOff(int iIndex, float fTime)
{
	if (m_pInven)
		m_pInven->InfoPanelOff();
}
