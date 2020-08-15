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

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
	if (!Scene::Init(POSITION(32.f * 8.f, 32.f * 14.f)))
		return false;

	GET_SINGLE(SoundManager)->Play("RoadBGM");

	SetEditMode(false);

	Texture* pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("Tile2", TEXT("Maps\\walls_and_floors.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("springoutdoor", TEXT("Maps\\spring_outdoorsTileSheet.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("mine", TEXT("Maps\\TheMines.bmp"));
	SAFE_RELEASE(pTexture);

	GET_SINGLE(Camera)->SetWorldResolution(3200, 3200);

	Texture* pNoneTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNone", L"TileSheets\\Move.bmp");
	pNoneTex->SetColorKey(0, 32, 0);
	SAFE_RELEASE(pNoneTex);
	Texture* pNoMoveTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNoMove", L"TileSheets\\Nomove.bmp");
	pNoMoveTex->SetColorKey(0, 32, 0);

	SAFE_RELEASE(pNoMoveTex);

	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	char	strFileName[MAX_PATH] = {};

	const char* strRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	if (strRootPath)
		strcat_s(strFileName, strRootPath);

	strcat_s(strFileName, "200808.tmp");

	FILE* pLoadFile = nullptr;

	fopen_s(&pLoadFile, strFileName, "rb");

	if (pLoadFile)
	{
		if (m_pStage)
			m_pStage->Load(pLoadFile);

		fclose(pLoadFile);
	}

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

	pRoadPortal->SetRect(32.f * 49.f, 32.f * 10.f, 32.f * 50.f, 32.f * 11.f);
	pRoadPortal->AddCollisionFunction(CS_ENTER, this, &InGameScene::RoadPortalCol);

	SAFE_RELEASE(pRoadPortal);

	Layer* pLayer = FindLayer("Default");

	for (int i = 0; i < 5; ++i)
	{
		Item* pSeed = (Item*)Obj::CreateCloneObj("PotatoSeed", "PotatoSeed", SC_NEXT, pLayer);

		pSeed->SetPos(600.f + 100 * i, 600.f);

		Collider* pCol = pSeed->GetCollider("ItemBody");

		pCol->AddCollisionFunction(CS_ENTER, pSeed, &Item::CollEnter);
		pCol->AddCollisionFunction(CS_STAY, pSeed, &Item::ColStay);
		pCol->AddCollisionFunction(CS_LEAVE, pSeed, &Item::ColEnd);

		SAFE_RELEASE(pCol);

		SAFE_RELEASE(pSeed);
	}

	return true;
}

void InGameScene::Cave(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneCave>(SC_NEXT);
	}
}

void InGameScene::HomePortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneHome>(SC_NEXT);
	}
}

void InGameScene::RoadPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		GET_SINGLE(SoundManager)->Stop(ST_BGM);

		GET_SINGLE(SceneManager)->CreateScene<SceneRoad>(SC_NEXT);
	}
}
