#include "Item.h"
#include "../Collider/ColliderRect.h"
#include "Player.h"
#include "UIInventory.h"

Item::Item()	:
	m_eType(IT_NONE),
	m_bInventory(false)
{
}

Item::Item(const Item& item)	:
	MoveObj(item)
{
	m_eType = item.m_eType;
	m_bInventory = item.m_bInventory;
}

Item::~Item()
{
}

bool Item::Init()
{
	SetTexture("items", TEXT("Item\\springobjects.bmp"));
	SetColorKey(255, 255, 255);
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
	return 0;
}

int Item::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void Item::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Item::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

Item* Item::Clone()
{
	return nullptr;
}

void Item::CollEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (!m_bInventory)
	{
		if (pDest->GetTag() == "PlayerBody")
		{
			((Player*)pDest->GetObj())->GetInven()->AddItem(this);
		}
	}
}
