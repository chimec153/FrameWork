#include "SceneShop.h"
#include "SceneTown.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/NPC.h"
#include "../Object/ObjManager.h"
#include "../Core/Camera.h"
#include "../Object/UIPanel.h"

SceneShop::SceneShop()
{
}

SceneShop::~SceneShop()
{
}

bool SceneShop::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("TownStage", pStageLayer);

	m_pStage->LoadFromPath("Shop.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pTownPortal = m_pStage->AddCollider<ColliderRect>("TownPortal");

	pTownPortal->SetRect(32.f * 6.f, 32.f * 18.f, 32.f * 7.f, 32.f * 19.f);
	pTownPortal->AddCollisionFunction(CS_ENTER, this, &SceneShop::TownPortalCol);

	SAFE_RELEASE(pTownPortal);

	Layer* pLayer = FindLayer("Default");

	NPC* pNPC = Obj::CreateObj<NPC>("Harvey", pLayer);

	pNPC->SetPos(32.f * 7.5f, 32.f * 4.5f);
	pNPC->SetSpeed(0.f);
	pNPC->SetAnimationCurrentClip("HarveyIdleDown");

	SAFE_RELEASE(pNPC);

	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	Layer* pBackStageLayer = FindLayer("BackStage");

	UIPanel* pBackStage = Obj::CreateObj<UIPanel>("backstage", pBackStageLayer);

	pBackStage->SetTexture("back", TEXT("AnimationUI\\BackGround.bmp"));

	pBackStage->SetSize((float)tRS.iW, (float)tRS.iH);

	pBackStage->SetPos(0.f, 0.f);

	SAFE_RELEASE(pBackStage);

	return true;
}

void SceneShop::TownPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneTown>("Town", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(960.f, 928.f);

		SAFE_RELEASE(pPlayer);
	}
}
