#include "Crop.h"
#include "../Scene/Scene.h"
#include "../Scene/InGameScene.h"
#include "Stage.h"
#include "Tile.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"
#include "Player.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "../Resources/ResourcesManager.h"

Crop::Crop()	:
	m_eCropType(CROP_NONE),
	m_iDay(0),
	m_iStage(0),
	m_bTimeStart(false),
	m_bHarvested(false),
	m_iMaxStage(0),
	m_iHP(0),
	m_iEnergy(0),
	m_iRegrowthDay(0),
	m_pFlower(nullptr),
	m_bFrozen(false)
{
	m_eBlock = OB_CROP;
	m_cAlpha = 255;
	m_bAlphaOn = true;
}

Crop::Crop(const Crop& crop)	:
	Item(crop)
{
	m_eCropType = crop.m_eCropType;
	m_iStage = crop.m_iStage;
	m_iDay = crop.m_iDay;
	m_bTimeStart = false;
	m_bHarvested = false;
	m_iMaxStage = crop.m_iMaxStage;
	m_vecMaxDay = crop.m_vecMaxDay;
	m_iHP = crop.m_iHP;
	m_iEnergy = crop.m_iEnergy;
	m_iRegrowthDay = crop.m_iRegrowthDay;

	if (crop.m_pFlower)
		m_pFlower = crop.m_pFlower->Clone();

	else
		m_pFlower = nullptr;

	m_bFrozen = crop.m_bFrozen;
}

Crop::~Crop()
{
	SAFE_RELEASE(m_pFlower);
}

void Crop::AddDay(int iDay)
{
	Tile* pTile = m_pScene->GetStage()->GetTile(m_tPos);

	if (pTile->GetTileOption() == TO_WATERDIRT && m_iStage >= 1)
	{
		m_iDay += iDay;

		if (m_iRegrowthDay != 0)
		{
			if (m_iStage == m_iMaxStage)
				m_iDay -= iDay;
		}
	}		

	if (m_iStage < m_iMaxStage)
	{
		if (m_iDay >= m_vecMaxDay[m_iStage - 1])
		{
			m_iDay -= m_vecMaxDay[m_iStage - 1];

			Grow();
		}
	}

	else if (m_iRegrowthDay < m_iDay && m_iStage > m_iMaxStage)
	{
		m_iDay -= m_iRegrowthDay;

		POSITION tSize = GetSize();

		tSize.y = 0;

		POSITION tOffset = GetImageOffset();

		SetImageOffset(tOffset - tSize);	//	이미지 오프셋을 작물 가로 크기만큼 한칸 왼쪽으로 이동시킨다.

		--m_iStage;
	}
}

void Crop::Grow()
{
	POSITION tSize = GetSize();

	tSize.y = 0;

	POSITION tOffset = GetImageOffset();

	SetImageOffset(tSize + tOffset);	//	이미지 오프셋을 작물 가로 크기만큼 한칸 오른쪽으로 이동시킨다.

	++m_iStage;

	if (m_iStage > m_iMaxStage)
		m_iStage = m_iMaxStage;

	else if (m_iStage == m_iMaxStage)
	{
		if (!m_pFlower && m_eCropType == CROP_JAZZ ||
			m_eCropType == CROP_POPPY || m_eCropType == CROP_SPANGLE || m_eCropType == CROP_ROSE)
		{
			PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(m_eCropType);

			m_pFlower = (Crop*)CreateCloneObj(pInfo->strCropName, pInfo->strCropName, m_pLayer);

			m_pFlower->SetPos(GetPos());

			POSITION tImageOffset = GetImageOffset();

			tImageOffset.x += GetSize().x;

			m_pFlower->SetImageOffset(tImageOffset);
		}
	}
}

void Crop::Harvest()
{
	PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(m_eCropType);

	if (m_iRegrowthDay == 0)
	{
		m_bHarvested = true;

		SetItemType(IT_CROP);
		SetSize(32.f, 32.f);
		SetTexture("items", TEXT("Item\\springobjects.bmp"));

		SetImageOffset(GetSize() * pInfo->tHarvestOffset);

		if (m_iStage == m_iMaxStage)
		{
			Obj* pEffect = CreateCloneObj("HoeEffect", "HarvestEffect", m_pLayer);

			pEffect->SetAnimationCurrentClip("HarvestEffect");
			pEffect->SetPos(GetPos());

			SAFE_RELEASE(pEffect);
		}

		if (m_pFlower)
			m_pFlower->Die();
	}

	else if (m_iStage == m_iMaxStage)
	{		
		Crop* pItem = (Crop*)CreateCloneObj(pInfo->strCropName, pInfo->strCropName, m_pLayer);

		pItem->SetRegrowthDay(0);

		pItem->SetPos(GetPos());

		pItem->Harvest();

		POSITION tOffSet = GetImageOffset();

		tOffSet.x += GetSize().x;

		SetImageOffset(tOffSet);

		++m_iStage;

		SAFE_RELEASE(pItem);
	}
}

bool Crop::Init()
{
	if (!Item::Init())
		return false;

	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);
	SetTexture("crops", TEXT("TileSheets\\crops.bmp"));
	SetColorKey(255, 255, 255);

	ColliderRect* pRC = AddCollider<ColliderRect>("CropBody");

	pRC->SetRect(-16.f, -32.f, 16.f, 32.f);

	SAFE_RELEASE(pRC);

	return true;
}

int Crop::Update(float fDeltaTime)
{
	Item::Update(fDeltaTime);

	return 0;
}

int Crop::LateUpdate(float fDeltaTime)
{
	Item::LateUpdate(fDeltaTime);
	return 0;
}

void Crop::Collision(float fDeltaTime)
{
	Item::Collision(fDeltaTime);
}

void Crop::Render(HDC hDC, float fDeltaTime)
{
	Item::Render(hDC, fDeltaTime);
}

Crop* Crop::Clone()
{
	return new Crop(*this);
}

void Crop::MouseOn(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Crop::MousePress(Collider* pSrc, Collider* pDest, float fTime)
{
	if (m_bHarvested && !m_bInventory && !m_bFrozen)
	{
		UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

		if (pDest->GetTag() == "PlayerBody")
			pInven->AddItem(this);

		SAFE_RELEASE(pInven);

		Collider* pRC = GetCollider("CropBody");

		pRC->Die();

		SAFE_RELEASE(pRC);
	}
}

void Crop::MouseOff(Collider* pSrc, Collider* pDest, float fTime)
{
}
