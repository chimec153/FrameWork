#include "SceneTown.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "SceneRoad.h"
#include "SceneShop.h"


SceneTown::SceneTown()
{
}

SceneTown::~SceneTown()
{
}

bool SceneTown::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("TownStage", pStageLayer);

	m_pStage->LoadFromPath("Town.tmp");

	if (!Scene::Init(POSITION(64.f, 32.f * 20.f)))
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pRoadPortal = m_pStage->AddCollider<ColliderRect>("RoadPortal");

	pRoadPortal->SetRect(32.f * 0.f, 32.f * 15.f, 32.f * 1.f, 32.f * 16.f);
	pRoadPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::RoadPortalCol);

	SAFE_RELEASE(pRoadPortal);

	ColliderRect* pShopPortal = m_pStage->AddCollider<ColliderRect>("ShopPortal");

	pShopPortal->SetRect(32.f * 29.f, 32.f * 27.f, 32.f *31.f, 32.f * 28.f);
	pShopPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::ShopPortalCol);

	SAFE_RELEASE(pShopPortal);

	return true;
}

void SceneTown::RoadPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneRoad>(SC_NEXT);
	}
}

void SceneTown::ShopPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneShop>(SC_NEXT);
	}
}
