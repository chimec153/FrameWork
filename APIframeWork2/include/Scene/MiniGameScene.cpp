#include "MiniGameScene.h"
#include "../Object/Stage.h"
#include "../Object/ObjManager.h"
#include "../Core/Input.h"
#include "../Scene/SceneManager.h"
#include "../Object/Bullet.h"
#include "../Object/PrairieKing.h"
#include "../Object/PrairieKingLeg.h"
#include "../Object/Orc.h"
#include "../Animation/Animation.h"
#include "../Object/UIInventory.h"
#include "../Object/Player.h"
#include "../Core/Camera.h"
#include "../Object/UIPanel.h"
#include "../Object/UIBar.h"
#include "../Object/Etc.h"
#include "../Object/FarmEffect.h"
#include "../Object/Text.h"
#include "SceneManager.h"
#include "../Collider/ColliderRect.h"

MiniGameScene::MiniGameScene()	:
	m_pKing(nullptr),
	m_fTime(60.f),
	m_fGenTime(0.f),
	m_fGenLimit(1.0f),
	m_fFiverTime(0.f),
	m_fFiverTimeLimit(20.f),
	m_bFiver(false),
	m_fReadyTime(0.f),
	m_fStartTime(5.f),
	m_bGenStart(false),
	m_fFiverEndTime(0.f),
	m_fFiverEndTimeLimit(5.f),
	m_pTimeBar(nullptr),
	m_eItemType(ITEM_NONE),
	m_pItemPanel(nullptr),
	m_pMoneyText(nullptr),
	m_pLifeText(nullptr),
	m_iStage(0),
	m_iMonsterCount(0)
{
}

MiniGameScene::~MiniGameScene()
{
	SAFE_RELEASE(m_pKing);
	SAFE_RELEASE(m_pTimeBar);
	SAFE_RELEASE(m_pItemPanel);
	SAFE_RELEASE(m_pMoneyText);
	SAFE_RELEASE(m_pLifeText);
}

Obj* MiniGameScene::GetKing()
{
	if (m_pKing)
		m_pKing->AddRef();

	return m_pKing;
}

void MiniGameScene::SetKing(PrairieKing* pKing)
{
	if (m_pKing)
		m_pKing->Die();

	SAFE_RELEASE(m_pKing);

	m_pKing = pKing;

	if (m_pKing)
		m_pKing->AddRef();
}

void MiniGameScene::SetStage(int iStage)
{
	m_iStage = iStage;

	char strNum[32] = {};

	if(m_iStage < 4)
		sprintf_s(strNum, "1-%d.tmp",m_iStage  +1);

	else if (m_iStage == 4)
		sprintf_s(strNum, "1-boss.tmp");

	else if (m_iStage < 8)
		sprintf_s(strNum, "2-%d.tmp", m_iStage - 4);

	else if (m_iStage == 8)
		sprintf_s(strNum, "2-boss.tmp");

	else if (m_iStage < 11)
		sprintf_s(strNum, "3-%d.tmp", m_iStage -8);

	else if (m_iStage == 11)
		sprintf_s(strNum, "3-boss.tmp");

	else if (m_iStage == 12)
		sprintf_s(strNum, "4-end.tmp");

	m_pStage->LoadFromPath(strNum);
}

bool MiniGameScene::Init()
{
	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("1-1", pStageLayer);

	m_pStage->LoadFromPath("1-1.tmp");

	int iNumX = m_pStage->GetTileNumX();
	int iNumY = m_pStage->GetTileNumY();

	int iSizeX = m_pStage->GetTileSizeX();
	int iSizeY = m_pStage->GetTileSizeY();

	int iW = tRS.iW;
	int iH = tRS.iH;

	m_pStage->SetStart((float)(iNumX/2 * iSizeX - iW / 2), (float)(iNumY/2 * iSizeY - iH / 2));

	GET_SINGLE(Camera)->SetTarget(nullptr);

	GET_SINGLE(Camera)->SetPos((float)(iNumX / 2 * iSizeX - iW / 2), (float)(iNumY / 2 * iSizeY - iH / 2));

	Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

	pPlayer->SetEnable(false);

	Obj* pHPBar = (Obj*)((Player*)pPlayer)->GetHPBar();

	pHPBar->SetEnable(false);

	Obj* pEBar = (Obj*)((Player*)pPlayer)->GetEnergyBar();

	pEBar->SetEnable(false);

	SAFE_RELEASE(pPlayer);

	Layer* pLayer = FindLayer("Default");

		PrairieKing* pKing = Obj::CreateObj<PrairieKing>("King", pLayer);

		pKing->SetPos(272.f, 272.f);

		SetKing(pKing);

		PrairieKingLeg* pLeg = Obj::CreateObj<PrairieKingLeg>("Leg", pLayer);

		pKing->SetLeg(pLeg);

		SAFE_RELEASE(pLeg);

		SAFE_RELEASE(pKing);

	Bullet* pBullet = CreateProtoType<Bullet>("Bullet");

	SAFE_RELEASE(pBullet);

	Orc* pOrc = CreateProtoType<Orc>("Orc");

	SAFE_RELEASE(pOrc);

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	pInven->SetEnable(false);

	for (int i = 0; i < 36; ++i)
	{
		Obj* pItem = (Obj*)pInven->GetItem(i);

		if(pItem)
			pItem->SetEnable(false);
	}

	SAFE_RELEASE(pInven);

	Obj* pText = m_pStage->FindObject("GoldText");

	pText->SetEnable(false);

	SAFE_RELEASE(pText);

	Layer* pUILayer = FindLayer("UI");

	UIPanel* pTutorialPanel = Obj::CreateObj<UIPanel>("TutorialPanel",pUILayer);

	pTutorialPanel->SetPos(iW/2.f - 80.f, iH/2.f + 288.f);

	pTutorialPanel->SetSize(160.f, 96.f);

	pTutorialPanel->SetTexture("Mouse", TEXT("UI\\Cursors.bmp"));

	pTutorialPanel->SetImageOffset(704.f, 3296.f);

	SAFE_RELEASE(pTutorialPanel);

	m_pTimeBar = Obj::CreateObj<UIBar>("TimeBar", pUILayer, POSITION(iW / 2.f - 288.f, iH / 2.f - 288.f), POSITION(518.f, 32.f));

	m_pTimeBar->SetMax(60.f * 16.5f / 15.5f);

	m_pTimeBar->SetValue(60.f * 16.5f / 15.5f);

	m_pTimeBar->SetTexture("Mouse");

	m_pTimeBar->SetImageOffset(546.f, 3648.f);

	m_pTimeBar->SetHorizon(true);

	UIPanel* pTimeIcon = Obj::CreateObj<UIPanel>("TimeIcon", pUILayer);

	pTimeIcon->SetSize(32.f, 32.f);

	pTimeIcon->SetImageOffset(1184.f, 3488.f);

	pTimeIcon->SetTexture("Mouse");

	pTimeIcon->SetPos(iW / 2.f - 288.f, iH / 2.f - 288.f);

	SAFE_RELEASE(pTimeIcon);

	UIPanel* pItemPanel = Obj::CreateObj<UIPanel>("ItemPanel", pUILayer);

	pItemPanel->SetSize(44.f, 44.f);

	pItemPanel->SetTexture("Mouse");

	pItemPanel->SetImageOffset(588.f, 3564.f);

	pItemPanel->SetPos(iW / 2.f - 300.f, iH / 2.f - 256.f);

	SAFE_RELEASE(pItemPanel);

	Etc* pMoney = CreateProtoType<Etc>("Money");	//	아이템을 복사생성할 프로토타입을 만든다.

	pMoney->SetSpeed(0.f);

	pMoney->SetSize(32.f, 32.f);

	pMoney->SetItemType((ITEM_TYPE)ITEM_MONEY);

	Animation* pMoneyAni = pMoney->CreateAnimation("ItemAni");

	pMoneyAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pMoneyAni);

	pMoney->SetAnimationCurrentClip("Money");

	SAFE_RELEASE(pMoney);

	Etc* pWheel = CreateProtoType<Etc>("Wheel");

	pWheel->SetSpeed(0.f);

	pWheel->SetItemType((ITEM_TYPE)ITEM_WHEEL);

	Animation* pWheelAni = pWheel->CreateAnimation("ItemAni");

	pWheelAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pWheelAni);

	pWheel->SetAnimationCurrentClip("Wheel");

	SAFE_RELEASE(pWheel);

	Etc* pMachineGun = CreateProtoType<Etc>("MachineGun");

	pMachineGun->SetSpeed(0.f);

	pMachineGun->SetItemType((ITEM_TYPE)ITEM_MACHINE);

	Animation* pMachineAni = pMachineGun->CreateAnimation("ItemAni");

	pMachineAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pMachineAni);

	pMachineGun->SetAnimationCurrentClip("Machine");

	SAFE_RELEASE(pMachineGun);

	Etc* pNuke = CreateProtoType<Etc>("Nuke");

	pNuke->SetSpeed(0.f);

	pNuke->SetItemType((ITEM_TYPE)ITEM_NUKE);

	Animation* pNukeAni = pNuke->CreateAnimation("ItemAni");

	pNukeAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pNukeAni);

	pNuke->SetAnimationCurrentClip("Nuke");

	SAFE_RELEASE(pNuke);

	Etc* pTombStone = CreateProtoType<Etc>("TombStone");

	pTombStone->SetSpeed(0.f);

	pTombStone->SetItemType((ITEM_TYPE)ITEM_TOMB);

	Animation* pTombAni = pTombStone->CreateAnimation("ItemAni");

	pTombAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pTombAni);

	pTombStone->SetAnimationCurrentClip("Tomb");

	SAFE_RELEASE(pTombStone);

	Etc* pCoffee = CreateProtoType<Etc>("Coffee");

	pCoffee->SetSpeed(0.f);

	pCoffee->SetItemType((ITEM_TYPE)ITEM_COFFEE);

	Animation* pCoffeeAni = pCoffee->CreateAnimation("ItemAni");

	pCoffeeAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pCoffeeAni);

	pCoffee->SetAnimationCurrentClip("Coffee");

	SAFE_RELEASE(pCoffee);

	Etc* pShotGun = CreateProtoType<Etc>("ShotGun");

	pShotGun->SetSpeed(0.f);

	pShotGun->SetItemType((ITEM_TYPE)ITEM_SHOTGUN);

	Animation* pShotGunAni = pShotGun->CreateAnimation("ItemAni");

	pShotGunAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pShotGunAni);

	pShotGun->SetAnimationCurrentClip("ShotGun");

	SAFE_RELEASE(pShotGun);

	Etc* pLife = CreateProtoType<Etc>("Life");

	pLife->SetSpeed(0.f);

	pLife->SetItemType((ITEM_TYPE)ITEM_LIFE);

	Animation* pLifeAni = pLife->CreateAnimation("ItemAni");

	pLifeAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pLifeAni);

	pLife->SetAnimationCurrentClip("Life");

	SAFE_RELEASE(pLife);

	Etc* pSmokeBomb = CreateProtoType<Etc>("SmokeBomb");

	pSmokeBomb->SetSpeed(0.f);

	pSmokeBomb->SetItemType((ITEM_TYPE)ITEM_SMOKE);

	Animation* pSmokeAni = pSmokeBomb->CreateAnimation("ItemAni");

	pSmokeAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pSmokeAni);

	pSmokeBomb->SetAnimationCurrentClip("Smoke");

	SAFE_RELEASE(pSmokeBomb);

	Etc* pBadge = CreateProtoType<Etc>("Badge");

	pBadge->SetSpeed(0.f);

	pBadge->SetItemType((ITEM_TYPE)ITEM_BADGE);

	Animation* pBadgeAni = pBadge->CreateAnimation("ItemAni");

	pBadgeAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(pBadgeAni);

	pBadge->SetAnimationCurrentClip("Badge");

	SAFE_RELEASE(pBadge);

	Etc* p5Money = CreateProtoType<Etc>("5Money");

	p5Money->SetSize(32.f, 32.f);

	p5Money->SetSpeed(0.f);

	p5Money->SetItemType((ITEM_TYPE)ITEM_5MONEY);

	Animation* p5MoneyAni = p5Money->CreateAnimation("ItemAni");

	p5MoneyAni->LoadFromPath("miniitem.sac");

	SAFE_RELEASE(p5MoneyAni);

	p5Money->SetAnimationCurrentClip("5Money");

	SAFE_RELEASE(p5Money);

	FarmEffect* pEffect = CreateProtoType<FarmEffect>("Effect");

	SAFE_RELEASE(pEffect);

	m_pItemPanel = Obj::CreateObj<UIPanel>("ItemPanel", pUILayer);

	m_pItemPanel->SetSize(32.f, 32.f);

	m_pItemPanel->SetTexture("Mouse");

	m_pItemPanel->SetImageOffset(896.f, 3616.f);

	m_pItemPanel->SetPos(iW / 2.f - 294.f, iH / 2.f - 250.f);

	UIPanel* pLifePanel = Obj::CreateObj<UIPanel>("LifePanel", pUILayer);

	pLifePanel->SetPos(iW / 2.f - 320.f, iH / 2.f - 160.f);

	pLifePanel->SetTexture("Mouse");

	pLifePanel->SetImageOffset(800.f, 3616.f);

	pLifePanel->SetSize(32.f, 32.f);

	SAFE_RELEASE(pLifePanel);

	UIPanel* pMoneyPanel = Obj::CreateObj<UIPanel>("MoneyPanel", pUILayer);

	pMoneyPanel->SetPos(iW / 2.f - 313.f, iH / 2.f - 185.f);

	pMoneyPanel->SetTexture("Mouse");

	pMoneyPanel->SetImageOffset(552.f, 3622.f);

	pMoneyPanel->SetSize(18.f, 18.f);

	SAFE_RELEASE(pMoneyPanel);

	m_pMoneyText = Obj::CreateObj<Text>("MoneyText", pUILayer);

	m_pMoneyText->SetColor(255, 255, 255);

	m_pMoneyText->SetPos(iW / 2.f - 288.f, iH / 2.f - 186.f);

	m_pMoneyText->SetText(TEXT("x 0"));

	m_pLifeText = Obj::CreateObj<Text>("LifeText", pUILayer);

	m_pLifeText->SetPos(iW / 2.f - 288.f, iH / 2.f - 152.f);

	m_pLifeText->SetColor(255, 255, 255);

	m_pLifeText->SetText(TEXT("x 3"));
	
	return true;
}

void MiniGameScene::Start()
{
	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	int iNumX = m_pStage->GetTileNumX();
	int iNumY = m_pStage->GetTileNumY();

	int iSizeX = m_pStage->GetTileSizeX();
	int iSizeY = m_pStage->GetTileSizeY();

	int iW = tRS.iW;
	int iH = tRS.iH;

	GET_SINGLE(Camera)->SetTarget(nullptr);

	GET_SINGLE(Camera)->SetPos((float)(iNumX / 2 * iSizeX - iW / 2), (float)(iNumY / 2 * iSizeY - iH / 2));
}

void MiniGameScene::Input(float fTime)
{
	Scene::Input(fTime);

	if (KEYDOWN("PrevScene"))
	{
		GET_SINGLE(SceneManager)->ReturnToInGameScene();

		UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

		pInven->SetEnable(true);

		for (int i = 0; i < 36; ++i)
		{
			Obj* pItem = (Obj*)pInven->GetItem(i);

			if (pItem)
			pItem->SetEnable(true);
		}

		SAFE_RELEASE(pInven);

		Obj* pClockHand = GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->SetEnable(true);

		Obj* pClockPanel = m_pStage->FindObject("ClockPanel");

		pClockPanel->SetEnable(true);

		SAFE_RELEASE(pClockPanel);

		Obj* pBarPanel = m_pStage->FindObject("BarPanel");

		pBarPanel->SetEnable(true);

		SAFE_RELEASE(pBarPanel);

		Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

		GET_SINGLE(Camera)->SetTarget(pPlayer);

		pPlayer->SetEnable(true);

		Obj* pHPBar = (Obj*)((Player*)pPlayer)->GetHPBar();

		pHPBar->SetEnable(true);

		Obj* pEBar = (Obj*)((Player*)pPlayer)->GetEnergyBar();

		pEBar->SetEnable(true);

		SAFE_RELEASE(pPlayer);

		Obj* pText = m_pStage->FindObject("GoldText");

		pText->SetEnable(true);

		SAFE_RELEASE(pText);
	}
}

int MiniGameScene::Update(float fTime)
{
	Scene::Update(fTime);

	if (m_fReadyTime >= m_fStartTime)
		m_bGenStart = true;

	else
		m_fReadyTime += fTime;

	if (m_bGenStart)
	{
		m_fTime -= fTime;

		m_fGenTime += fTime;

		m_fFiverTime += fTime;
	}

	if (m_fTime > 0 && m_fGenTime >= m_fGenLimit && m_iStage != 4)
	{
		int iDirX = rand() % 2;
		int iDirY = rand() % 2;

		Layer* pLayer = FindLayer("Default");

		Obj* pOrc = m_pStage->Obj::CreateCloneObj("Orc", "Orc", pLayer);	//	오크 몬스터 소환

		++m_iMonsterCount;

		if (iDirX == 0)
		{
			if (iDirY == 0)
				pOrc->SetPos(272.f, 16.f);

			else
				pOrc->SetPos(272.f, 480.f);
		}

		else
		{
			if (iDirY == 0)
				pOrc->SetPos(16.f, 272.f);

			else
				pOrc->SetPos(480.f, 272.f);
		}

		SAFE_RELEASE(pOrc);

		if (m_bFiver)
		{
			int iDirX = rand() % 2;
			int iDirY = rand() % 2;

			Layer* pLayer = FindLayer("Default");

			for (int i = 0; i < 2; ++i)
			{
				Obj* pOrc = m_pStage->Obj::CreateCloneObj("Orc", "Orc", pLayer);	//	오크 몬스터 추가로 2마리 더 소환

				++m_iMonsterCount;

				if (iDirX == 0)
				{
					if (iDirY == 0)
						pOrc->SetPos(240.f + 64.f * i, 16.f);

					else
						pOrc->SetPos(240.f + 64.f * i, 480.f);
				}

				else
				{
					if (iDirY == 0)
						pOrc->SetPos(16.f, 240.f + 64.f * i);

					else
						pOrc->SetPos(480.f, 240.f + 64.f * i);
				}

				SAFE_RELEASE(pOrc);
			}
		}

		m_fGenTime -= m_fGenLimit;
	}

	if (m_fFiverTime >= m_fFiverTimeLimit)
	{
		m_bFiver = true;

		m_fFiverTime -= m_fFiverTimeLimit;
	}

	if (m_bFiver)
		m_fFiverEndTime += fTime;

	if (m_fFiverEndTime >= m_fFiverEndTimeLimit)
	{
		m_fFiverEndTime -= m_fFiverEndTimeLimit;

		m_bFiver = false;
	}
	
	if (m_eItemType == ITEM_WHEEL)	//	현재 가지고 있는 아이템을 보여준다.
		m_pItemPanel->SetImageOffset(608.f, 3616.f);

	else if (m_eItemType == ITEM_MACHINE)
		m_pItemPanel->SetImageOffset(640.f, 3616.f);

	else if (m_eItemType == ITEM_NUKE)
		m_pItemPanel->SetImageOffset(672.f, 3616.f);

	else if (m_eItemType == ITEM_TOMB)
		m_pItemPanel->SetImageOffset(704.f, 3616.f);

	else if (m_eItemType == ITEM_COFFEE)
		m_pItemPanel->SetImageOffset(736.f, 3616.f);

	else if (m_eItemType == ITEM_SHOTGUN)
		m_pItemPanel->SetImageOffset(768.f, 3616.f);

	else if (m_eItemType == ITEM_LIFE)
		m_pItemPanel->SetImageOffset(800.f, 3616.f);

	else if (m_eItemType == ITEM_SMOKE)
		m_pItemPanel->SetImageOffset(832.f, 3616.f);

	else if (m_eItemType == ITEM_BADGE)
		m_pItemPanel->SetImageOffset(864.f, 3616.f);

	else if(m_eItemType == ITEM_NONE)
		m_pItemPanel->SetImageOffset(896.f, 3616.f);

	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	int iW = tRS.iW;
	int iH = tRS.iH;

	if(m_fTime >= 0.f)
		m_pTimeBar->SetValue(m_fTime + 60.f / 15.5f);	//	시간 막대의 값을 설정한다.

	m_pTimeBar->SetPos(iW / 2.f - 288.f, iH / 2.f - 288.f);	//	시간 막대의 위치를 조절한다.

	if (m_fTime < 30.f || m_iStage !=0)
	{
		Obj* pTutorialPanel = m_pStage->FindObject("TutorialPanel");	//	도움말을 사라지게 한다.

		if(pTutorialPanel)
			pTutorialPanel->Die();

		SAFE_RELEASE(pTutorialPanel);
	}

	int iLife = m_pKing->GetLifeCount();

	int iMoney = m_pKing->GetMoney();

	TCHAR strMoney[32] = {};

	swprintf_s(strMoney, TEXT("x %d"), iMoney);

	if(m_pMoneyText)
	m_pMoneyText->SetText(strMoney);

	TCHAR strLife[32] = {};

	swprintf_s(strLife, TEXT("x %d"), iLife);

	if (m_pLifeText)
	m_pLifeText->SetText(strLife);

	Collider* pCol = m_pStage->GetCollider("PortalBody");

	if (m_iMonsterCount == 0 && m_fTime <= 0.f && !pCol)	//	스테이지를 클리어했을 경우다.
	{
		Layer* pLayer = FindLayer("Default");

		ColliderRect* pRC = m_pStage->AddCollider<ColliderRect>("PortalBody");	//	포탈을 생성한다.

		pRC->SetRect(224.f, 480.f, 320.f, 512.f);

		pRC->AddCollisionFunction(CS_ENTER, this, &MiniGameScene::ColPortal);
		pRC->AddCollisionFunction(CS_STAY, this, &MiniGameScene::ColPortal);

		SAFE_RELEASE(pRC);

		Obj* pEffect = m_pStage->CreateCloneObj("Effect", "effect", pLayer);	//	안내 화살표를 생성한다.

		pEffect->SetAnimationCurrentClip("Arrow");

		pEffect->SetPos(8.f * 32.f, 14.f * 32.f);

		SAFE_RELEASE(pEffect);
	}

	SAFE_RELEASE(pCol);

	return 0;
}

void MiniGameScene::ColPortal(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "KingBody")
	{
		if (m_iMonsterCount == 0 && m_fTime <= 0.f)
		{
			char strNum[32] = {};

			sprintf_s(strNum, "MiniGame%d", m_iStage + 1);

			Scene* pScene = GET_SINGLE(SceneManager)->CreateScene<MiniGameScene>(strNum, SC_NEXT);

			((MiniGameScene*)pScene)->SetStage(m_iStage + 1);

			Layer* pLayer = pScene->FindLayer("Default");

			pLayer->AddObject(m_pKing);
			pLayer->AddObject(m_pKing->GetLeg());

			if(m_iStage == 4)
				m_pKing->SetPos(272.f, 112.f);

			else
				m_pKing->SetPos(272.f, 272.f);

			((MiniGameScene*)pScene)->SetKing(m_pKing);
			((MiniGameScene*)pScene)->SetItemType(m_eItemType);
		}
	}
}
