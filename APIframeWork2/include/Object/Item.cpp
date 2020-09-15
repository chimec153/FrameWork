#include "Item.h"
#include "../Collider/ColliderRect.h"
#include "Player.h"
#include "UIInventory.h"
#include "UIPanel.h"
#include "../Scene/Scene.h"
#include "UINum.h"
#include "../Core/Input.h"
#include "ObjManager.h"
#include "UIShop.h"
#include "../Core/Camera.h"
#include "../Resources/ResourcesManager.h"
#include "Crop.h"

Item::Item()	:
	m_eType(IT_NONE),
	m_bInventory(false),
	m_bMouseOn(false),
	m_pInventory(nullptr),
	m_iCount(0),
	m_pNumber(nullptr),
	m_iIndex(0),
	m_bDrag(false),
	m_iSellPrice(0),
	m_iFileIndex(-1)
{
}

Item::Item(const Item& item)	:
	MoveObj(item)
{
	m_eType = item.m_eType;
	m_bInventory = item.m_bInventory;
	m_pInventory = item.m_pInventory;
	m_bMouseOn = false;
	m_iCount = item.m_iCount;

	if (item.m_pNumber)
		m_pNumber = item.m_pNumber->Clone();

	else
		m_pNumber = nullptr;	

	m_iIndex = item.m_iIndex;
	m_bDrag = false;
	m_iSellPrice = item.m_iSellPrice;
	m_iFileIndex = item.m_iFileIndex;

	Collider* pCol = GetCollider("ItemBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &Item::CollEnter);
	pCol->AddCollisionFunction(CS_STAY, this, &Item::ColStay);
	pCol->AddCollisionFunction(CS_LEAVE, this, &Item::ColEnd);

	SAFE_RELEASE(pCol);
}

Item::~Item()
{
	if(m_pNumber)
		m_pNumber->Die();

	SAFE_RELEASE(m_pNumber);
}

void Item::AddItemCount(int iCount)
{
	m_iCount += iCount;

	if (m_pNumber)
	{
		m_pNumber->DeleteNum();

		m_pNumber->CreateNum(m_iCount);

		m_pNumber->SetEnable(false);
	}
}

bool Item::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);

	ColliderRect* pRC = AddCollider<ColliderRect>("ItemBody");

	pRC->SetRect(-16.f, -16.f, 16.f, 16.f);

	SAFE_RELEASE(pRC);

	return true;
}

int Item::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	if (m_bInventory && m_pNumber)
	{
		POSITION tSize = m_pNumber->GetSize();

		POSITION tPos = m_tPos + m_tSize * m_tPivot - tSize;

		m_pNumber->SetPosAll(tPos);

		//DisableItem();
	}

	if (m_bInventory)
	{
		if (m_bDrag)
		{
			if (KEYPRESS("MouseLButton"))
			{
				POSITION tMousePos = GET_SINGLE(Input)->GetMouseClientPos();

				SetPos(m_tStartPos + tMousePos);
			}

			else if (KEYUP("MouseLButton"))
			{
				POSITION tMousePos = GET_SINGLE(Input)->GetMouseClientPos();

				m_pInventory->SwapItem(this, tMousePos);

				m_bDrag = false;
			}
		}
	}

	return 0;
}

int Item::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);

	if (m_bInventory && m_pNumber)
	{
		POSITION tSize = m_pNumber->GetSize();

		POSITION tPos = m_tPos + m_tSize * m_tPivot - tSize;

		m_pNumber->SetPosAll(tPos);
	}

	return 0;
}

void Item::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Item::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);

	if (m_pNumber)
		m_pNumber->Render(hDC, fDeltaTime);

#ifdef _DEBUG
	
	if (KEYPRESS("Debug"))
	{
		POSITION tPos = m_tPos - GET_SINGLE(Camera)->GetPos();

		TCHAR strHP[32] = {};

		wsprintf(strHP, TEXT("L:%d"), m_pLayer->GetZOrder()%1000);
		TextOut(hDC, (int)tPos.x, (int)tPos.y, strHP, lstrlen(strHP));
		wsprintf(strHP, TEXT("S:%d"), ((int)m_pScene)%1000);
		TextOut(hDC, (int)tPos.x, (int)tPos.y - 20, strHP, lstrlen(strHP));
	}
#endif
}

Item* Item::Clone()
{
	return nullptr;
}

void Item::CollEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (!m_bInventory)
	{
		ITEM_TYPE eType = GetType();

		if (eType == IT_CROP)
		{
			bool bHarvest = ((Crop*)pSrc->GetObj())->Crop::IsHarvested();

			if (bHarvest)
			{
				if (pDest->GetTag() == "PlayerBody")
				{
					UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

					pInven->AddItem(this);

					SAFE_RELEASE(pInven);
				}
			}
		}

		else if (pDest->GetTag() == "PlayerBody")
		{
			UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

			pInven->AddItem(this);

			SAFE_RELEASE(pInven);
		}

	}

	else
	{
		if (pDest->GetTag() == "Mouse")
		{
			PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(m_iFileIndex);

			m_bMouseOn = true;

			m_pInventory->InfoPanelOn(pDest->GetObj()->GetPos(), *pInfo);
		}
	}
}

void Item::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
	if (m_bInventory)
	{
		if (pDest->GetTag() == "Mouse")
		{
			if (m_bMouseOn)
				m_pInventory->InfoPanelUpdate(pDest->GetObj()->GetPos());

			else
			{
				PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(m_iFileIndex);

				m_bMouseOn = true;

				m_pInventory->InfoPanelOn(pDest->GetObj()->GetPos(), *pInfo);
			}

			if (KEYDOWN("MouseLButton"))
			{
				m_bDrag = true;

				m_tStartPos = m_tPos - pDest->GetObj()->GetPos();
			}

			else if (KEYPRESS("MouseLButton"))
			{
				if (m_bDrag)
				{
					POSITION tMousePos = GET_SINGLE(Input)->GetMouseClientPos();

					SetPos(m_tStartPos + tMousePos);
				}
			}

			else if (KEYDOWN("MouseRButton"))
			{
				SellItem();
			}
		}
	}
}

void Item::ColEnd(Collider* pSrc, Collider* pDest, float fTime)
{
	if (m_bInventory)
	{
		if (m_bMouseOn)
		{
			m_bMouseOn = false;

			m_pInventory->InfoPanelOff();
		}
	}
		
}

void Item::CreateItemNumber()
{
	Layer* pLayer = m_pScene->FindLayer("HUD");

	m_pNumber = (UINum*)CreateCloneObj("NumSm", "ItemNumber", pLayer);

	m_pNumber->CreateNum(m_iCount);

	m_pNumber->SetSpeed(0.f);
}

void Item::DeleteItemNumber()
{
	if (m_pNumber)
		m_pNumber->Die();

	SAFE_RELEASE(m_pNumber);
}

void Item::AddObjectToLayer(Layer* pLayer)
{
	pLayer->AddObject(this);

	if (m_pNumber)
	{
		m_pNumber->DeleteObjectFromLayer(pLayer);
		m_pNumber->AddObjectToLayer(pLayer);
	}		
}

void Item::DisableItem()
{
	if (m_pNumber)
	m_pNumber->DisableNumber();

	SetEnable(false);
}

void Item::EnableItem()
{
	if(m_pNumber)
		m_pNumber->EnableNumber();

	SetEnable(true);
}

void Item::SellItem()
{
	if (m_pInventory)
	{
		bool bShopOn = m_pInventory->GetShop()->IsShopPanelOn();

		if (bShopOn)
		{
			bool bInven = m_bInventory;

			if (bInven)
			{
				Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

				pPlayer->AddGold(m_iSellPrice);

				SAFE_RELEASE(pPlayer);

				m_pInventory->DeleteItem(this);
			}
		}
	}
}
