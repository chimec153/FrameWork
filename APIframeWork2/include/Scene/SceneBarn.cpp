#include "SceneBarn.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "InGameScene.h"
#include "SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/ObjManager.h"
#include "../Object/Cow.h"
#include "../Object/Sheep.h"
#include "../Object/Pig.h"
#include "../Object/Goat.h"
#include "../Object/Player.h"
#include "../Object/UIClockHand.h"
#include "../Object/Tile.h"
#include "../Object/FarmEffect.h"

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

	CreateProtoTypes();

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pRC->SetRect(32.f * 8.f, 32.f * 14.f, 32.f * 9.f, 32.f * 15.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneBarn::OutPortalCol);

	SAFE_RELEASE(pRC);

	Goat* pGoat = CreateProtoType<Goat>("Goat");

	SAFE_RELEASE(pGoat);

	Pig* pPig = CreateProtoType<Pig>("Pig");

	SAFE_RELEASE(pPig);

	Sheep* pSheep = CreateProtoType<Sheep>("Sheep");

	SAFE_RELEASE(pSheep);

	Cow* pCow = CreateProtoType<Cow>("Cow");

	SAFE_RELEASE(pCow);

	FarmEffect* pEffect = CreateProtoType<FarmEffect>("HoeEffect");

	SAFE_RELEASE(pEffect);

	return true;
}

int SceneBarn::Update(float fTime)
{
	Scene::Update(fTime);

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	int iBrownCow  = pPlayer->GetAnimal(ANIMAL_BROWNCOW);

	while (iBrownCow > 0)
	{
		int iTileNumX = m_pStage->GetTileNumX();

		int iRandX = rand() % iTileNumX;
		int iRandY = rand() % m_pStage->GetTileNumY();

		Tile* pTile = m_pStage->GetTile(iRandX + iRandY * iTileNumX);

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption != TO_NOMOVE)
		{
			Layer* pLayer = FindLayer("Default");

			Animal* pBrownCow = (Animal*)pPlayer->Obj::CreateCloneObj("Cow", "BrownCow", pLayer);

			int iTileSizeX = m_pStage->GetTileSizeX();
			int iTileSizeY = m_pStage->GetTileSizeY();

			pBrownCow->SetPos((float)(iRandX * iTileSizeX), (float)(iRandY * iTileSizeY));

			pBrownCow->ActionChange(AA_IDLE);

			pBrownCow->SetAngle(1.f, 0.f);

			pBrownCow->SetSpeed(0.f);

			Collider* pAnimalBody = pBrownCow->GetCollider("AnimalBody");

			pAnimalBody->AddCollisionFunction(CS_ENTER, pBrownCow, &Animal::ColEnter);
			pAnimalBody->AddCollisionFunction(CS_STAY, pBrownCow, &Animal::ColEnter);

			SAFE_RELEASE(pAnimalBody);

			((Cow*)pBrownCow)->SetColor(COW_BROWN);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pBrownCow);

			SAFE_RELEASE(pBrownCow);

			pPlayer->LocateAnimal(ANIMAL_BROWNCOW, 1);

			--iBrownCow;
		}
	}

	int iMilkCow = pPlayer->GetAnimal(ANIMAL_MILKCOW);

	while (iMilkCow > 0)
	{
		int iTileNumX = m_pStage->GetTileNumX();

		int iRandX = rand() % iTileNumX;
		int iRandY = rand() % m_pStage->GetTileNumY();

		Tile* pTile = m_pStage->GetTile(iRandX + iRandY * iTileNumX);

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption != TO_NOMOVE)
		{
			Layer* pLayer = FindLayer("Default");

			Animal* pWhiteCow = (Animal*)pPlayer->Obj::CreateCloneObj("Cow", "WhiteCow", pLayer);

			int iTileSizeX = m_pStage->GetTileSizeX();
			int iTileSizeY = m_pStage->GetTileSizeY();

			pWhiteCow->SetPos((float)(iRandX * iTileSizeX), (float)(iRandY * iTileSizeY));

			pWhiteCow->ActionChange(AA_IDLE);

			pWhiteCow->SetAngle(1.f, 0.f);

			pWhiteCow->SetSpeed(0.f);

			Collider* pAnimalBody = pWhiteCow->GetCollider("AnimalBody");

			pAnimalBody->AddCollisionFunction(CS_ENTER, pWhiteCow, &Animal::ColEnter);
			pAnimalBody->AddCollisionFunction(CS_STAY, pWhiteCow, &Animal::ColEnter);

			SAFE_RELEASE(pAnimalBody);

			((Cow*)pWhiteCow)->SetColor(COW_WHITE);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pWhiteCow);

			SAFE_RELEASE(pWhiteCow);

			pPlayer->LocateAnimal(ANIMAL_MILKCOW, 1);

			--iMilkCow;
		}
	}

	int iGoat = pPlayer->GetAnimal(ANIMAL_GOAT);

	while (iGoat > 0)
	{
		int iTileNumX = m_pStage->GetTileNumX();

		int iRandX = rand() % iTileNumX;
		int iRandY = rand() % m_pStage->GetTileNumY();

		Tile* pTile = m_pStage->GetTile(iRandX + iRandY * iTileNumX);

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption != TO_NOMOVE)
		{
			Layer* pLayer = FindLayer("Default");

			Animal* pGoat = (Animal*)pPlayer->Obj::CreateCloneObj("Goat", "Goat", pLayer);

			int iTileSizeX = m_pStage->GetTileSizeX();
			int iTileSizeY = m_pStage->GetTileSizeY();

			pGoat->SetPos((float)(iRandX * iTileSizeX), (float)(iRandY * iTileSizeY));

			pGoat->SetSpeed(0.f);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pGoat);

			SAFE_RELEASE(pGoat);

			pPlayer->LocateAnimal(ANIMAL_GOAT, 1);

			--iGoat;
		}
	}

	int iSheep = pPlayer->GetAnimal(ANIMAL_SHEEP);

	while (iSheep > 0)
	{
		int iTileNumX = m_pStage->GetTileNumX();

		int iRandX = rand() % iTileNumX;
		int iRandY = rand() % m_pStage->GetTileNumY();

		Tile* pTile = m_pStage->GetTile(iRandX + iRandY * iTileNumX);

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption != TO_NOMOVE)
		{
			Layer* pLayer = FindLayer("Default");

			Animal* pSheep = (Animal*)pPlayer->Obj::CreateCloneObj("Sheep", "Sheep", pLayer);

			int iTileSizeX = m_pStage->GetTileSizeX();
			int iTileSizeY = m_pStage->GetTileSizeY();

			pSheep->SetPos((float)(iRandX * iTileSizeX), (float)(iRandY * iTileSizeY));

			pSheep->SetSpeed(0.f);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pSheep);

			SAFE_RELEASE(pSheep);

			pPlayer->LocateAnimal(ANIMAL_SHEEP, 1);

			--iSheep;
		}
	}

	int iPig = pPlayer->GetAnimal(ANIMAL_PIG);

	while (iPig > 0)
	{
		int iTileNumX = m_pStage->GetTileNumX();

		int iRandX = rand() % iTileNumX;
		int iRandY = rand() % m_pStage->GetTileNumY();

		Tile* pTile = m_pStage->GetTile(iRandX + iRandY * iTileNumX);

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption != TO_NOMOVE)
		{
			Layer* pLayer = FindLayer("Default");

			Animal* pPig = (Animal*)pPlayer->Obj::CreateCloneObj("Pig", "Pig", pLayer);

			int iTileSizeX = m_pStage->GetTileSizeX();
			int iTileSizeY = m_pStage->GetTileSizeY();

			pPig->SetPos((float)(iRandX * iTileSizeX), (float)(iRandY * iTileSizeY));

			pPig->SetSpeed(0.f);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pPig);

			SAFE_RELEASE(pPig);

			pPlayer->LocateAnimal(ANIMAL_PIG, 1);

			--iPig;
		}
	}

	SAFE_RELEASE(pPlayer);

	return 0;
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
