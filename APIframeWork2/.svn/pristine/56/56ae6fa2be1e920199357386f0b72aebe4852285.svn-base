#include "SceneWeaponShop.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "SceneTown.h"
#include "SceneManager.h"
#include "../Object/NPC.h"

SceneWeaponShop::SceneWeaponShop()
{
}

SceneWeaponShop::~SceneWeaponShop()
{
}

bool SceneWeaponShop::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("GuildStage", pStageLayer);

	m_pStage->LoadFromPath("AdvantureGuild.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	Layer* pLayer = FindLayer("Default");

	NPC* pMarlon = Obj::CreateObj<NPC>("Marlon", pLayer);

	pMarlon->SetAnimationCurrentClip("MarlonIdleDown");
	pMarlon->SetPos(176.f, 336.f);

	SAFE_RELEASE(pMarlon);

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("TownPortalBody");

	pRC->SetRect(32.f * 6, 32.f * 20, 32.f * 7, 32.f * 21);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneWeaponShop::TownPortalCol);

	SAFE_RELEASE(pRC);

	return true;
}

void SceneWeaponShop::TownPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneTown>("Town", SC_NEXT);

		pDest->GetObj()->SetPos(32.f * 3.5f, 32.f * 38.5f);
	}
}
