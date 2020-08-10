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

InGameScene::InGameScene()	:
	m_pStage(nullptr)
{
}

InGameScene::~InGameScene()
{
	SAFE_RELEASE(m_pStage);
}

bool InGameScene::Init()
{
	GET_SINGLE(SoundManager)->LoadSound("RoadBGM", true, "RoadBgm.wav");
	//GET_SINGLE(SoundManager)->Play("RoadBGM");

	SetEditMode(false);

	if (!Scene::Init())
		return false;

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

	m_pStage->CreateTile(100, 100, 32, 32, "Tiles", L"TileSheets\\Flooring.bmp");

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

		Layer* pLayer = FindLayer("Default");

		if (pLayer)
			pLayer->Load(pLoadFile);

		fclose(pLoadFile);
	}

	Player* pPlayer = (Player*)CreatePlayer();

	CreateProtoNumberSmall();

	//CreateBatProto();

	//CreateBatClone();

	//CreateSlimeProto();

	//CreateSlimeClone();

	CreateUI(pPlayer);

	SAFE_RELEASE(pPlayer);

	ColliderRect* pPortal = m_pStage->AddCollider<ColliderRect>("CavePortal");

	pPortal->SetRect(32.f * 10.f, 32.f * 10.f, 32.f * 11.f, 32.f * 11.f);
	pPortal->AddCollisionFunction(CS_ENTER, this, &InGameScene::Cave);

	SAFE_RELEASE(pPortal);

	Layer* pLayer = FindLayer("Default");

	Item* pSeed = Obj::CreateObj<Seed>("PotatoSeed", pLayer);
	
	pSeed->SetImageOffset(pSeed->GetSize() * POSITION(19.f, 19.f));
	pSeed->SetPos(512.f, 512.f);

	Collider* pCol = pSeed->GetCollider("ItemBody");

	pCol->AddCollisionFunction(CS_ENTER, pSeed, &Item::CollEnter);

	SAFE_RELEASE(pCol);
	
	SAFE_RELEASE(pSeed);

	return true;
}

void InGameScene::Cave(Collider* pSrc, Collider* pDest, float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);

	if (pDest->GetTag() == "PlayerBody")
		GET_SINGLE(SceneManager)->CreateScene<SceneCave>(SC_NEXT);
}
