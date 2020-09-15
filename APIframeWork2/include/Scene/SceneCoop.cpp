#include "SceneCoop.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderRect.h"
#include "InGameScene.h"
#include "../Sound/SoundManager.h"
#include "SceneManager.h"
#include "../Object/ObjManager.h"
#include "../Object/Chicken.h"
#include "../Object/Player.h"
#include "../Object/Tile.h"
#include "../Object/UIClockHand.h"
#include "../Object/Duck.h"
#include "../Object/Dinosaur.h"
#include "../Object/Rabbit.h"
#include "../Object/FarmEffect.h"

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

	CreateProtoTypes();

	ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pRC->SetRect(32.f * 2.f, 32.f * 11.f, 32.f * 3.f, 32.f * 12.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &SceneCoop::OutPortalCol);

	SAFE_RELEASE(pRC);

	Chicken* pWhiteChicken = CreateProtoType<Chicken>("Chick");

	SAFE_RELEASE(pWhiteChicken);

	Duck* pDuck = CreateProtoType<Duck>("Duck");

	SAFE_RELEASE(pDuck);

	Rabbit* pRabbit = CreateProtoType<Rabbit>("Rabbit");

	SAFE_RELEASE(pRabbit);

	Dinosaur* pDinosaur = CreateProtoType<Dinosaur>("Dinosaur");

	SAFE_RELEASE(pDinosaur);

	FarmEffect* pEffect = CreateProtoType<FarmEffect>("HoeEffect");

	SAFE_RELEASE(pEffect);

	return true;
}

int SceneCoop::Update(float fTime)
{
	Scene::Update(fTime);

	Layer* pLayer = FindLayer("Default");

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	int iChicken = pPlayer->GetAnimal(ANIMAL_CHICKEN);

	int iDuck = pPlayer->GetAnimal(ANIMAL_RABBIT);

	int iDinosaur = pPlayer->GetAnimal(ANIMAL_DINOSAUR);

	int iRabbit = pPlayer->GetAnimal(ANIMAL_RABBIT);

	if (iChicken > 0)
	{
		int iNumX = m_pStage->GetTileNumX();

		int iNumY = m_pStage->GetTileNumY();

		while (iChicken > 0)
		{
			TILE_OPTION eOption = TO_NONE;

			int iRandX = 0;
			int iRandY = 0;

			do
			{
				iRandX = rand() % iNumX;
				iRandY = rand() % iNumY;

				Tile* pTile = m_pStage->GetTile(iNumX * iRandY + iRandX);

				eOption = pTile->GetTileOption();

			} while (eOption != TO_NONE);

			Animal* pChicken = (Animal*)m_pStage->CreateCloneObj("Chick", "Chick", pLayer);

			pChicken->SetPos(32.f * iRandX, 32.f * iRandY);

			int iColor = rand() % CC_END;

			((Chicken*)pChicken)->SetColor((CHICKEN_COLOR)iColor);

			pChicken->SetAngle(1.f, 0.f);

			pChicken->SetSpeed(0.f);

			pChicken->ActionChange(AA_IDLE);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pChicken);

			SAFE_RELEASE(pChicken);

			--iChicken;

			pPlayer->LocateAnimal(ANIMAL_CHICKEN, 1);
		}
	}

	if (iDuck > 0)
	{
		int iNumX = m_pStage->GetTileNumX();

		int iNumY = m_pStage->GetTileNumY();

		while (iDuck > 0)
		{
			TILE_OPTION eOption = TO_NONE;

			int iRandX = 0;
			int iRandY = 0;

			do
			{
				iRandX = rand() % iNumX;
				iRandY = rand() % iNumY;

				Tile* pTile = m_pStage->GetTile(iNumX * iRandY + iRandX);

				eOption = pTile->GetTileOption();

			} while (eOption != TO_NONE);

			Animal* pDuck = (Animal*)m_pStage->CreateCloneObj("Duck", "Duck", pLayer);

			pDuck->SetPos(32.f * iRandX, 32.f * iRandY);

			pDuck->SetAngle(1.f, 0.f);

			pDuck->SetSpeed(0.f);

			pDuck->ActionChange(AA_IDLE);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pDuck);

			SAFE_RELEASE(pDuck);

			--iDuck;

			pPlayer->LocateAnimal(ANIMAL_DUCK, 1);
		}
	}

	if (iRabbit > 0)
	{
		int iNumX = m_pStage->GetTileNumX();

		int iNumY = m_pStage->GetTileNumY();

		while (iRabbit > 0)
		{
			TILE_OPTION eOption = TO_NONE;

			int iRandX = 0;
			int iRandY = 0;

			do
			{
				iRandX = rand() % iNumX;
				iRandY = rand() % iNumY;

				Tile* pTile = m_pStage->GetTile(iNumX * iRandY + iRandX);

				eOption = pTile->GetTileOption();

			} while (eOption != TO_NONE);

			Animal* pRabbit = (Animal*)m_pStage->CreateCloneObj("Rabbit", "Rabbit", pLayer);

			pRabbit->SetPos(32.f * iRandX, 32.f * iRandY);

			pRabbit->SetAngle(1.f, 0.f);

			pRabbit->SetSpeed(0.f);

			pRabbit->ActionChange(AA_IDLE);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pRabbit);

			SAFE_RELEASE(pRabbit);

			--iRabbit;

			pPlayer->LocateAnimal(ANIMAL_RABBIT, 1);
		}
	}

	if (iDinosaur > 0)
	{
		int iNumX = m_pStage->GetTileNumX();

		int iNumY = m_pStage->GetTileNumY();

		while (iDinosaur > 0)
		{
			TILE_OPTION eOption = TO_NONE;

			int iRandX = 0;
			int iRandY = 0;

			do
			{
				iRandX = rand() % iNumX;
				iRandY = rand() % iNumY;

				Tile* pTile = m_pStage->GetTile(iNumX * iRandY + iRandX);

				eOption = pTile->GetTileOption();

			} while (eOption != TO_NONE);

			Animal* pDinosaur = (Animal*)m_pStage->CreateCloneObj("Dinosaur", "Dinosaur", pLayer);

			pDinosaur->SetPos(32.f * iRandX, 32.f * iRandY);

			pDinosaur->SetAngle(1.f, 0.f);

			pDinosaur->SetSpeed(0.f);

			pDinosaur->ActionChange(AA_IDLE);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddAnimal(pDinosaur);

			SAFE_RELEASE(pDinosaur);

			--iDinosaur;

			pPlayer->LocateAnimal(ANIMAL_DINOSAUR, 1);
		}
	}

	SAFE_RELEASE(pPlayer);

	return 0;
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
