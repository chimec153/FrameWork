#include "SceneCoop.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "InGameScene.h"
#include "../Sound/SoundManager.h"
#include "SceneManager.h"
#include "../Object/ObjManager.h"
#include "../Object/Chicken.h"

SceneCoop::SceneCoop()
{
}

SceneCoop::~SceneCoop()
{
}

bool SceneCoop::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->LoadFromPath("coop.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pRC->SetRect(32.f * 2.f, 32.f * 11.f, 32.f * 3.f, 32.f * 12.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneCoop::OutPortalCol);

	SAFE_RELEASE(pRC);

	Layer* pLayer = FindLayer("Default");

	Chicken* pChicken = m_pStage->CreateObj<Chicken>("BlackChicken", pLayer);

	pChicken->SetPos(200.f, 200.f);

	SAFE_RELEASE(pChicken);

	return true;
}

void SceneCoop::OutPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 44.5f, 32.f * 16.5f);

		SAFE_RELEASE(pPlayer);
	}
}
