#include "Grass.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Item.h"
#include "Tool.h"
#include "../Collider/ColliderRect.h"
#include "Player.h"
#include "../Scene/Scene.h"

Grass::Grass()
{
	m_eBlock = OB_GRASS;
	m_cAlpha = 255;
	m_bAlphaOn = true;
}

Grass::Grass(const Grass& grass)	:
	StaticObj(grass)
{
	AddCollisionFunc();

	Collider* pRC = GetCollider("StaticObjBody");

	pRC->AddCollisionFunction(CS_ENTER, this, &Grass::ColEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &Grass::ColEnter);

	SAFE_RELEASE(pRC);
}

Grass::~Grass()
{
}

bool Grass::Init()
{

	return true;
}

int Grass::Update(float fTime)
{
	StaticObj::Update(fTime);
	return 0;
}

void Grass::Collision(float fTime)
{
	StaticObj::Collision(fTime);
}

void Grass::Render(HDC hDC, float fTime)
{
	StaticObj::Render(hDC, fTime);
}

Grass* Grass::Clone()
{
	return new Grass(*this);
}

void Grass::Save(FILE* pFile)
{
	StaticObj::Save(pFile);
}

void Grass::Load(FILE* pFile)
{
	StaticObj::Load(pFile);

	Collider* pRC = GetCollider("StaticObjBody");

	pRC->AddCollisionFunction(CS_ENTER, this, &Grass::ColEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &Grass::ColEnter);

	SAFE_RELEASE(pRC);
}

void Grass::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "attack")
	{
		UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

		Item* pItem = pInven->GetItem();

		if (pItem)
		{
			ITEM_TYPE eType = pItem->GetType();

			if (eType == IT_TOOL)
			{
				TOOL_TYPE eToolType = ((Tool*)pItem)->GetToolType();

				if (eToolType == TOOL_SCYTHE)
				{
					Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

					bool bSilo = pPlayer->GetSilo();

					if (bSilo)
					{
						Layer* pLayer = m_pScene->FindLayer("Default");

						Item* pHay = (Item*)CreateCloneObj("Hay", "Hay", pLayer);

						pHay->SetPos(m_tPos);

						Collider* pHayCol = pHay->GetCollider("ItemBody");

						pHayCol->AddCollisionFunction(CS_ENTER, pHay, &Item::CollEnter);
						pHayCol->AddCollisionFunction(CS_STAY, pHay, &Item::ColStay);
						pHayCol->AddCollisionFunction(CS_LEAVE, pHay, &Item::ColEnd);

						SAFE_RELEASE(pHayCol);

						SAFE_RELEASE(pHay);
					}

					SAFE_RELEASE(pPlayer);

					Die();
				}
			}
		}

		SAFE_RELEASE(pInven);
	}
}
