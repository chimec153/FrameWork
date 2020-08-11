#include "UIInventory.h"
#include "Item.h"
#include "../Core/Camera.h"
#include "..//Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/UIPanel.h"
#include "..//Scene/Scene.h"

UIInventory::UIInventory()	:
	m_iCount(0),
	m_iCursor(0),
	m_pCursorTex(nullptr),
	m_pBackPanel(nullptr),
	m_iCountX(0),
	m_iCountY(0)
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
}

bool UIInventory::Init()
{
	SetSize(422.f,48.f);
	SetTexture("InventoryShortcut", TEXT("UI\\InvenShortCut.bmp"));
	SetColorKey(255, 255, 255);

	m_pCursorTex = GET_SINGLE(ResourcesManager)->LoadTexture("Menu", TEXT("Maps\\MenuTiles.bmp"));
	m_pCursorTex->SetColorKey(255, 255, 255);

	return true;
}

void UIInventory::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIInventory::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);
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
	UI::Render(hDC, fDeltaTime);

	for (int i = 0; i < 12; ++i)
	{
		POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

		POSITION tPos = m_tPos + tCamPos + POSITION(8.f,8.f);

		if (m_vecItem[i])
		{
			POSITION tSize = m_vecItem[i]->GetSize();

			tPos += m_vecItem[i]->GetPivot() * tSize;

			tPos.x += (tSize.x + 2) * i;

			m_vecItem[i]->SetPos(tPos);
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

void UIInventory::InfoPanelOn(POSITION tPos)
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

void UIInventory::InfoPanelUpdate(POSITION tPos)
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

void UIInventory::AddItem(Item* pItem)
{
	if (pItem)
	{
		pItem->SetInventory(this);

		for (int i = 0; i < 36; ++i)
		{
			if (!m_vecItem[i])
			{
				m_vecItem[i] = pItem;
				break;
			}			
		}

		++m_iCount;
	}
}