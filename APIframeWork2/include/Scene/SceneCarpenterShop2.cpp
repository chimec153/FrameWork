#include "SceneCarpenterShop2.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "SceneManager.h"
#include "SceneCrapentersShop.h"
#include "../Object/NPC.h"

SceneCarpenterShop2::SceneCarpenterShop2()
{
}

SceneCarpenterShop2::~SceneCarpenterShop2()
{
}

bool SceneCarpenterShop2::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("CarpentersShopStage2", pStageLayer);

	m_pStage->LoadFromPath("CarpentersShop2.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	Layer* pLayer = FindLayer("Default");

	NPC* pNPC = m_pStage->CreateObj<NPC>("Haley", pLayer);

	pNPC->SetPos(32.f * 11, 32.f * 6);
	pNPC->SetSpeed(50.f);
	pNPC->SetAngle(1.f, 0.f);
	pNPC->SetAnimationCurrentClip("HaleyWalkRight");

	SAFE_RELEASE(pNPC);

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("Shop1PortalBody");

	pRC->SetRect(32.f * 11, 32.f * 15, 32.f * 12, 32.f * 16);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneCarpenterShop2::Shop1Col);

	SAFE_RELEASE(pRC);

	return true;
}

void SceneCarpenterShop2::Shop1Col(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneCrapentersShop>("CarpentersShop", SC_NEXT);

		pDest->GetObj()->SetPos(32.f * 3.5f, 32.f * 3.5f);
	}
}
