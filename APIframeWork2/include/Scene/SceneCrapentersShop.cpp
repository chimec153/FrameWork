#include "SceneCrapentersShop.h"
#include "../Collider/ColliderRect.h"
#include "../Object/Stage.h"
#include "SceneManager.h"
#include "SceneTown.h"
#include "SceneCarpenterShop2.h"
#include "../Object/NPC.h"

SceneCrapentersShop::SceneCrapentersShop()
{
}

SceneCrapentersShop::~SceneCrapentersShop()
{
}

bool SceneCrapentersShop::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	if(m_pStage)
		m_pStage->LoadFromPath("CarpentersShop1.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	Layer* pLayer = FindLayer("Default");

	NPC* pNPC = Obj::CreateObj<NPC>("Robin", pLayer);

	pNPC->SetPos(32.f * 2.5f, 32.f * 5.5f);

	pNPC->SetAnimationCurrentClip("RobinIdleDown");

	SAFE_RELEASE(pNPC);

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("TownPortalBody");

	pRC->SetRect(32.f*4, 32.f * 11, 32.f*5, 32.f * 12);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneCrapentersShop::TownPortalCol);

	SAFE_RELEASE(pRC);

	ColliderRect* pShopRC = m_pStage->AddCollider<ColliderRect>("Shop2Body");

	pShopRC->SetRect(32.f * 3, 0.f, 32.f*4, 32.f);
	pShopRC->AddCollisionFunction(CS_ENTER, this, &SceneCrapentersShop::Shop2Col);

	SAFE_RELEASE(pShopRC);

	return true;
}

void SceneCrapentersShop::TownPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneTown>("Town", SC_NEXT);

		pDest->GetObj()->SetPos(32.f * 31.5f, 32.f * 47.5f);
	}
}

void SceneCrapentersShop::Shop2Col(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneCarpenterShop2>("CarpentersShop2", SC_NEXT);

		pDest->GetObj()->SetPos(32.f * 11.5f, 32.f * 12.5f);
	}
}
