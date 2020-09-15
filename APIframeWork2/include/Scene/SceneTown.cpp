#include "SceneTown.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "SceneRoad.h"
#include "SceneShop.h"
#include "../Object/ObjManager.h"
#include "SceneWeaponShop.h"
#include "SceneCrapentersShop.h"
#include "../Core/Camera.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../Object/UIClockHand.h"

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

	m_pStage->SetStageType(STAGE_OUTDOOR);

	if (!Scene::Init())//POSITION(64.f, 32.f * 20.f)))
		return false;

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pRoadPortal = m_pStage->AddCollider<ColliderRect>("RoadPortalBody");

	pRoadPortal->SetRect(32.f * 0.f, 32.f * 25.f, 32.f * 1.f, 32.f * 26.f);
	pRoadPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::RoadPortalCol);

	SAFE_RELEASE(pRoadPortal);

	ColliderRect* pShopPortal = m_pStage->AddCollider<ColliderRect>("ShopPortalBody");

	pShopPortal->SetRect(32.f * 29.f, 32.f * 26.f, 32.f * 31.f, 32.f * 27.f);
	pShopPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::ShopPortalCol);

	SAFE_RELEASE(pShopPortal);

	ColliderRect* pWeaponShopPortal = m_pStage->AddCollider<ColliderRect>("WeaponShopPortalBody");

	pWeaponShopPortal->SetRect(32.f * 3.f, 32.f * 35.f, 32.f * 4.f, 32.f * 36.f);
	pWeaponShopPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::WeaponShopPortalCol);

	SAFE_RELEASE(pWeaponShopPortal);

	ColliderRect* pCarpentersShopPortal = m_pStage->AddCollider<ColliderRect>("CarpentersShopPortalBody");

	pCarpentersShopPortal->SetRect(32.f * 31.f, 32.f * 44.f, 32.f * 32.f, 32.f * 45.f);
	pCarpentersShopPortal->AddCollisionFunction(CS_ENTER, this, &SceneTown::CarpentersShopPortalCol);

	SAFE_RELEASE(pCarpentersShopPortal);

	return true;
}

int SceneTown::Update(float fTime)
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

void SceneTown::RoadPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneRoad>("Road", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(1520.f, 334.f);

		SAFE_RELEASE(pPlayer);
	}
}

void SceneTown::ShopPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneShop>("Shop", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 6.5f, 32.f * 17.f);

		SAFE_RELEASE(pPlayer);
	}
}

void SceneTown::WeaponShopPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneWeaponShop>("WeaponShop", SC_NEXT);

		GET_SINGLE(Camera)->SetPos(0.f, 0.f);

		pDest->GetObj()->SetPos(32.f * 6.5f, 32.f * 17.5f);
	}
}

void SceneTown::CarpentersShopPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneCrapentersShop>("CarpentersShop", SC_NEXT);

		pDest->GetObj()->SetPos(32.f * 4.5f, 32.f * 9.5f);
	}
}
