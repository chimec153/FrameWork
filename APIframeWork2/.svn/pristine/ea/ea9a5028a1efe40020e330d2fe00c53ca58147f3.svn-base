#include "SceneBarn.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "InGameScene.h"
#include "SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/ObjManager.h"

SceneBarn::SceneBarn()
{
}

SceneBarn::~SceneBarn()
{
}

bool SceneBarn::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->LoadFromPath("barn.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	if (!Scene::Init())
		return false;

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pRC->SetRect(32.f * 8.f, 32.f * 14.f, 32.f * 9.f, 32.f * 15.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneBarn::OutPortalCol);

	SAFE_RELEASE(pRC);

	return true;
}

void SceneBarn::OutPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 39.5f, 32.f * 17.5f);

		SAFE_RELEASE(pPlayer);
	}
}
