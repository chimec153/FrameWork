#include "UIInventory.h"
#include "Item.h"
#include "../Core/Camera.h"
#include "..//Resources/Texture.h"
#include "../Resources/ResourcesManager.h"

UIInventory::UIInventory()	:
	m_iCount(0),
	m_iCursor(0),
	m_pCursorTex(nullptr)
{
	m_vecItem.resize(36);

	memset(&m_vecItem[0], 0, 36 * sizeof(Item*));
}

UIInventory::~UIInventory()
{
	Safe_Release_VecList(m_vecItem);
	SAFE_RELEASE(m_pCursorTex);
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

			tPos.x += tSize.x * i;

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

void UIInventory::AddItem(Item* pItem)
{
	if (pItem)
	{
		pItem->AddRef();

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