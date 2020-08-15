#include "UIInventory.h"
#include "Item.h"
#include "../Core/Camera.h"
#include "..//Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/UIPanel.h"
#include "..//Scene/Scene.h"
#include "../Object/Seed.h"
#include "../Object/Crop.h"
#include "../Core/Input.h"
#include "../Core.h"
#include "../Object/Text.h"
#include "../Object/UIButton.h"
#include "../Object/UITilePanel.h"
#include "../Collider/ColliderRect.h"

UIInventory::UIInventory()	:
	m_iCount(0),
	m_iCursor(0),
	m_pCursorTex(nullptr),
	m_pBackPanel(nullptr),
	m_iCountX(0),
	m_iCountY(0),
	m_pInventoryPanel(nullptr),
	m_pBackTexture(nullptr),
	m_bExtended(false),
	m_bShopUIOn(false),
	m_pShopPanel(nullptr),
	m_pShopValancePanel(nullptr),
	m_pShopBackPanel(nullptr),
	m_pShopExitBtn(nullptr),
	m_pShopUpBtn(nullptr),
	m_pShopDownBtn(nullptr),
	m_pShopScrollBtn(nullptr),
	m_iPage(0)
{
	m_vecItem.resize(36);

	memset(&m_vecItem[0], 0, 36 * sizeof(Item*));
}

UIInventory::~UIInventory()
{
	Safe_Release_VecList(m_vecItem);
	SAFE_RELEASE(m_pCursorTex);
	Safe_Release_VecList(m_vecPanel);
	SAFE_RELEASE(m_pBackPanel);
	SAFE_RELEASE(m_pInventoryPanel);
	SAFE_RELEASE(m_pBackTexture);
	SAFE_RELEASE(m_pShopPanel);
	Safe_Release_VecList(m_vecShopPanel);
	Safe_Delete_VecList(m_vecShopItemPanel);
	SAFE_RELEASE(m_pShopValancePanel);
	SAFE_RELEASE(m_pShopBackPanel);
	SAFE_RELEASE(m_pShopExitBtn);
	SAFE_RELEASE(m_pShopUpBtn);
	SAFE_RELEASE(m_pShopDownBtn);
	SAFE_RELEASE(m_pShopScrollBtn);
}

void UIInventory::DisableShopPanel(int iNum, float fTime)
{
	m_pShopExitBtn->SetEnable(false);

	m_pShopUpBtn->SetEnable(false);

	m_pShopDownBtn->SetEnable(false);

	m_pShopScrollBtn->SetEnable(false);

	for (int i = 0; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i])
			m_vecItem[i]->SetAlpha(255);
	}

	m_bShopUIOn = false;
}

bool UIInventory::Init()
{
	SetSize(422.f,48.f);
	SetTexture("InventoryShortcut", TEXT("UI\\InvenShortCut.bmp"));
	SetColorKey(255, 255, 255);

	m_pCursorTex = GET_SINGLE(ResourcesManager)->LoadTexture("Menu", TEXT("Maps\\MenuTiles.bmp"));
	m_pCursorTex->SetColorKey(255, 255, 255);

	m_pBackTexture = GET_SINGLE(ResourcesManager)->LoadTexture("pade", TEXT("UI\\Pade.bmp"));

	return true;
}

void UIInventory::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);

	if(!m_pInventoryPanel)
		CreateInventory();

	if (KEYDOWN("Inventory"))
	{
		size_t iSize = m_vecInvenPanel.size();

		if (!m_bExtended)
		{
			m_bExtended = true;

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->SetEnable(false);

			m_pInventoryPanel->SetEnable(true);
		}

		else
		{
			m_bExtended = false;

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->SetEnable(false);

			m_pInventoryPanel->SetEnable(false);
		}
	}
}

int UIInventory::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	POSITION tPos = {};

	bool bEnable = m_pInventoryPanel->GetEnable();

	for (int i = 0; i < INVENTORY_SHORT; ++i)
	{
		if (m_vecItem[i])
		{
			if (!m_vecItem[i]->IsDrag())
			{
				POSITION tSize = m_vecItem[i]->GetSize();

				if (!bEnable && !m_bShopUIOn)
				{
					tPos = m_tPos + tCamPos + POSITION(8.f, 8.f);

					tPos += m_vecItem[i]->GetPivot() * tSize;

					tPos.x += (tSize.x + 2) * i;
				}

				else
				{
					tPos = m_pInventoryPanel->GetPos() + tCamPos;

					if (m_bShopUIOn)
					{
						tPos = m_pShopBackPanel->GetPos() + tCamPos;

						tPos.x -= 16.f;
						tPos.y -= 16.f;
					}

					tPos += m_vecItem[i]->GetPivot() * tSize;

					tPos.x += tSize.x * i + 16.f;

					tPos.y += 16.f;
				}

				m_vecItem[i]->SetPos(tPos);
			}

			m_vecItem[i]->SetEnable(true);
		}
	}

	for (int i = INVENTORY_SHORT; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i])
		{
			if (!m_vecItem[i]->IsDrag())
			{
				POSITION tSize = m_vecItem[i]->GetSize();

				tPos = m_pInventoryPanel->GetPos() + tCamPos;

				tPos += m_vecItem[i]->GetPivot() * tSize;

				tPos.x += tSize.x * (i % INVENTORY_SHORT) + 16.f;

				tPos.y += 21.f + (i / INVENTORY_SHORT) * tSize.y;

				m_vecItem[i]->SetPos(tPos);
			}

			m_vecItem[i]->SetEnable(bEnable);
		}			
	}

	if (m_bShopUIOn)	//	상점 판넬이 열렸을 경우이다.
	{
		tPos = m_pShopPanel->GetPos();

		POSITION tSize = m_pShopPanel->GetSize();

		tPos.x += tSize.x + 22.f;

		tPos.x += (m_pShopUpBtn->GetSize().x - m_pShopScrollBtn->GetSize().x)/2.f;

		float fScrollPosY = m_pShopScrollBtn->GetPos().y;

		float fScrollSizeY = m_pShopScrollBtn->GetSize().y;
		float fUpBtnPosY = m_pShopUpBtn->GetPos().y;
		float fUpBtnSizeY = m_pShopUpBtn->GetSize().y;
		float fDownBtnPosY = m_pShopDownBtn->GetPos().y;

		if (fScrollPosY > fDownBtnPosY - fScrollSizeY)
			fScrollPosY = fDownBtnPosY - fScrollSizeY;

		else if (fScrollPosY < fUpBtnPosY + fUpBtnSizeY)
			fScrollPosY= fUpBtnPosY + fUpBtnSizeY;

		m_pShopScrollBtn->SetPos(tPos.x, fScrollPosY);

		size_t iSize = m_vecShopItemPanel.size();

		m_iPage = (int)((iSize - SHOP_PAGE) * (fScrollPosY - (fUpBtnPosY + fUpBtnSizeY)) / ((fDownBtnPosY - fScrollSizeY) - (fUpBtnPosY + fUpBtnSizeY)));

		POSITION tBackPanelPos = m_pShopPanel->GetPos();

		for (size_t i = 0; i < iSize; ++i)	//	아이템 목록들의 위치를 업데이트 한다.
		{
			POSITION tPos = {};

			POSITION tPanelPos = m_vecShopItemPanel[i]->tPos;
			tPanelPos.y -= m_iPage * 32.f;

			m_vecShopItemPanel[i]->pItem->SetPos(tPos + tPanelPos + tBackPanelPos);

			tPos += POSITION(37.f, 6.f);

			m_vecShopItemPanel[i]->pName->SetPos(tPos + tPanelPos + tBackPanelPos);
			((Text*)m_vecShopItemPanel[i]->pName)->SetColor(102, 28, 28);

			tPos += POSITION(32.f * 12.f - 5.f, 0.f);

			m_vecShopItemPanel[i]->pPrice->SetPos(tPos + tPanelPos + tBackPanelPos);
			((Text*)m_vecShopItemPanel[i]->pPrice)->SetColor(102, 28, 28);

			tPos += POSITION(32.f * 2.f, 1.f);

			m_vecShopItemPanel[i]->pIcon->SetPos(tPos + tPanelPos + tBackPanelPos);
		}
	}

	return 0;
}

int UIInventory::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIInventory::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIInventory::Render(HDC hDC, float fDeltaTime)
{
	bool bEnable = m_pInventoryPanel->GetEnable();

	RESOLUTION tRS = GET_SINGLE(Core)->GetResolution();

	if (bEnable || m_bShopUIOn)
	{
		BLENDFUNCTION tBF = {};

		tBF.BlendFlags = 0;
		tBF.BlendOp = 0;
		tBF.AlphaFormat = AC_SRC_OVER;
		tBF.SourceConstantAlpha = 125;

		GdiAlphaBlend(hDC, 0, 0, tRS.iW, tRS.iH, m_pBackTexture->GetDC(), 0, 0, tRS.iW, tRS.iH, tBF);

		if(!m_bExtended)	//	상점 창이 열린 경우다.
		{
			m_pShopBackPanel->Render(hDC, fDeltaTime);
			m_pShopPanel->Render(hDC, fDeltaTime);

			size_t iSize = m_vecShopPanel.size();

			for (size_t i = 0; i < iSize; ++i)
				m_vecShopPanel[i]->Render(hDC, fDeltaTime);

			size_t iContextSize = m_vecShopItemPanel.size();

			for (size_t i = 0; i < SHOP_PAGE; ++i)
			{
				m_vecShopItemPanel[i + m_iPage]->pItem->Render(hDC, fDeltaTime);
				m_vecShopItemPanel[i + m_iPage]->pIcon->Render(hDC, fDeltaTime);
				m_vecShopItemPanel[i + m_iPage]->pName->Render(hDC, fDeltaTime);
				m_vecShopItemPanel[i + m_iPage]->pPrice->Render(hDC, fDeltaTime);
			}

			m_pShopValancePanel->Render(hDC, fDeltaTime);

			m_pShopExitBtn->Render(hDC, fDeltaTime);

			m_pShopUpBtn->Render(hDC, fDeltaTime);

			m_pShopDownBtn->Render(hDC, fDeltaTime);

			m_pShopScrollBtn->Render(hDC, fDeltaTime);

			TCHAR strPage[32] = {};
			swprintf_s(strPage, TEXT("Page: %d"), m_iPage);
			TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y, strPage, lstrlen(strPage));
		}

		else
		{
			m_pInventoryPanel->Render(hDC, fDeltaTime);

			size_t iSize = m_vecInvenPanel.size();

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->Render(hDC, fDeltaTime);
		}
	}

	UI::Render(hDC, fDeltaTime);

	for (int i = 0; i < INVENTORY_SHORT; ++i)
	{
		if (m_vecItem[i])
			m_vecItem[i]->Render(hDC, fDeltaTime);
	}

	if (m_bShopUIOn)
	{
		for (int i = 0; i < INVENTORY_MAX; ++i)
		{
			if (m_vecItem[i])
			{
				m_vecItem[i]->SetAlpha(120);
				m_vecItem[i]->EnableAlpha(true);
			}				
		}
	}

	if (bEnable || m_bShopUIOn)
	{
		for (int i = INVENTORY_SHORT; i < INVENTORY_MAX; ++i)
		{
			if (m_vecItem[i])
				m_vecItem[i]->Render(hDC, fDeltaTime);
		}
	}

	if (m_pCursorTex)
		TransparentBlt(hDC, (int)m_tPos.x + 8 + m_iCursor * 34, (int)m_tPos.y + 8, 32, 32, m_pCursorTex->GetDC(),
			0, 448, 32, 32, m_pCursorTex->GetColorKey());
	
}

UIInventory* UIInventory::Clone()
{
	return new UIInventory(*this);
}

void UIInventory::CreateInfoPanel(int iCountX, int iCountY)
{
	Layer* pLayer = m_pScene->FindLayer("UI");

	m_pBackPanel = CreateObj<UIPanel>("BackPanel", pLayer);

	m_pBackPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

	m_pBackPanel->SetSize(32.f * 3.f, 32.f * 3.f);

	m_pBackPanel->SetEnable(false);

	m_iCountX = iCountX;
	m_iCountY = iCountY;

	m_vecPanel.resize(m_iCountX * m_iCountY);

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			UIPanel* pPanel = CreateObj<UIPanel>("Panel", pLayer);

			if (i != 2 || j == 0 || j == m_iCountY - 1)
			{
				pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));
				pPanel->SetColorKey(255, 255, 255);
			}

			pPanel->SetSize(32.f, 32.f);

			if(i !=2 && j> 0 && j< m_iCountX-1)
				pPanel->SetImageOffset(32.f * (m_iCountX - 2), 32.f * i);

			else
				pPanel->SetImageOffset(32.f * j, 32.f * i);

			pPanel->SetEnable(false);

			m_vecPanel[i * m_iCountX + j] = pPanel;

			SAFE_RELEASE(pPanel);
		}
	}
}

void UIInventory::InfoPanelOn(const POSITION& tPos)
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
		{
			m_vecPanel[i * m_iCountX + j]->SetEnable(true);
			m_vecPanel[i * m_iCountX + j]->SetPos(tPos);
		}
	}

	m_pBackPanel->SetEnable(true);
	m_pBackPanel->SetPos(tPos + POSITION(16.f, 16.f));
}

void UIInventory::InfoPanelUpdate(const POSITION& tPos)
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
			m_vecPanel[i * m_iCountX + j]->SetPos(tPos.x + 32.f * j, tPos.y + 32.f * i);
	}

	m_pBackPanel->SetPos(tPos + POSITION(16.f, 16.f));
}

void UIInventory::InfoPanelOff()
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
			m_vecPanel[i * m_iCountX + j]->SetEnable(false);
	}

	m_pBackPanel->SetEnable(false);
}

void UIInventory::CreateInventory()
{
	int iTileNumX = 14;
	int iTileNumY = 10;

	Layer* pLayer = m_pScene->FindLayer("Default");

	m_pInventoryPanel = CreateObj<UIPanel>("InventoryPanel", pLayer);

	m_pInventoryPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

	m_pInventoryPanel->SetSize((iTileNumX-1) * 32.f, (iTileNumY - 1) * 32.f);

	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	POSITION tSize = m_pInventoryPanel->GetSize();

	m_pInventoryPanel->SetPos(tRS.iW / 2.f - tSize.x / 2.f, tRS.iH / 2.f - tSize.y / 2.f);

	m_pInventoryPanel->SetEnable(false);

	m_vecInvenPanel.reserve(132);

	POSITION tPos = m_pInventoryPanel->GetPos();

	for (int i = 0; i < iTileNumY; ++i)
	{
		for (int j = 0; j < iTileNumX; ++j)
		{
			if (i == 0 || i == iTileNumY-1 || j == 0 || j == iTileNumX-1)
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

				m_vecInvenPanel.push_back(pPanel);

				SAFE_RELEASE(pPanel);
			}
		}		
	}

	for (int i = 0; i < iTileNumX; ++i)
	{
		UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

		pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

		pPanel->SetColorKey(255, 255, 255);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(32.f * i + tPos.x - 16.f, tRS.iH / 2.f - 32.f);

		if (i != 0 && i != iTileNumX-1)
			pPanel->SetImageOffset(64.f, 32.f);

		else if (i == 0)
			pPanel->SetImageOffset(0.f, 32.f);

		else
			pPanel->SetImageOffset(96.f, 32.f);

		pPanel->SetEnable(false);

		m_vecInvenPanel.push_back(pPanel);

		SAFE_RELEASE(pPanel);
	}

	for (int i = 0; i < 12; ++i)
	{
		UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

		pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

		pPanel->SetColorKey(255, 255, 255);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(32.f * i + tPos.x + 16.f, tPos.y + 16.f);

		pPanel->SetImageOffset(64.f, 64.f);

		pPanel->SetEnable(false);

		m_vecInvenPanel.push_back(pPanel);

		SAFE_RELEASE(pPanel);
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 12; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(32.f * i + tPos.x + 16.f, 32.f * (j + 1) + tPos.y + 16.f + 5.f);

			pPanel->SetImageOffset(64.f, 64.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);

			SAFE_RELEASE(pPanel);
		}
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(14.f + 100.f * j + tPos.x, 32.f * (i + 4) + tPos.y + 16.f);

			if (i == 0 && j == 0)
				pPanel->SetImageOffset(32.f, 320.f);

			else if (i == 1 && j == 0)
				pPanel->SetImageOffset(96.f, 320.f);

			else if (i == 2 && j == 0)
				pPanel->SetImageOffset(0.f, 320.f);

			else if (i == 0 && j == 1)
				pPanel->SetImageOffset(64.f, 320.f);

			else if (i == 1 && j == 1)
				pPanel->SetImageOffset(32.f, 288.f);

			else if (i == 2 && j == 1)
				pPanel->SetImageOffset(0.f, 288.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);

			SAFE_RELEASE(pPanel);
		}
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(48.f + 32*j + tPos.x, 32.f * (i + 4) + tPos.y + 16.f);

			if ((i + j) % 2 == 0)
				pPanel->SetImageOffset(0.f, 512.f);

			else
				pPanel->SetImageOffset(64.f, 512.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);

			SAFE_RELEASE(pPanel);
		}
	}

	UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

	pPanel->SetTexture("playerIdleDown", TEXT("aniii.bmp"));

	pPanel->SetColorKey(0, 0, 0);

	pPanel->SetSize(32.f, 64.f);

	pPanel->SetPos(64.f +tPos.x, 32.f * 5.f + tPos.y);

	pPanel->SetEnable(false);

	m_vecInvenPanel.push_back(pPanel);

	SAFE_RELEASE(pPanel);
}

void UIInventory::SwapItem(Item* pItem, const POSITION& tPos)
{
	if (!m_pInventoryPanel->GetEnable())
	{
		POSITION tSwapPos = tPos - POSITION(8.f, 8.f) - m_tPos;

		int iIndexX = (int)(tSwapPos.x / 33.f);
		int iIndexY = (int)(tSwapPos.y / 33.f);
		
		if (iIndexY == 0 && iIndexX < 12 && iIndexX >= 0)
		{
			size_t iSize = m_vecItem.size();

			int iIndex = 0;

			for (size_t i = 0; i < iSize; ++i)
			{
				if (m_vecItem[i] == pItem)
				{
					iIndex = (int)i;
					break;
				}
			}

			if (m_vecItem[iIndexX])
			{
				Item* pTempItem = m_vecItem[iIndexX];
				m_vecItem[iIndexX] = m_vecItem[iIndex];
				m_vecItem[iIndex] = pTempItem;
			}
			else
			{
				m_vecItem[iIndexX] = pItem;
				m_vecItem[iIndex] = nullptr;
			}
		}
	}

	else
	{
		POSITION tSwapPos = tPos - POSITION(16.f, 16.f) - m_pInventoryPanel->GetPos();

		int iIndexX = (int)(tSwapPos.x / 32.f);
		int iIndexY = (int)(tSwapPos.y / 32.f);

		if (iIndexY >= 0 && iIndexY <= 2 && iIndexX < 12 && iIndexX >= 0)
		{
			size_t iSize = m_vecItem.size();

			int iIndex = 0;

			for (size_t i = 0; i < iSize; ++i)
			{
				if (m_vecItem[i] == pItem)
				{
					iIndex = (int)i;
					break;
				}
			}

			if (m_vecItem[iIndexX + iIndexY * 12])
			{
				Item* pTempItem = m_vecItem[iIndexX + iIndexY * 12];
				m_vecItem[iIndexX + iIndexY * 12] = m_vecItem[iIndex];
				m_vecItem[iIndex] = pTempItem;
			}
			else
			{
				m_vecItem[iIndexX + iIndexY * 12] = pItem;
				m_vecItem[iIndex] = nullptr;
			}
		}
	}
}

void UIInventory::CreateShopPanel()
{
	m_bShopUIOn = true;

	if (m_pShopPanel)
	{
		m_pShopExitBtn->SetEnable(true);

		m_pShopUpBtn->SetEnable(true);

		m_pShopDownBtn->SetEnable(true);

		m_pShopScrollBtn->SetEnable(true);

		return;
	}
		

	int iTileNumX = 17;
	int iTileNumY = 5;

	Layer* pLayer = m_pScene->FindLayer("Default");

	m_pShopPanel = CreateObj<UIPanel>("ShopPanel", pLayer);	//	상점 배경을 만든다.

	m_pShopPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

	m_pShopPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	POSITION tSize = m_pShopPanel->GetSize();

	m_pShopPanel->SetPos(tRS.iW / 2.f - tSize.x / 2.f, tRS.iH / 2.f - (tSize.y / 2.f + 200.f));

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

				SAFE_RELEASE(pPanel);
			}
		}
	}

	for(int i=0;i<8;++i)		//	아이템 목록을 만든다.
	{
		PITEMPANEL pPanel = new ITEMPANEL;

		pPanel->tPos.y = 32.f * i;

		PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(i);

		pPanel->pIcon = Obj::CreateObj<UIPanel>("MoneyIcon", pLayer);
		pPanel->pIcon->SetTexture("Mouse");
		pPanel->pIcon->SetImageOffset(386.f, 746.f);
		pPanel->pIcon->SetEnable(false);
		pPanel->pIcon->SetPos(11.f * 32.f, 0.f);
		pPanel->pIcon->SetSize(18.f, 18.f);

		pPanel->pItem = Obj::CreateObj<UIPanel>("ItemPanel", pLayer);
		pPanel->pItem->SetTexture("items");
		pPanel->pItem->SetImageOffset(((16 + i) % 24) * 32.f, (19.f + ((i + 16) / 24)) * 32.f);
		pPanel->pItem->SetEnable(false);
		pPanel->pItem->SetSize(32.f, 32.f);

		pPanel->pName = Obj::CreateObj<Text>("ItemName", pLayer);

		char pName[256] = {};

		strcpy_s(pName, pInfo->strName.c_str());

		TCHAR strName[256] = {};

		MultiByteToWideChar(CP_ACP, NULL, pName, -1, strName, (int)strlen(pName));

		((Text*)pPanel->pName)->SetText(strName);
		pPanel->pName->SetEnable(false);
		pPanel->pName->SetPos(32.f, 0.f);

		char pNumber[6] = {};

		TCHAR strNumber[256] = {};

		int iSize = sizeof(pNumber) / sizeof(char);

		int iDiv = 1;

		for (int i = 0; i < iSize; ++i)
		{
			if (pInfo->vecPrice[0] >= iDiv)
				pNumber[iSize - i - 1] = (pInfo->vecPrice[0] % (iDiv * 10)) / iDiv + '0';

			else
			{
				memmove(pNumber, pNumber + iSize - i, i);
				memset(pNumber + i, 0, iSize - i);

				break;
			}

			iDiv *= 10;
		}

		MultiByteToWideChar(CP_ACP, NULL, pNumber, -1, strNumber, (int)strlen(pNumber));

		pPanel->pPrice = Obj::CreateObj<Text>("Price", pLayer);
		((Text*)pPanel->pPrice)->SetText(strNumber);
		pPanel->pPrice->SetEnable(false);
		pPanel->pPrice->SetPos(32.f * 8.f, 0.f);

		m_vecShopItemPanel.push_back(pPanel);
	}

	m_pShopValancePanel = Obj::CreateObj<UIPanel>("ValancePanel", pLayer);

	m_pShopValancePanel->SetTexture("Mouse");
	m_pShopValancePanel->SetColorKey(255, 255, 255);
	m_pShopValancePanel->SetSize(130.f, 34.f);
	m_pShopValancePanel->SetImageOffset(680.f, 944.f);

	tPos = m_pShopPanel->GetPos();

	tPos.x -= 16.f;
	tPos.y += m_pShopPanel->GetSize().y + 8.f;

	m_pShopValancePanel->SetPos(tPos);
	m_pShopValancePanel->SetAlpha(255);
	m_pShopValancePanel->EnableAlpha(true);
	m_pShopValancePanel->SetEnable(false);

	iTileNumX = 13;
	iTileNumY = 4;

	m_pShopBackPanel = Obj::CreateObj<UIPanel>("ShopBackPanel", pLayer);	//	배경 판을 만든다.

	m_pShopBackPanel->SetSize((iTileNumX - 1) * 32.f, (iTileNumY - 1) * 32.f);

	tPos.x += m_pShopValancePanel->GetSize().x + 12.f;
	tPos.y += 12.f;

	m_pShopBackPanel->SetTexture("backPanel");
	m_pShopBackPanel->SetPos(tPos);
	m_pShopBackPanel->SetEnable(false);

	tPos = m_pShopBackPanel->GetPos();

	for (int i = 0; i < iTileNumY; ++i)	//	사각 틀을 만든다.
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

				SAFE_RELEASE(pPanel);
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

			SAFE_RELEASE(pPanel);
		}
	}

	tPos = m_pShopPanel->GetPos();

	tPos.x += tSize.x + 22.f;
	tPos.y -= 26.f;

	m_pShopExitBtn = Obj::CreateObj<UIButton>("ShopExitButton", pLayer);	//	상점 판넬 종료 버튼을 생성한다.

	m_pShopExitBtn->SetSize(22.f, 22.f);
	m_pShopExitBtn->SetImageOffset(676.f, 988.f);
	m_pShopExitBtn->SetTexture("Mouse");
	m_pShopExitBtn->SetCallback(this, &UIInventory::DisableShopPanel);
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
	m_pShopUpBtn->SetCallback(this, &UIInventory::ShopPageUp);
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
	m_pShopDownBtn->SetCallback(this, &UIInventory::ShopPageDown);
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
}

void UIInventory::ShopPageUp(int iNum, float fTime)
{
}

void UIInventory::ShopPageDown(int iNum, float fTime)
{
}

void UIInventory::AddItem(Item* pItem)
{
	if (pItem)
	{
		for (int i = 0; i < 36; ++i)
		{
			if (m_vecItem[i])
			{
				ITEM_TYPE eItemType = m_vecItem[i]->GetType();

				if (eItemType == pItem->GetType())
				{
					if (eItemType != IT_TOOL)
					{
						int iIndex = m_vecItem[i]->GetIndex();

						if (iIndex == pItem->GetIndex())
						{
							m_vecItem[i]->AddItemCount(1);

							pItem->Die();

							return;
						}
					}
				}
			}

			else
			{
				m_vecItem[i] = pItem;

				ITEM_TYPE eItemType = m_vecItem[i]->GetType();

				if (eItemType == IT_SEED || eItemType == IT_CROP || eItemType == IT_ETC)
				{
					int iItemCount = m_vecItem[i]->GetItemCount();

					m_vecItem[i]->CreateItemNumber();

					m_vecItem[i]->AddItemCount(1);
				}

				pItem->AddRef();

				pItem->SetInventory(this);

				++m_iCount;

				break;
			}
		}

	}
}

void UIInventory::DeleteItem()
{
	if (m_vecItem[m_iCursor])
	{
		int iCount = m_vecItem[m_iCursor]->GetItemCount();

		if (iCount > 1)
			m_vecItem[m_iCursor]->AddItemCount(-1);

		else
		{
			m_vecItem[m_iCursor]->DeleteItemNumber();

			m_vecItem[m_iCursor]->Die();

			SAFE_RELEASE(m_vecItem[m_iCursor]);
		}
	}
}
