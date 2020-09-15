#include "SceneRoad.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "InGameScene.h"
#include "SceneTown.h"
#include "../Object/ObjManager.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../Object/UIClockHand.h"

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

	m_pStage->SetStageType(STAGE_OUTDOOR);

	if (!Scene::Init())
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pFarmPortal = m_pStage->AddCollider<ColliderRect>("FarmPortal");

	pFarmPortal->SetRect(32.f * 0.f, 32.f * 10.f, 32.f * 1.f, 32.f * 11.f);
	pFarmPortal->AddCollisionFunction(CS_ENTER, this, &SceneRoad::FarmPortal);

	SAFE_RELEASE(pFarmPortal);

	ColliderRect* pTownPortal = m_pStage->AddCollider<ColliderRect>("TownPortal");

	pTownPortal->SetRect(32.f * 49.f, 32.f * 10.f, 32.f * 50.f, 32.f * 11.f);
	pTownPortal->AddCollisionFunction(CS_ENTER, this, &SceneRoad::TownPortal);

	SAFE_RELEASE(pTownPortal);

	return true;
}

int SceneRoad::Update(float fTime)
{
	Scene::Update(fTime);

	UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

	int iDay = pClockHand->GetDay() % 365;

	SEASON eSeason = m_pStage->GetSeason();

	if (iDay > 90 && eSeason == SEASON_SPRING)
	{
		m_pStage->SetSeason(SEASON_SUMMER);

		Texture* pTexture = GET_SINGLE(ResourcesManager)->FindTexture("summeroutdoor");

		Texture* pCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("springoutdoor");

		Texture* pUpperTexture = GET_SINGLE(ResourcesManager)->FindTexture("spring_outdoorsTileSheet");

		m_pStage->ChangeTileTextureAll(pUpperTexture, pTexture);
		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture);

		m_pStage->ChangeTileTextureAll(pUpperTexture, pTexture, 1);
		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture, 1);

		SAFE_RELEASE(pUpperTexture);

		SAFE_RELEASE(pCurTexture);

		SAFE_RELEASE(pTexture);

		Texture* pTownTexture = GET_SINGLE(ResourcesManager)->LoadTexture("summer_town", TEXT("Maps\\summer_town.bmp"));

		Texture* pTownCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("spring_town");

		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture);
		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture, 1);

		SAFE_RELEASE(pTownCurTexture);

		SAFE_RELEASE(pTownTexture);
	}

	else if (iDay > 180 && eSeason == SEASON_SUMMER)
	{
		m_pStage->SetSeason(SEASON_FALL);

		Texture* pTexture = GET_SINGLE(ResourcesManager)->FindTexture("falloutdoor");

		Texture* pCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("summeroutdoor");

		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture);
		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture, 1);

		SAFE_RELEASE(pCurTexture);

		SAFE_RELEASE(pTexture);

		Texture* pTownTexture = GET_SINGLE(ResourcesManager)->LoadTexture("fall_town", TEXT("Maps\\fall_town.bmp"));

		Texture* pTownCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("summer_town");

		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture);
		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture, 1);

		SAFE_RELEASE(pTownCurTexture);

		SAFE_RELEASE(pTownTexture);
	}

	else if (iDay > 270 && eSeason == SEASON_FALL)
	{
		m_pStage->SetSeason(SEASON_WINTER);

		Texture* pTexture = GET_SINGLE(ResourcesManager)->FindTexture("winteroutdoor");

		Texture* pCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("falloutdoor");

		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture);
		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture, 1);

		SAFE_RELEASE(pCurTexture);

		SAFE_RELEASE(pTexture);

		Texture* pTownTexture = GET_SINGLE(ResourcesManager)->LoadTexture("winter_town", TEXT("Maps\\winter_town.bmp"));

		Texture* pTownCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("fall_town");

		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture);
		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture, 1);

		SAFE_RELEASE(pTownCurTexture);

		SAFE_RELEASE(pTownTexture);
	}

	else if (iDay <= 90 && eSeason == SEASON_WINTER)
	{
		m_pStage->SetSeason(SEASON_SPRING);

		Texture* pTexture = GET_SINGLE(ResourcesManager)->FindTexture("springoutdoor");

		Texture* pCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("winteroutdoor");

		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture);
		m_pStage->ChangeTileTextureAll(pCurTexture, pTexture, 1);

		SAFE_RELEASE(pCurTexture);

		SAFE_RELEASE(pTexture);

		Texture* pTownTexture = GET_SINGLE(ResourcesManager)->LoadTexture("spring_town", TEXT("Maps\\spring_town.bmp"));

		Texture* pTownCurTexture = GET_SINGLE(ResourcesManager)->FindTexture("winter_town");

		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture);
		m_pStage->ChangeTileTextureAll(pTownCurTexture, pTownTexture, 1);

		SAFE_RELEASE(pTownCurTexture);

		SAFE_RELEASE(pTownTexture);
	}

	return 0;
}

void SceneRoad::TownPortal(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);
		
		GET_SINGLE(SceneManager)->CreateScene<SceneTown>("Town", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(64.f, 816.f);

		SAFE_RELEASE(pPlayer);
	}
}

void SceneRoad::FarmPortal(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(1520.f, 560.f);

		SAFE_RELEASE(pPlayer);
	}
}
