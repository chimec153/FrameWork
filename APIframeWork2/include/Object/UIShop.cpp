#include "UIShop.h"
#include "UIButton.h"
#include "UIPanel.h"
#include "Text.h"
#include "UITilePanel.h"
#include "UIContext.h"
#include "../Resources/ResourcesManager.h"
#include "../Scene/Scene.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "Item.h"
#include "Player.h"
#include "ObjManager.h"
#include "UIInventory.h"

UIShop::UIShop() :
	m_bShopUIOn(false),
	m_pShopPanel(nullptr),
	m_pShopGoldPanel(nullptr),
	m_pShopBackPanel(nullptr),
	m_pShopExitBtn(nullptr),
	m_pShopUpBtn(nullptr),
	m_pShopDownBtn(nullptr),
	m_pShopScrollBtn(nullptr),
	m_iPage(0),
	m_pShopGoldText(nullptr),
	m_pShopPacePanel(nullptr),
	m_pShopCommentBack(nullptr),
	m_pInven(nullptr)
{
}

UIShop::UIShop(const UIShop& shop)
{
}

UIShop::~UIShop()
{
	Safe_Release_VecList(m_vecShopPanel);
	Safe_Release_VecList(m_vecShopItemPanel);
	Safe_Release_VecList(m_vecShopCommentPanel);
	Safe_Release_VecList(m_vecShopComment);
	SAFE_RELEASE(m_pShopPanel);
	SAFE_RELEASE(m_pShopGoldPanel);
	SAFE_RELEASE(m_pShopBackPanel);
	SAFE_RELEASE(m_pShopExitBtn);
	SAFE_RELEASE(m_pShopUpBtn);
	SAFE_RELEASE(m_pShopDownBtn);
	SAFE_RELEASE(m_pShopScrollBtn);
	SAFE_RELEASE(m_pShopGoldText);
	SAFE_RELEASE(m_pShopPacePanel);
	SAFE_RELEASE(m_pShopCommentBack);
}

POSITION UIShop::GetShopPanelPos() const
{
	if (!m_pShopBackPanel)
		return POSITION::Zero;

	return m_pShopBackPanel->GetPos();
}

bool UIShop::Init()
{
	return true;
}

void UIShop::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIShop::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	if (m_bShopUIOn)	//	상점 판넬이 열렸을 경우이다.
	{
		POSITION tPos = m_pShopPanel->GetPos();

		POSITION tSize = m_pShopPanel->GetSize();

		tPos.x += tSize.x + 22.f;

		tPos.x += (m_pShopUpBtn->GetSize().x - m_pShopScrollBtn->GetSize().x) / 2.f;

		float fScrollPosY = m_pShopScrollBtn->GetPos().y;

		float fScrollSizeY = m_pShopScrollBtn->GetSize().y;

		float fUpBtnPosY = m_pShopUpBtn->GetPos().y;

		float fUpBtnSizeY = m_pShopUpBtn->GetSize().y;

		float fDownBtnPosY = m_pShopDownBtn->GetPos().y;

		if (fScrollPosY > fDownBtnPosY - fScrollSizeY)
			fScrollPosY = fDownBtnPosY - fScrollSizeY;

		else if (fScrollPosY < fUpBtnPosY + fUpBtnSizeY)
			fScrollPosY = fUpBtnPosY + fUpBtnSizeY;

		m_pShopScrollBtn->SetPos(tPos.x, fScrollPosY);

		PSHOPINFO pShopInfo = GET_SINGLE(ResourcesManager)->FindShopInfo(m_strNPCName);

		int iCount = pShopInfo->iItemCount;

		m_iPage = (int)((iCount - SHOP_PAGE) * (fScrollPosY - (fUpBtnPosY + fUpBtnSizeY)) / ((fDownBtnPosY - fScrollSizeY) - (fUpBtnPosY + fUpBtnSizeY)));

		if (m_iPage < 0)
			m_iPage = 0;

		size_t iSize = m_vecShopItemPanel.size();

		for (size_t i = 0; i < iSize; ++i)
		{
			if (pShopInfo->iItemCount + pShopInfo->iItemPanelStartIndex <= i)
				m_vecShopItemPanel[i]->SetEnable(false);

			else if (m_iPage <= i - pShopInfo->iItemPanelStartIndex && i - pShopInfo->iItemPanelStartIndex < m_iPage + SHOP_PAGE)
				m_vecShopItemPanel[i]->SetEnable(true);

			else
				m_vecShopItemPanel[i]->SetEnable(false);
		}

		UpdateShopItemPos(fDeltaTime);
	}

	return 0;
}

int UIShop::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIShop::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIShop::Render(HDC hDC, float fDeltaTime)
{
	if (m_bShopUIOn)	//	상점 창이 열린 경우다.
	{
		m_pShopBackPanel->Render(hDC, fDeltaTime);
		m_pShopPanel->Render(hDC, fDeltaTime);

		size_t iSize = m_vecShopPanel.size();

		for (size_t i = 0; i < iSize; ++i)
			m_vecShopPanel[i]->Render(hDC, fDeltaTime);

		size_t iContextSize = m_vecShopItemPanel.size();

		PSHOPINFO pShopInfo = GET_SINGLE(ResourcesManager)->FindShopInfo(m_strNPCName);

		for (size_t i = 0; i < SHOP_PAGE; ++i)
		{
			if (pShopInfo->iItemCount <= i)
				continue;

			if (iContextSize > i + m_iPage + pShopInfo->iItemPanelStartIndex)
				m_vecShopItemPanel[i + m_iPage + pShopInfo->iItemPanelStartIndex]->Render(hDC, fDeltaTime);
		}

		m_pShopGoldPanel->Render(hDC, fDeltaTime);

		m_pShopExitBtn->Render(hDC, fDeltaTime);

		m_pShopUpBtn->Render(hDC, fDeltaTime);

		m_pShopDownBtn->Render(hDC, fDeltaTime);

		m_pShopScrollBtn->Render(hDC, fDeltaTime);

		m_pShopPacePanel->Render(hDC, fDeltaTime);

		m_pShopCommentBack->Render(hDC, fDeltaTime);

		iSize = m_vecShopCommentPanel.size();

		for (size_t i = 0; i < iSize; ++i)
			m_vecShopCommentPanel[i]->Render(hDC, fDeltaTime);

		size_t iCommentSize = m_vecShopComment.size();

		for (size_t i = 0; i < iCommentSize; ++i)
			m_vecShopComment[i]->Render(hDC, fDeltaTime);
	}

	UI::Render(hDC, fDeltaTime);
}

UIShop* UIShop::Clone()
{
	return new UIShop(*this);
}

void UIShop::DisableShopPanel(int iNum, float fTime)
{
	if (!m_bShopUIOn)
		return;

	m_pShopExitBtn->SetEnable(false);

	m_pShopUpBtn->SetEnable(false);

	m_pShopDownBtn->SetEnable(false);

	m_pShopScrollBtn->SetEnable(false);

	m_pShopGoldText->SetEnable(false);

	size_t iSize = m_vecShopItemPanel.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecShopItemPanel[i]->SetEnable(false);
/*
	for (int i = 0; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i])
			m_vecItem[i]->SetAlpha(255);
	}*/

	m_bShopUIOn = false;
}

void UIShop::UpdateShopItemPos(float fTime)
{
	size_t iSize = m_vecShopItemPanel.size();

	POSITION tBackPanelPos = m_pShopPanel->GetPos();

	PSHOPINFO pShopInfo = GET_SINGLE(ResourcesManager)->FindShopInfo(m_strNPCName);

	for (size_t i = 0; i < iSize; ++i)	//	아이템 목록들의 위치를 업데이트 한다.
	{
		POSITION tPanelPos = tBackPanelPos;

		tPanelPos.y += (i - m_iPage - pShopInfo->iItemPanelStartIndex) * 32.f;

		m_vecShopItemPanel[i]->SetPos(tPanelPos);
		m_vecShopItemPanel[i]->Update(fTime);
	}
}

void UIShop::UpdateScrollBtnPos()
{
	POSITION tPos = m_pShopScrollBtn->GetPos();

	float fScrollPosY = m_pShopScrollBtn->GetPos().y;

	float fScrollSizeY = m_pShopScrollBtn->GetSize().y;

	float fUpBtnPosY = m_pShopUpBtn->GetPos().y;

	float fUpBtnSizeY = m_pShopUpBtn->GetSize().y;

	float fDownBtnPosY = m_pShopDownBtn->GetPos().y;

	if (fScrollPosY > fDownBtnPosY - fScrollSizeY)
		fScrollPosY = fDownBtnPosY - fScrollSizeY;

	else if (fScrollPosY < fUpBtnPosY + fUpBtnSizeY)
		fScrollPosY = fUpBtnPosY + fUpBtnSizeY;

	m_pShopScrollBtn->SetPos(tPos.x, fScrollPosY);

	size_t iSize = m_vecShopItemPanel.size();

	fScrollPosY = (m_iPage * ((fDownBtnPosY - fScrollSizeY) - (fUpBtnPosY + fUpBtnSizeY))) / (iSize - SHOP_PAGE) + (fUpBtnPosY + fUpBtnSizeY);

	m_pShopScrollBtn->SetPos(m_pShopUpBtn->GetPos().x, fScrollPosY);
}

void UIShop::CreateShopPanel(const string& strName)
{
	m_bShopUIOn = true;

	if (!m_pShopPanel)
	{
		int iTileNumX = 17;
		int iTileNumY = SHOP_PAGE + 1;

		Layer* pLayer = m_pScene->FindLayer("Default");

		m_pShopPanel = CreateObj<UIPanel>("ShopPanel", pLayer);	//	상점 배경을 만든다.

		m_pShopPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

		m_pShopPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

		POSITION tSize = m_pShopPanel->GetSize();

		m_pShopPanel->SetPos(tRS.iW / 2.f - tSize.x / 2.f, tRS.iH / 2.f - (tSize.y + 96.f) / 2.f);

		m_pShopPanel->SetEnable(false);

		m_vecShopPanel.reserve(132);

		POSITION tPos = m_pShopPanel->GetPos();

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

					m_vecShopPanel.push_back(pPanel);
				}
			}
		}

		int iCount = GET_SINGLE(ResourcesManager)->GetItemCount();

		for (int i = 0; i < iCount; ++i)		//	아이템 목록을 만든다.
		{
			UIContext* pPanel = Obj::CreateObj<UIContext>("ShopContext", pLayer, POSITION::Zero, POSITION(32.f * 16.f, 32.f));

			pPanel->SetPos(0.f, 32.f * i);
			pPanel->SetIndex(i);
			pPanel->SetEnable(true);
			pPanel->SetInven(m_pInven);

			m_vecShopItemPanel.push_back(pPanel);
		}

		m_pShopGoldPanel = Obj::CreateObj<UIPanel>("GoldPanel", pLayer);	//	플레이어 소지금 판넬을 만든다.

		m_pShopGoldPanel->SetTexture("Mouse");
		m_pShopGoldPanel->SetColorKey(255, 255, 255);
		m_pShopGoldPanel->SetSize(130.f, 34.f);
		m_pShopGoldPanel->SetImageOffset(680.f, 944.f);

		tPos = m_pShopPanel->GetPos();

		tPos.x -= 16.f;
		tPos.y += m_pShopPanel->GetSize().y + 8.f;

		m_pShopGoldPanel->SetPos(tPos);
		m_pShopGoldPanel->SetAlpha(255);
		m_pShopGoldPanel->EnableAlpha(true);
		m_pShopGoldPanel->SetEnable(false);

		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		m_pShopGoldText = Obj::CreateObj<Text>("ShopGoldText", pHUDLayer);

		m_pShopGoldText->SetPos(tPos.x + 20.f, tPos.y + 8.f);

		//m_pShopGoldText->SetText(m_pGoldText->GetText());

		iTileNumX = 13;
		iTileNumY = 4;

		m_pShopBackPanel = Obj::CreateObj<UIPanel>("ShopBackPanel", pLayer);	//	상점용 인벤토리 배경 판을 만든다.

		m_pShopBackPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		tPos.x += m_pShopGoldPanel->GetSize().x + 12.f;
		tPos.y += 12.f;

		m_pShopBackPanel->SetTexture("backPanel");
		m_pShopBackPanel->SetPos(tPos);
		m_pShopBackPanel->SetEnable(false);

		tPos = m_pShopBackPanel->GetPos();

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

					m_vecShopPanel.push_back(pPanel);
				}
			}
		}

		for (int j = 0; j < 3; ++j)	//	아이템들의 틀을 만든다.
		{
			for (int i = 0; i < 12; ++i)
			{
				UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

				pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

				pPanel->SetColorKey(255, 255, 255);

				pPanel->SetSize(32.f, 32.f);

				pPanel->SetPos(32.f * i + tPos.x, 32.f * j + tPos.y);

				pPanel->SetImageOffset(64.f, 64.f);

				pPanel->SetEnable(false);

				m_vecShopPanel.push_back(pPanel);
			}
		}

		tPos = m_pShopPanel->GetPos();

		tPos.x += tSize.x + 22.f;
		tPos.y -= 26.f;

		m_pShopExitBtn = Obj::CreateObj<UIButton>("ShopExitButton", pLayer);	//	상점 판넬 종료 버튼을 생성한다.

		m_pShopExitBtn->SetSize(22.f, 22.f);
		m_pShopExitBtn->SetImageOffset(676.f, 988.f);
		m_pShopExitBtn->SetTexture("Mouse");
		m_pShopExitBtn->SetCallback(this, &UIShop::DisableShopPanel);
		m_pShopExitBtn->SetAlpha(255);
		m_pShopExitBtn->EnableAlpha(true);
		m_pShopExitBtn->SetPos(tPos);
		m_pShopExitBtn->SetEnable(true);

		ColliderRect* pExitButtonRC = (ColliderRect*)m_pShopExitBtn->GetCollider("ButtonBody");

		pExitButtonRC->SetRect(0.f, 0.f, 22.f, 22.f);

		SAFE_RELEASE(pExitButtonRC);

		tPos.y += 26.f;

		m_pShopUpBtn = Obj::CreateObj<UIButton>("ShopUpButton", pLayer);	//	상점 판넬 이전 페이지 버튼을 생성한다.

		m_pShopUpBtn->SetSize(20.f, 22.f);
		m_pShopUpBtn->SetImageOffset(844.f, 918.f);
		m_pShopUpBtn->SetTexture("Mouse");
		m_pShopUpBtn->SetCallback(this, &UIShop::ShopPageUp);
		m_pShopUpBtn->SetAlpha(255);
		m_pShopUpBtn->EnableAlpha(true);
		m_pShopUpBtn->SetPos(tPos);
		m_pShopUpBtn->SetEnable(true);

		ColliderRect* pUpButtonRC = (ColliderRect*)m_pShopUpBtn->GetCollider("ButtonBody");

		pUpButtonRC->SetRect(0.f, 0.f, 20.f, 22.f);

		SAFE_RELEASE(pUpButtonRC);

		tPos.y += tSize.y + m_pShopBackPanel->GetSize().y + 8.f;

		m_pShopDownBtn = Obj::CreateObj<UIButton>("ShopDownButton", pLayer);	//	상점 판넬 다음 페이지 버튼을 생성한다.

		m_pShopDownBtn->SetSize(20.f, 22.f);
		m_pShopDownBtn->SetImageOffset(844.f, 944.f);
		m_pShopDownBtn->SetTexture("Mouse");
		m_pShopDownBtn->SetCallback(this, &UIShop::ShopPageDown);
		m_pShopDownBtn->SetAlpha(255);
		m_pShopDownBtn->EnableAlpha(true);
		m_pShopDownBtn->SetPos(tPos);
		m_pShopDownBtn->SetEnable(true);

		ColliderRect* pDownButtonRC = (ColliderRect*)m_pShopDownBtn->GetCollider("ButtonBody");

		pDownButtonRC->SetRect(0.f, 0.f, 20.f, 22.f);

		SAFE_RELEASE(pDownButtonRC);

		tPos.y -= tSize.y - 22.f;

		m_pShopScrollBtn = Obj::CreateObj<UITilePanel>("ShopScrollButton", pLayer,
			POSITION::Zero, POSITION(12.f, 20.f));	//	상점 판넬 스크롤 버튼을 생성한다.

		tPos.x += (m_pShopUpBtn->GetSize().x - m_pShopScrollBtn->GetSize().x) / 2.f;

		m_pShopScrollBtn->SetImageOffset(870.f, 926.f);
		m_pShopScrollBtn->SetTexture("Mouse");
		m_pShopScrollBtn->SetAlpha(255);
		m_pShopScrollBtn->EnableAlpha(true);
		m_pShopScrollBtn->SetPos(tPos);
		m_pShopScrollBtn->SetEnable(true);

		m_pShopPacePanel = Obj::CreateObj<UIPanel>("ShopPace", pLayer);	//	상점 얼굴 판넬을 생성한다.

		m_pShopPacePanel->SetSize(148.f, 148.f);
		m_pShopPacePanel->SetTexture("Harvey", TEXT("Characters\\NPC\\HarveyAll.bmp"));
		m_pShopPacePanel->SetImageOffset(216.f, 2.f);

		POSITION tPaceSize = m_pShopPacePanel->GetSize();

		POSITION tShopPanelPos = m_pShopPanel->GetPos();

		tShopPanelPos.x -= tPaceSize.x + 10.f;

		m_pShopPacePanel->SetPos(tShopPanelPos);

		m_pShopPacePanel->SetEnable(false);

		m_pShopPacePanel->SetColorKey(255, 255, 255);

		m_pShopPacePanel->EnableAlpha(true);

		m_pShopPacePanel->SetAlpha(255);

		iTileNumX = 5;
		iTileNumY = 5;

		tShopPanelPos.y += tPaceSize.y + 10.f;

		m_pShopCommentBack = Obj::CreateObj<UIPanel>("ShopCommentBack", pLayer);

		m_pShopCommentBack->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

		m_pShopCommentBack->SetTexture("backPanel");

		m_pShopCommentBack->SetEnable(false);

		m_pShopCommentBack->SetPos(tShopPanelPos);

		for (int i = 0; i < iTileNumY; ++i)	//	상점 코멘트 사각 틀을 만든다.
		{
			for (int j = 0; j < iTileNumX; ++j)
			{
				if (i == 0 || i == iTileNumY - 1 || j == 0 || j == iTileNumX - 1)
				{
					UIPanel* pPanel = CreateObj<UIPanel>("Panel", pLayer);

					pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

					pPanel->SetColorKey(255, 255, 255);

					pPanel->SetSize(32.f, 32.f);

					pPanel->SetPos(32.f * j + tShopPanelPos.x - 16.f, 32.f * i + tShopPanelPos.y - 16.f);

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

					m_vecShopCommentPanel.push_back(pPanel);
				}
			}
		}

		tShopPanelPos += 4.f;

		for (int i = 0; i < 4; ++i)
		{
			Text* pComment = Obj::CreateObj<Text>("ShopComment", pLayer);

			pComment->SetPos(tShopPanelPos.x, tShopPanelPos.y + 20.f * i);

			pComment->SetColor(102, 27, 27);

			pComment->SetFont("SmallFont");

			pComment->SetEnable(false);

			m_vecShopComment.push_back(pComment);
		}
	}

	InitShopPanel(strName);
}

void UIShop::InitShopPanel(const string& strName)
{
	m_strNPCName = strName;

	m_pShopExitBtn->SetEnable(true);

	m_pShopUpBtn->SetEnable(true);

	m_pShopDownBtn->SetEnable(true);

	m_pShopScrollBtn->SetEnable(true);

	m_pShopGoldText->SetEnable(true);

	PSHOPINFO pInfo = GET_SINGLE(ResourcesManager)->FindShopInfo(strName);

	int iRand = rand() % pInfo->iSize;

	for (int i = 0; i < pInfo->iLine; ++i)
		m_vecShopComment[i]->SetText(pInfo->vecText[i + iRand * pInfo->iLine]);

	m_pShopPacePanel->SetTexture(strName);

	if (strName == "Harvey")
		m_pShopPacePanel->SetImageOffset(216.f, 2.f);

	else
		m_pShopPacePanel->SetImageOffset(216.f, 762.f);

	return;
}

void UIShop::ShopPageUp(int iNum, float fTime)
{
	size_t iSize = m_vecShopItemPanel.size();

	--m_iPage;

	if (0 > m_iPage)
		++m_iPage;

	else
	{
		UpdateScrollBtnPos();

		UpdateShopItemPos(fTime);
	}
}

void UIShop::ShopPageDown(int iNum, float fTime)
{
	size_t iSize = m_vecShopItemPanel.size();

	++m_iPage;

	if (iSize - SHOP_PAGE < m_iPage)
		--m_iPage;

	else
	{
		UpdateScrollBtnPos();

		UpdateShopItemPos(fTime);
	}
}

void UIShop::BuyItem(int iIndex)
{
	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	int iGold = pPlayer->GetGold();

	int iCount = m_pInven->GetItemCount();

	PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(iIndex);

	if (iGold < pInfo->vecPrice[0])
	{
		SAFE_RELEASE(pPlayer);
		return;
	}

	else if (iCount >= INVENTORY_MAX)
	{
		SAFE_RELEASE(pPlayer);
		return;
	}

	pPlayer->AddGold(-pInfo->vecPrice[0]);

	Layer* pLayer = m_pScene->FindLayer("Default");

	Item* pItem = (Item*)CreateCloneObj(pInfo->strName, pInfo->strName, pLayer);

	m_pInven->AddItem(pItem);

	Collider* pCol = pItem->GetCollider("ItemBody");

	pCol->AddCollisionFunction(CS_ENTER, pItem, &Item::CollEnter);
	pCol->AddCollisionFunction(CS_STAY, pItem, &Item::ColStay);
	pCol->AddCollisionFunction(CS_LEAVE, pItem, &Item::ColEnd);

	SAFE_RELEASE(pCol);

	SAFE_RELEASE(pPlayer);

	SAFE_RELEASE(pItem);

	PSHOPINFO pShopInfo = GET_SINGLE(ResourcesManager)->FindShopInfo(m_strNPCName);

	if (pShopInfo->iBuyMessageCount != 0)
	{
		m_pShopPacePanel->SetImageOffset(pShopInfo->tBuyImageOffset);

		for (int i = 0; i < pShopInfo->iLine; ++i)
			m_vecShopComment[i]->SetText(pShopInfo->vecText[pShopInfo->iLine * pShopInfo->iSize + i]);
	}
}

void UIShop::AddObjectToLayer(Layer* pLayer)
{
	size_t iPanelSize = m_vecShopItemPanel.size();

	for (size_t i = 0; i < iPanelSize; ++i)
	{
		if (!pLayer->HasObject(m_vecShopItemPanel[i]))
			pLayer->AddObject(m_vecShopItemPanel[i]);
	}

	if (m_pShopDownBtn)
	{
		if (!pLayer->HasObject(m_pShopDownBtn))
			pLayer->AddObject(m_pShopDownBtn);
	}

	if (m_pShopExitBtn)
	{
		if (!pLayer->HasObject(m_pShopExitBtn))
			pLayer->AddObject(m_pShopExitBtn);
	}

	if (m_pShopUpBtn)
	{
		if (!pLayer->HasObject(m_pShopUpBtn))
			pLayer->AddObject(m_pShopUpBtn);
	}

	if (m_pShopScrollBtn)
	{
		if (!pLayer->HasObject(m_pShopScrollBtn))
			pLayer->AddObject(m_pShopScrollBtn);
	}
}

void UIShop::SetObjectLayer(Layer* pLayer)
{
	Scene* pScene = pLayer->GetScene();

	if (m_pShopDownBtn)
	{
		m_pShopDownBtn->SetLayer(pLayer);
		m_pShopDownBtn->SetScene(pScene);
	}

	if (m_pShopExitBtn)
	{
		m_pShopExitBtn->SetLayer(pLayer);
		m_pShopExitBtn->SetScene(pScene);
	}

	if (m_pShopUpBtn)
	{
		m_pShopUpBtn->SetLayer(pLayer);
		m_pShopUpBtn->SetScene(pScene);
	}

	if (m_pShopScrollBtn)
	{
		m_pShopScrollBtn->SetLayer(pLayer);
		m_pShopScrollBtn->SetScene(pScene);
	}

	size_t iSize = m_vecShopItemPanel.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecShopItemPanel[i]->SetLayer(pLayer);
		m_vecShopItemPanel[i]->SetScene(pScene);
	}
}

void UIShop::SetGoldText(const TCHAR* strNum)
{
	if (m_pShopGoldText)
		m_pShopGoldText->SetText(strNum);
}
