#include "InGameScene.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderPixel.h"
#include "../Sound/SoundManager.h"
#include "..//Core/PathManager.h"
#include "..//Resources/ResourcesManager.h"
#include "..//Resources/Texture.h"
#include "..//Animation/Animation.h"
#include "..//Collider/ColliderRect.h"
#include "..//Collider/ColliderSphere.h"
#include "..//Object/Player.h"
#include "..//Object/UIBar.h"
#include "../Object/UIPanel.h"
#include "SceneManager.h"
#include "SceneCave.h"
#include "../Object/Seed.h"
#include "SceneHome.h"
#include "../Object/PadeEffect.h"
#include "SceneRoad.h"
#include "../Object/ObjManager.h"
#include "../Object/UIInventory.h"
#include "../Object/Tool.h"
#include "SceneCoop.h"
#include "SceneBarn.h"
#include "../Object/Horse.h"
#include "../Object/UIClockHand.h"

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
	if (!Scene::Init())
		return false;

	GET_SINGLE(SoundManager)->Play("RoadBGM");

	SetEditMode(false);

	Texture* pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("Tile2", TEXT("Maps\\walls_and_floors.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("springoutdoor", TEXT("Maps\\spring_outdoorsTileSheet.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("mine", TEXT("Maps\\TheMines.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("summeroutdoor", TEXT("Maps\\summer_outdoors.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("falloutdoor", TEXT("Maps\\fall_outdoors.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("winteroutdoor", TEXT("Maps\\winter_outdoors.bmp"));
	SAFE_RELEASE(pTexture);

	Texture* pNoneTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNone", L"TileSheets\\Move.bmp");
	pNoneTex->SetColorKey(0, 32, 0);
	SAFE_RELEASE(pNoneTex);
	Texture* pNoMoveTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNoMove", L"TileSheets\\Nomove.bmp");
	pNoMoveTex->SetColorKey(0, 32, 0);

	SAFE_RELEASE(pNoMoveTex);

	LoadFile();

	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->LoadFromPath("200808.tmp");

	m_pStage->SetStageType(STAGE_OUTDOOR);

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pPortal = m_pStage->AddCollider<ColliderRect>("CavePortal");

	pPortal->SetRect(32.f * 23.f, 32.f * 15.f, 32.f * 24.f, 32.f * 16.f);
	pPortal->AddCollisionFunction(CS_ENTER, this, &InGameScene::Cave);

	SAFE_RELEASE(pPortal);

	ColliderRect* pHomePortal = m_pStage->AddCollider<ColliderRect>("HomePortal");

	pHomePortal->SetRect(32.f * 6.f, 32.f * 10.f, 32.f * 7.f, 32.f * 11.f);
	pHomePortal->AddCollisionFunction(CS_ENTER, this, &InGameScene::HomePortalCol);

	SAFE_RELEASE(pHomePortal);

	ColliderRect* pRoadPortal = m_pStage->AddCollider<ColliderRect>("RoadPortal");

	pRoadPortal->SetRect(32.f * 49.f, 32.f * 15.f, 32.f * 50.f, 32.f * 16.f);
	pRoadPortal->AddCollisionFunction(CS_ENTER, this, &InGameScene::RoadPortalCol);

	SAFE_RELEASE(pRoadPortal);

	Layer* pHUDLayer = FindLayer("HUD");

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	pPlayer->AddGold(100000);

	SAFE_RELEASE(pPlayer);

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	for (int i = 0; i < 20; ++i)
	{
		Item* pHay = (Item*)pInven->Obj::CreateCloneObj("Hay", "Hay", pHUDLayer);

		pInven->AddItem(pHay);

		SAFE_RELEASE(pHay);
	}

	SAFE_RELEASE(pInven);

	return true;
}

int InGameScene::Update(float fTime)
{
	Scene::Update(fTime);

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	if (pPlayer->GetBuilding(BT_COOP))
	{
		Layer* pLayer = FindLayer("Default");

		Obj* pObj = m_pStage->CreateCloneObj("Coop", "Coop", pLayer);

		pObj->SetPos(32.f * 44.5f, 32.f * 12.5f);

		pPlayer->BuildComplete(BT_COOP);

		Collider* pRC = pObj->GetCollider("DoorBody");

		pRC->AddCollisionFunction(CS_ENTER, this, &InGameScene::CoopPortalCol);

		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pObj);
	}

	if (pPlayer->GetBuilding(BT_BARN))
	{
		Layer* pLayer = FindLayer("Default");

		Obj* pObj = m_pStage->CreateCloneObj("Barn", "Barn", pLayer);

		pObj->SetPos(32.f * 38.5f, 32.f * 11.5f);

		pPlayer->BuildComplete(BT_BARN);

		Collider* pRC = pObj->GetCollider("DoorBody");

		pRC->AddCollisionFunction(CS_ENTER, this, &InGameScene::BarnPortalCol);

		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pObj);
	}

	if (pPlayer->GetBuilding(BT_STABLE))
	{
		Layer* pLayer = FindLayer("Default");

		Obj* pObj = m_pStage->CreateCloneObj("Stable", "Stable", pLayer);

		pObj->SetPos(32.f * 28.5f, 32.f * 11.5f);

		pPlayer->BuildComplete(BT_STABLE);

		SAFE_RELEASE(pObj);

		Horse* pHorse = Obj::CreateObj<Horse>("Horse", pLayer);

		pHorse->SetPos(32.f * 28.5f, 32.f * 13.f);		

		SAFE_RELEASE(pHorse);
	}

	if (pPlayer->GetBuilding(BT_SILO))
	{
		Layer* pLayer = FindLayer("Default");

		Obj* pObj = m_pStage->CreateCloneObj("Silo", "Silo", pLayer);

		pObj->SetPos(32.f * 20.5f, 32.f * 11.5f);

		pPlayer->BuildComplete(BT_SILO);

		SAFE_RELEASE(pObj);
	}

	SAFE_RELEASE(pPlayer);

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
	}

	return 0;
}

void InGameScene::Cave(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		SceneCave* pSceneCave = GET_SINGLE(SceneManager)->CreateScene<SceneCave>("Cave", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 5.5f, 32.f * 5.5f);

		SAFE_RELEASE(pPlayer);
	}
}

void InGameScene::HomePortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneHome>("Home", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 3.5f, 32.f * 9.f);

		SAFE_RELEASE(pPlayer);
	}
}

void InGameScene::RoadPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneRoad>("Road", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(80.f, 334.f);

		SAFE_RELEASE(pPlayer);
	}
}

void InGameScene::CoopPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneCoop>("Coop", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 2.5f, 32.f * 10);

		SAFE_RELEASE(pPlayer);
	}
}

void InGameScene::BarnPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneBarn>("Barn", SC_NEXT);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetPos(32.f * 8.5f, 32.f * 12.5f);

		SAFE_RELEASE(pPlayer);
	}
}
