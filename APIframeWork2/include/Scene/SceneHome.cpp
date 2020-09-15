#include "SceneHome.h"
#include "../Object/Stage.h"
#include "../Core/PathManager.h"
#include "SceneManager.h"
#include "InGameScene.h"
#include "../Collider/ColliderRect.h"
#include "../Object/ObjManager.h"
#include "../Object/Portal.h"
#include "../Object/UIClockHand.h"
#include "../Object/Player.h"
#include "../Object/UIInventory.h"
#include "../Object/Item.h"

SceneHome::SceneHome()
{
}

SceneHome::~SceneHome()
{
}

bool SceneHome::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->LoadFromPath("home.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pRC->SetRect(32.f * 3.f, 32.f * 11.f, 32.f * 4.f, 32.f * 12.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneHome::OutPortalCol);

	SAFE_RELEASE(pRC);

	Layer* pLayer = FindLayer("Default");

	Portal* pPortal = Obj::CreateObj<Portal>("Bed", pLayer);

	pPortal->SetPos(288.f, 288.f);

	ColliderRect* pPortalRC = pPortal->AddCollider<ColliderRect>("BedBody");

	pPortalRC->AddCollisionFunction(CS_ENTER, this, &SceneHome::Sleep);

	pPortalRC->SetRect(0.f, 0.f, 32.f, 32.f);

	SAFE_RELEASE(pPortalRC);

	SAFE_RELEASE(pPortal);

	return true;
}

void SceneHome::OutPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main", SC_NEXT);

	Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

	pPlayer->SetPos(240.f, 396.f);

	SAFE_RELEASE(pPlayer);
}

void SceneHome::Sleep(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		float fClockTime = pClockHand->GetTime();

		if (fClockTime > 1320.f)
		{
			pClockHand->SetTime(360.f);
			pClockHand->AddDay();

			Player* pPlayer = (Player*)pDest->GetObj();

			pPlayer->SetHP((int)(100 * (1800.f - fClockTime) / 480.f));
			pPlayer->SetEnergy((int)(100 * (1800.f - fClockTime) / 480.f));
		}

		else if (fClockTime < 120.f)
		{
			pClockHand->SetTime(360.f);

			Player* pPlayer = (Player*)pDest->GetObj();

			pPlayer->SetHP((int)(100 * (360.f - fClockTime) / 480.f));
			pPlayer->SetEnergy((int)(100 * (360.f - fClockTime) / 480.f));
		}
	}
}
