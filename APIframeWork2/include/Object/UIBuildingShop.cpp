#include "UIBuildingShop.h"
#include "UIButton.h"
#include "UIPanel.h"
#include "Text.h"
#include "../Scene/Scene.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "UINum.h"
#include "ObjManager.h"
#include "Player.h"

UIBuildingShop::UIBuildingShop()	:
	m_bShopUIOn(false),
	m_pInven(nullptr),
	m_pPrevBtn(nullptr),
	m_pNextBtn(nullptr),
	m_pExitBtn(nullptr),
	m_pBuildBtn(nullptr),
	m_pBuildingName(nullptr),
	m_iPage(0)
{
}

UIBuildingShop::UIBuildingShop(const UIBuildingShop& buildshop)	:
	UI(buildshop)
{
	m_bShopUIOn = buildshop.m_bShopUIOn;
	m_pInven = buildshop.m_pInven;
	m_pPrevBtn = buildshop.m_pPrevBtn;
	m_pNextBtn = buildshop.m_pNextBtn;
	m_pExitBtn = buildshop.m_pExitBtn;
	m_pBuildBtn = buildshop.m_pBuildBtn;
	m_pBuildingName = buildshop.m_pBuildingName;
	m_iPage = buildshop.m_iPage;
}

UIBuildingShop::~UIBuildingShop()
{
	DisablePanel(0, 0.f);
}

bool UIBuildingShop::Init()
{
	return true;
}

void UIBuildingShop::Input(float fDeltaTime)
{
}

int UIBuildingShop::Update(float fDeltaTime)
{
	if (m_pPrevBtn)
		m_pPrevBtn->Update(fDeltaTime);

	if (m_pNextBtn)
		m_pNextBtn->Update(fDeltaTime);

	if (m_pBuildBtn)
		m_pBuildBtn->Update(fDeltaTime);

	if (m_pExitBtn)
		m_pExitBtn->Update(fDeltaTime);

	return 0;
}

int UIBuildingShop::LateUpdate(float fDeltaTime)
{
	if (m_pPrevBtn)
		m_pPrevBtn->LateUpdate(fDeltaTime);

	if (m_pNextBtn)
		m_pNextBtn->LateUpdate(fDeltaTime);

	if (m_pBuildBtn)
		m_pBuildBtn->LateUpdate(fDeltaTime);

	if (m_pExitBtn)
		m_pExitBtn->LateUpdate(fDeltaTime);

	return 0;
}

void UIBuildingShop::Collision(float fDeltaTime)
{
}

void UIBuildingShop::Render(HDC hDC, float fDeltaTime)
{
	size_t iSize = m_vecPanel.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecPanel[i]->Render(hDC, fDeltaTime);

	size_t iTextSize = m_vecMaterialText.size();

	for (size_t i = 0; i < iTextSize; ++i)
		m_vecMaterialText[i]->Render(hDC, fDeltaTime);

	if (m_pPrevBtn)
		m_pPrevBtn->Render(hDC, fDeltaTime);

	if (m_pNextBtn)
		m_pNextBtn->Render(hDC, fDeltaTime);

	if (m_pBuildBtn)
		m_pBuildBtn->Render(hDC, fDeltaTime);

	if (m_pExitBtn)
		m_pExitBtn->Render(hDC, fDeltaTime);

	if (m_pBuildingName)
		m_pBuildingName->Render(hDC, fDeltaTime);

	size_t iBuildingTextSize = m_vecBuildingText.size();

	for (size_t i = 0; i < iBuildingTextSize; ++i)
		m_vecBuildingText[i]->Render(hDC, fDeltaTime);

	UI::Render(hDC, fDeltaTime);
}

UIBuildingShop* UIBuildingShop::Clone()
{
	return new UIBuildingShop(*this);
}

void UIBuildingShop::CreatePanel()
{
	m_bShopUIOn = true;

	if (!m_pNextBtn)
	{
		int iTileNumX = 10;
		int iTileNumY = 10;

		Layer* pLayer = m_pScene->FindLayer("Default");

		UIPanel* pPanel = CreateObj<UIPanel>("BuildingPanel", pLayer);	//	상점 배경을 만든다.

		pPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

		pPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

		POSITION tSize = pPanel->GetSize();

		pPanel->SetPos(tRS.iW / 2.f - (tSize.x*2) / 2.f, tRS.iH / 2.f - tSize.y / 2.f);

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

		POSITION tPanelPos = pPanel->GetPos();

		POSITION tPanelSize = pPanel->GetSize();

		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		iTileNumX = 8;
		iTileNumY = 8;

		pPanel = Obj::CreateObj<UIPanel>("MaterialPanel", pLayer);	//	건물 재료 배경 판을 만든다.

		pPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		pPanel->SetTexture("backPanel");
		pPanel->SetPos(tPanelPos.x + tPanelSize.x + 18.f, tPanelPos.y + 48.f);
		pPanel->SetEnable(false);

		POSITION tMaterialPanelPos = pPanel->GetPos();

		POSITION tMaterialPanelSize = pPanel->GetSize();

		m_vecPanel.push_back(pPanel);

		tPos = pPanel->GetPos();

		for (int i = 0; i < iTileNumY; ++i)	//	상점용 인벤토리 사각 틀을 만든다.
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

		m_pExitBtn = Obj::CreateObj<UIButton>("ExitButton", pLayer);	//	상점 판넬 종료 버튼을 생성한다.

		m_pExitBtn->SetSize(22.f, 22.f);
		m_pExitBtn->SetImageOffset(676.f, 988.f);
		m_pExitBtn->SetTexture("Mouse");
		m_pExitBtn->SetCallback(this, &UIBuildingShop::DisablePanel);
		m_pExitBtn->SetAlpha(255);
		m_pExitBtn->EnableAlpha(true);
		m_pExitBtn->SetPos(tPanelPos.x + tPanelSize.x + tMaterialPanelSize.x, tPanelPos.y);
		m_pExitBtn->SetEnable(true);

		ColliderRect* pExitButtonRC = (ColliderRect*)m_pExitBtn->GetCollider("ButtonBody");

		pExitButtonRC->SetRect(0.f, 0.f, 22.f, 22.f);

		SAFE_RELEASE(pExitButtonRC);

		m_pPrevBtn = Obj::CreateObj<UIButton>("PrevButton", pLayer);	//	상점 판넬 이전 페이지 버튼을 생성한다.

		m_pPrevBtn->SetSize(22.f, 20.f);
		m_pPrevBtn->SetImageOffset(706.f, 990.f);
		m_pPrevBtn->SetTexture("Mouse");
		m_pPrevBtn->SetCallback(this, &UIBuildingShop::PagePrev);
		m_pPrevBtn->SetAlpha(255);
		m_pPrevBtn->EnableAlpha(true);
		m_pPrevBtn->SetPos(tPanelPos.x + tPanelSize.x/2.f - 25.f, tPanelPos.y + tPanelSize.y + m_pPrevBtn->GetSize().y);
		m_pPrevBtn->SetEnable(true);

		ColliderRect* pUpButtonRC = (ColliderRect*)m_pPrevBtn->GetCollider("ButtonBody");

		pUpButtonRC->SetRect(0.f, 0.f, 20.f, 22.f);

		SAFE_RELEASE(pUpButtonRC);

		m_pNextBtn = Obj::CreateObj<UIButton>("NextButton", pLayer);	//	상점 판넬 다음 페이지 버튼을 생성한다.

		m_pNextBtn->SetSize(22.f, 20.f);
		m_pNextBtn->SetImageOffset(732.f, 990.f);
		m_pNextBtn->SetTexture("Mouse");
		m_pNextBtn->SetCallback(this, &UIBuildingShop::PageNext);
		m_pNextBtn->SetAlpha(255);
		m_pNextBtn->EnableAlpha(true);
		m_pNextBtn->SetPos(tPanelPos.x + tPanelSize.x / 2.f + 25.f, tPanelPos.y + tPanelSize.y + m_pNextBtn->GetSize().y);
		m_pNextBtn->SetEnable(true);

		ColliderRect* pDownButtonRC = (ColliderRect*)m_pNextBtn->GetCollider("ButtonBody");

		pDownButtonRC->SetRect(0.f, 0.f, 20.f, 22.f);

		SAFE_RELEASE(pDownButtonRC);

		m_pBuildBtn = Obj::CreateObj<UIButton>("BuildButton", pLayer);	//	건물 짓기 버튼을 생성한다.

		m_pBuildBtn->SetSize(32.f, 32.f);
		m_pBuildBtn->SetImageOffset(732.f, 746.f);
		m_pBuildBtn->SetTexture("Mouse");
		m_pBuildBtn->SetCallback(this, &UIBuildingShop::BuyBuilding);
		m_pBuildBtn->SetAlpha(255);
		m_pBuildBtn->EnableAlpha(true);
		m_pBuildBtn->SetPos(tPanelPos.x + tPanelSize.x + 50.f, tPanelPos.y + tPanelSize.y);
		m_pBuildBtn->SetEnable(true);

		ColliderRect* pBuildButtonRC = (ColliderRect*)m_pBuildBtn->GetCollider("ButtonBody");

		pBuildButtonRC->SetRect(0.f, 0.f, 32.f, 32.f);

		SAFE_RELEASE(pBuildButtonRC);

		pPanel = Obj::CreateObj<UIPanel>("Building", pLayer);	//	건물 판넬을 생성한다.

		pPanel->SetSize(32.f * 6, 32.f * 7);
		pPanel->SetTexture("coop", TEXT("Buildings\\Coop.bmp"));

		pPanel->SetEnable(false);

		pPanel->SetColorKey(255, 255, 255);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255); 

		pPanel->SetPos(tPanelPos + 32.f);
		
		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("BuildingNamePanel", pLayer);	//	건물 이름 판넬을 생성한다.
		
		pPanel->SetSize(204.f, 36.f);
		pPanel->SetImageOffset(1178.f, 976.f);
		pPanel->SetTexture("Mouse");
		pPanel->SetEnable(false);
		pPanel->SetPos(tPanelPos.x + tPanelSize.x + 20.f, tPanelPos.y);

		pPanel->EnableAlpha(true);

		pPanel->SetAlpha(255);

		m_vecPanel.push_back(pPanel);

		m_pBuildingName = Obj::CreateObj<Text>("BuildingName", pLayer);

		m_pBuildingName->SetText(TEXT("Coop"));

		m_pBuildingName->SetColor(102, 27, 27);

		m_pBuildingName->SetPos(tPanelPos.x + tPanelSize.x + 32.f, tPanelPos.y + 12.f);

		m_pBuildingName->SetEnable(false);

		for (int i = 0; i < 4; ++i)
		{
			Text* pText = Obj::CreateObj<Text>("BuildingText", pLayer);

			pText->SetText(TEXT(""));

			pText->SetPos(tMaterialPanelPos.x + 4.f, tMaterialPanelPos.y + 20.f * i);

			pText->SetEnable(false);

			m_vecBuildingText.push_back(pText);
		}

		Text* pText = Obj::CreateObj<Text>("MaterialText", pLayer);

		pText->SetText(TEXT("4000g"));

		pText->SetPos(tMaterialPanelPos.x + 32.f, tMaterialPanelPos.y + 80.f);

		pText->SetEnable(false);

		m_vecMaterialText.push_back(pText);

		pText = Obj::CreateObj<Text>("MaterialText", pLayer);

		pText->SetText(TEXT("Wood"));

		pText->SetPos(tMaterialPanelPos.x + 32.f, tMaterialPanelPos.y + 112.f);

		pText->SetEnable(false);

		m_vecMaterialText.push_back(pText);

		pText = Obj::CreateObj<Text>("MaterialText", pLayer);

		pText->SetText(TEXT("Stone"));

		pText->SetPos(tMaterialPanelPos.x + 32.f, tMaterialPanelPos.y + 144.f);

		pText->SetEnable(false);

		m_vecMaterialText.push_back(pText);

		pPanel = Obj::CreateObj<UIPanel>("Icon", pLayer);

		pPanel->SetTexture("Mouse");

		pPanel->SetImageOffset(386.f, 746.f);

		pPanel->SetEnable(false);

		pPanel->SetSize(18.f, 18.f);

		pPanel->SetPos(tMaterialPanelPos.x + 5.f, tMaterialPanelPos.y + 80.f);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("Material", pLayer);

		pPanel->SetTexture("items");

		pPanel->SetImageOffset(32.f * 4, 32.f * 16);

		pPanel->SetEnable(false);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(tMaterialPanelPos.x, tMaterialPanelPos.y + 112.f);

		m_vecPanel.push_back(pPanel);

		pPanel = Obj::CreateObj<UIPanel>("Material", pLayer);

		pPanel->SetTexture("items");

		pPanel->SetImageOffset(32.f * 6, 32.f * 16);

		pPanel->SetEnable(false);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(tMaterialPanelPos.x, tMaterialPanelPos.y +144.f);

		m_vecPanel.push_back(pPanel);
	}

	PagePrev(0, 0.f);
}

void UIBuildingShop::InitPanel()
{
	m_pBuildingName->SetText(TEXT("Coop"));

	m_vecPanel[66]->SetSize(192.f, 224.f);
	m_vecPanel[66]->SetTexture("coop", TEXT("Buildings\\Coop.bmp"));

	m_vecBuildingText[0]->SetText(TEXT("Houses 4 coop-dwelling"));
	m_vecBuildingText[1]->SetText(TEXT("animals."));
	m_vecBuildingText[2]->SetText(TEXT(""));
	m_vecBuildingText[3]->SetText(TEXT(""));
}

void UIBuildingShop::PagePrev(int iNum, float fTime)
{
	--m_iPage;

	if (m_iPage < 0)
		++m_iPage;

	 if (m_iPage == 0)
	{
		m_pBuildingName->SetText(TEXT("Barn"));

		m_vecPanel[66]->SetSize(224.f, 224.f);
		m_vecPanel[66]->SetTexture("barn", TEXT("Buildings\\Barn.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("6000g"));

		m_vecBuildingText[0]->SetText(TEXT("Houses 4 barn-dwelling"));
		m_vecBuildingText[1]->SetText(TEXT("animals. Allows animals"));
		m_vecBuildingText[2]->SetText(TEXT("to give birth."));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}

	else if (m_iPage == 1)
	{
		m_pBuildingName->SetText(TEXT("Coop"));

		m_vecPanel[66]->SetSize(192.f, 224.f);
		m_vecPanel[66]->SetTexture("coop", TEXT("Buildings\\Coop.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("4000g"));

		m_vecBuildingText[0]->SetText(TEXT("Houses 4 coop-dwelling"));
		m_vecBuildingText[1]->SetText(TEXT("animals."));
		m_vecBuildingText[2]->SetText(TEXT(""));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}

	else if (m_iPage == 2)
	{
		m_pBuildingName->SetText(TEXT("Silo"));

		m_vecPanel[66]->SetSize(96.f, 256.f);
		m_vecPanel[66]->SetTexture("Silo", TEXT("Buildings\\Silo.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("100g"));

		m_vecBuildingText[0]->SetText(TEXT("Allows you to cut and"));
		m_vecBuildingText[1]->SetText(TEXT("store grass for feed."));
		m_vecBuildingText[2]->SetText(TEXT(""));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}
}

void UIBuildingShop::PageNext(int iNum, float fTime)
{
	++m_iPage;

	if (m_iPage == 4)
		--m_iPage;

	if (m_iPage == 1)
	{
		m_pBuildingName->SetText(TEXT("Coop"));

		m_vecPanel[66]->SetSize(192.f, 224.f);
		m_vecPanel[66]->SetTexture("coop", TEXT("Buildings\\Coop.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("4000g"));

		m_vecBuildingText[0]->SetText(TEXT("Houses 4 coop-dwelling"));
		m_vecBuildingText[1]->SetText(TEXT("animals."));
		m_vecBuildingText[2]->SetText(TEXT(""));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}

	else if (m_iPage == 2)
	{
		m_pBuildingName->SetText(TEXT("Silo"));

		m_vecPanel[66]->SetSize(96.f, 256.f);
		m_vecPanel[66]->SetTexture("Silo", TEXT("Buildings\\Silo.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("100g"));

		m_vecBuildingText[0]->SetText(TEXT("Allows you to cut and"));
		m_vecBuildingText[1]->SetText(TEXT("store grass for feed."));
		m_vecBuildingText[2]->SetText(TEXT(""));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}

	else if (m_iPage == 3)
	{
		m_pBuildingName->SetText(TEXT("Stable"));

		m_vecPanel[66]->SetSize(128.f, 192.f);
		m_vecPanel[66]->SetTexture("Stable", TEXT("Buildings\\Stable.bmp"));

		m_vecMaterialText[0]->SetText(TEXT("10000g"));

		m_vecBuildingText[0]->SetText(TEXT("Allows you to keep and"));
		m_vecBuildingText[1]->SetText(TEXT("ride a horse. Horse"));
		m_vecBuildingText[2]->SetText(TEXT("included."));
		m_vecBuildingText[3]->SetText(TEXT(""));
	}
}

void UIBuildingShop::DisablePanel(int iNum, float fTime)
{
	if (m_pNextBtn)
		m_pNextBtn->Die();

	if (m_pPrevBtn)
		m_pPrevBtn->Die();

	if (m_pExitBtn)
		m_pExitBtn->Die();

	if (m_pBuildBtn)
		m_pBuildBtn->Die();

	if (m_pBuildingName)
		m_pBuildingName->Die();

	size_t iSize = m_vecMaterialText.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecMaterialText[i]->Die();

	size_t iTextSize = m_vecBuildingText.size();

	for (size_t i = 0; i < iTextSize; ++i)
		m_vecBuildingText[i]->Die();

	SAFE_RELEASE(m_pNextBtn);
	SAFE_RELEASE(m_pPrevBtn);
	SAFE_RELEASE(m_pExitBtn);
	SAFE_RELEASE(m_pBuildBtn);
	SAFE_RELEASE(m_pBuildingName);
	Safe_Release_VecList(m_vecMaterialText);
	Safe_Release_VecList(m_vecBuildingText);
	Safe_Release_VecList(m_vecPanel);
	Safe_Release_VecList(m_vecMaterialNum);

	Die();
}

void UIBuildingShop::BuyBuilding(int iIndex, float fTime)
{
	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	int iGold = pPlayer->GetGold();

	int iCost = 0;

	if (m_iPage == 0)
		iCost = 6000;

	else if (m_iPage == 1)
		iCost = 4000;

	else if (m_iPage == 2)
		iCost = 100;

	else if (m_iPage == 3)
		iCost = 10000;

	if (iGold < iCost)
	{
		SAFE_RELEASE(pPlayer);
		return;
	}

	unsigned int iBuilding = 1;

	iBuilding <<= m_iPage;

	pPlayer->Build((BUILDING_TYPE)iBuilding);

	pPlayer->AddGold(-iCost);

	SAFE_RELEASE(pPlayer);
}

void UIBuildingShop::AddObjectToLayer(Layer* pLayer)
{
}

void UIBuildingShop::SetObjectLayer(Layer* pLayer)
{
}
