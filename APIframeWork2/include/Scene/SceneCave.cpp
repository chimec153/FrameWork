#include "SceneCave.h"
#include "../Core/Camera.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Collider/ColliderSphere.h"
#include "../Object/Minion.h"
#include "../Object/UIBar.h"
#include "../Object/UIPanel.h"
#include "SceneManager.h"
#include "../Core/PathManager.h"
#include "..//Object/Effect.h"
#include "..//Collider/ColliderRect.h"
#include "..//Object/Slime.h"
#include "../Animation/Animation.h"
#include "InGameScene.h"
#include "../Object/Tile.h"
#include "../Sound/SoundManager.h"
#include "../Object/UINum.h"
#include "../Core/Camera.h"
#include "../Object/Portal.h"
#include "../Object/ObjManager.h"
#include "../Object/UIInventory.h"
#include "../Object/Fly.h"
#include "../Object/Bug.h"
#include "../Object/SlimeHead.h"
#include "../Resources/ResourcesManager.h"

SceneCave::SceneCave()	:
	m_iLevel(0),
	m_pLevelNum(nullptr)
{
}

SceneCave::~SceneCave()
{
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pLevelNum);
}

void SceneCave::SetLevel(int iLevel)
{
	m_iLevel = iLevel;
}

bool SceneCave::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	char	strFileName[MAX_PATH] = {};

	const char* strRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	if (strRootPath)
		strcat_s(strFileName, strRootPath);

	strcat_s(strFileName, "cave.tmp");

	m_pStage->SetStageType(STAGE_INDOOR);

	FILE* pLoadFile = nullptr;

	fopen_s(&pLoadFile, strFileName, "rb");

	if (pLoadFile)
	{
		if (m_pStage)
			m_pStage->Load(pLoadFile);

		fclose(pLoadFile);
	}

	if (!Scene::Init())
		return false;

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	Layer* pHUDLayer = FindLayer("HUD");

	Layer* pUILayer = FindLayer("UI");

	UIPanel* pLevelPanel = Obj::CreateObj<UIPanel>("LevelPanel", pHUDLayer, POSITION(50.f, 50.f), POSITION(50.f, 36.f));

	pLevelPanel->SetTexture("Mouse");
	pLevelPanel->SetImageOffset(650.f, 636.f);
	
	m_pLevelNum = (UINum*)m_pStage->CreateCloneObj("Num", "Num", pUILayer);

	m_pLevelNum->CreateNum(m_iLevel);
	m_pLevelNum->SetSpeed(0.f);

	m_pLevelNum->SetPosAll(POSITION(70.f, 62.f) + tCamPos);

	SAFE_RELEASE(pLevelPanel);

	CreateFarmEffect();

	CreateProtoTypes();

	ColliderRect* pPortal = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pPortal->SetRect(32.f * 5.f, 32.f * 2.f, 32.f * 6.f, 32.f * 3.f);
	pPortal->AddCollisionFunction(CS_ENTER, this, &SceneCave::OutPortalCol);

	SAFE_RELEASE(pPortal); 
	
	LoadFile();

	return true;
}

void SceneCave::Start()
{
	Scene::Start();

	CreateMonsters();

	CreateRocks();
}

int SceneCave::Update(float fTime)
{
	Scene::Update(fTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	m_pLevelNum->CreateNum(m_iLevel);

	m_pLevelNum->SetPosAll(POSITION(70.f, 62.f) + tCamPos);

	return 0;
}

void SceneCave::OutPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "PlayerBody")
	{
		if (m_iLevel == 0)
		{
			GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main",SC_NEXT);

			Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

			pPlayer->SetPos(752.f, 576.f);

			SAFE_RELEASE(pPlayer);
		}

		else
		{
			char strNum[32] = {};

			int iLevel = m_iLevel - 1;

			int iSize = sizeof(strNum) / sizeof(strNum[0]);

			for (int i=0;iLevel >=10;++i)
			{
				strNum[iSize - i - 1] = iLevel % 10;

				iLevel /= 10;
			}

			char strName[32] = {};

			if (m_iLevel != 1)
				sprintf_s(strName, "Cave_%d", m_iLevel - 1);

			else
				strcat(strName, "Cave");

			SceneCave* pScene = GET_SINGLE(SceneManager)->CreateScene<SceneCave>(strName, SC_NEXT);

			pScene->SetLevel(m_iLevel - 1);

			Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

			pPlayer->SetPos(32.f * 5.5f, 32.f * 5.5f);

			SAFE_RELEASE(pPlayer);
		}

		GET_SINGLE(SoundManager)->Stop(ST_BGM);
		GET_SINGLE(SoundManager)->Stop(ST_EFFECT);
	}
}

void SceneCave::NextLevel(Collider* pSrc, Collider* pDest, float fTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "PlayerBody")
	{
		RECTANGLE tRC = ((ColliderRect*)pSrc)->GetWorldInfo();

		Tile* pTile = m_pStage->GetTile(tRC.l, tRC.t);

		if (pTile)
		{
			TILE_OPTION eOption = pTile->GetTileOption();

			if (eOption != TO_NOMOVE)
			{
				char strName[32] = {};

				sprintf_s(strName, "Cave_%d", m_iLevel + 1);

				SceneCave* pScene = GET_SINGLE(SceneManager)->CreateScene<SceneCave>(strName, SC_NEXT);

				pScene->SetLevel(m_iLevel + 1);

				/*pScene->CreateMonsters();
				pScene->CreateRocks();*/

				GET_SINGLE(SoundManager)->Stop(ST_BGM);
				GET_SINGLE(SoundManager)->Stop(ST_EFFECT);

				Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

				pPlayer->SetPos(32.f * 5.5f, 32.f * 5.5f);

				SAFE_RELEASE(pPlayer);
			}
		}
	}
}

void SceneCave::CreateRocks()
{
	int iTileSizeX = m_pStage->GetTileSizeX();
	int iTileSizeY = m_pStage->GetTileSizeY();

	if (m_iLevel <= 100)
	{
		while (true)
		{
			float fIndexX = (float)(rand() % 14 + 2);
			float fIndexY = (float)(rand() % 9 + 8);

			Tile* pTile = m_pStage->GetTile((float)iTileSizeX * fIndexX, (float)iTileSizeY * fIndexY);

			POSITION tPos = pTile->GetPos();

			TILE_OPTION eOption = pTile->GetTileOption();

			if (eOption == TO_NONE)
			{
				pTile->SetUpperImageOffset(416.f, 320.f);
				pTile->SetUpperTexture("mine", TEXT("Maps\\TheMines.bmp"));

				Layer* pLayer = FindLayer("Default");

				Portal* pPortal = Obj::CreateObj<Portal>("DownPortal", pLayer);

				pPortal->SetCallback(this, &SceneCave::NextLevel);
				pPortal->SetPos(iTileSizeX * fIndexX, iTileSizeY * fIndexY);

				ColliderRect* pDownPortal = pPortal->AddCollider<ColliderRect>("PortalBody");

				pDownPortal->SetRect(0.f, 0.f, 32.f, 32.f);
				pDownPortal->AddCollisionFunction(CS_ENTER, pPortal, &Portal::ColEnter);

				SAFE_RELEASE(pDownPortal);

				SAFE_RELEASE(pPortal);

				break;
			}
		}
	}

	Layer* pLayer = FindLayer("Default");

	for (int i = 0; i < 20; ++i)
	{
		int iIndex = rand() % (int)(22 * (m_iLevel / 100.f) + 2) + 16;

		float fIndexX = (float)(rand() % 14 + 2);
		float fIndexY = (float)(rand() % 12 + 5);

		Tile* pTile = m_pStage->GetTile((float)iTileSizeX * fIndexX, (float)iTileSizeY * fIndexY);

		POSITION tPos = pTile->GetPos();

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption == TO_NOMOVE)
			continue;

		Obj* pRock = m_pStage->CreateCloneObj(m_vecstrProto[iIndex], "Rock", pLayer);

		if (pRock)
			m_pStage->SetBlock(pTile, pRock->GetBlock(), pRock);

		SAFE_RELEASE(pRock);

		pTile->SetTileOption(TO_NOMOVE);
	}
}

void SceneCave::CreateMonsters()
{
	Layer* pLayer = FindLayer("Default");

	int iRand = rand() % 5;

	int iTileSizeX = m_pStage->GetTileSizeX();
	int iTileSizeY = m_pStage->GetTileSizeY();

	for (int i = 0; i < 4; ++i)
	{
		TILE_OPTION eOption = TO_NONE;
		POSITION tPos = {};
		do
		{
			int iIndex = rand() % 24 + 16;

			float fIndexX = (float)(rand() % 14 + 2);
			float fIndexY = (float)(rand() % 12 + 5);

			Tile* pTile = m_pStage->GetTile((float)iTileSizeX * fIndexX, (float)iTileSizeY * fIndexY);

			tPos = pTile->GetPos();

			eOption = pTile->GetTileOption();
		} while (eOption == TO_NOMOVE);

		Obj* pMonster = nullptr;

		if (iRand == MT_BAT)
		{
			pMonster = m_pStage->CreateCloneObj("Minion", "Minion", pLayer);

			int iLeft = rand() % 2;

			if (iLeft)
				tPos.x = 32.f;

			else
				tPos.x = 480.f;
		}

		else if (iRand == MT_SLIME)
		{
			pMonster = m_pStage->CreateCloneObj("Slime", "slime", pLayer);

			Obj* pHead = m_pStage->CreateCloneObj("SlimeHead", "Head", pLayer);

			((Slime*)pMonster)->SetHead((SlimeHead*)pHead);

			SAFE_RELEASE(pHead);
		}

		else if (iRand == MT_BUG)
		{
			pMonster = m_pStage->CreateCloneObj("Bug", "Bug", pLayer);
		}

		else if (iRand == MT_FLY)
		{
			pMonster = m_pStage->CreateCloneObj("Fly", "Fly", pLayer);
		}

		else if (iRand == MT_ROCKCRAB)
		{
			pMonster = m_pStage->CreateCloneObj("RockCrab", "RockCrab", pLayer);
		}

		pMonster->SetPos(tPos + 16.f);

		PMonsterInfo pInfo = GET_SINGLE(ResourcesManager)->FindMonsterInfo(iRand);

		((FightObj*)pMonster)->SetAttack(pInfo->vecAttack[m_iLevel / 10]);

		((FightObj*)pMonster)->SetHP(pInfo->vecHP[m_iLevel / 10]);

		SAFE_RELEASE(pMonster);
	}
}
