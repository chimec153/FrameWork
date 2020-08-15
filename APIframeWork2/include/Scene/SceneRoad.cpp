#include "SceneRoad.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "InGameScene.h"
#include "SceneTown.h"

SceneRoad::SceneRoad()
{
}

SceneRoad::~SceneRoad()
{
}

bool SceneRoad::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("RoadStage", pStageLayer);

	m_pStage->LoadFromPath("Road.tmp");

	if (!Scene::Init(POSITION(64.f, 32.f * 15.f)))
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pFarmPortal = m_pStage->AddCollider<ColliderRect>("FarmPortal");

	pFarmPortal->SetRect(32.f * 0.f, 32.f * 15.f, 32.f * 1.f, 32.f * 16.f);
	pFarmPortal->AddCollisionFunction(CS_ENTER, this, &SceneRoad::FarmPortal);

	SAFE_RELEASE(pFarmPortal);

	ColliderRect* pTownPortal = m_pStage->AddCollider<ColliderRect>("TownPortal");

	pTownPortal->SetRect(32.f * 49.f, 32.f * 15.f, 32.f * 50.f, 32.f * 16.f);
	pTownPortal->AddCollisionFunction(CS_ENTER, this, &SceneRoad::TownPortal);

	SAFE_RELEASE(pTownPortal);

	return true;
}

void SceneRoad::TownPortal(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);
		
		GET_SINGLE(SceneManager)->CreateScene<SceneTown>(SC_NEXT);
	}
}

void SceneRoad::FarmPortal(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<InGameScene>(SC_NEXT);
	}
}
