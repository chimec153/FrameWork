#include "StartScene.h"
#include "../Object/UIPanel.h"
#include "../Object/UIButton.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "../Core.h"
#include "SceneManager.h"
#include "InGameScene.h"
#include "MapEditScene.h"
#include "../Sound/SoundManager.h"
#include "AnimationEditScene.h"
#include "../Object/Player.h"
#include "../Object/UIInventory.h"
#include "../Object/ObjManager.h"
#include "../Object/Tool.h"
#include "../Object/UIBar.h"
#include "../Object/Arm.h"
#include "../Animation/Animation.h"
#include "../Object/Pants.h"
#include "../Object/Shirt.h"
#include "../Object/Hair.h"
#include "../Object/Stage.h"
#include "../Object/Hat.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

bool StartScene::Init() 
{
	GET_SINGLE(SoundManager)->LoadSound("BGM", false, "RoadBgm.mp3");
	GET_SINGLE(SoundManager)->Play("BGM");

	Layer* pLayer = FindLayer("UI");

	UIPanel* pBackPanel = Obj::CreateObj<UIPanel>("BackPanel",
		pLayer);

	pBackPanel->SetSize((float)GETRESOLUTION.iW, (float)GETRESOLUTION.iH);
	pBackPanel->SetTexture("StartBack", L"UI/stardewPanorama.bmp");

	SAFE_RELEASE(pBackPanel);

	UIPanel* pFrontPanel = Obj::CreateObj<UIPanel>("FrontPanel",
		pLayer);

	pFrontPanel->SetPos((float)GETRESOLUTION.iW / 2 - 400,
		GETRESOLUTION.iH/3 - 187.f);

	pFrontPanel->SetSize(800.f, 374.f);
	pFrontPanel->SetTexture("Start", L"UI/TitleButtons1.bmp");
	pFrontPanel->SetColorKey(255, 255, 255);

	SAFE_RELEASE(pFrontPanel);

	UIButton* pStartBtn = Obj::CreateObj<UIButton>("StartButton",
		pLayer);

	pStartBtn->SetPos((float)GETRESOLUTION.iW / 5 - 74.f,
		(float)GETRESOLUTION.iH / 2 + 200.f);
	pStartBtn->SetSize(148, 116);
	pStartBtn->SetTexture("StartButton", L"UI/TitleButtons1.bmp");
	pStartBtn->SetImageOffset(0.f, 374.f);
	pStartBtn->SetColorKey(255, 255, 255);

	ColliderRect* pRC = (ColliderRect*)pStartBtn->GetCollider("ButtonBody");

	POSITION	tPos = pStartBtn->GetPos();
	pRC->SetRect(0.f,0.f,148, 116);

	SAFE_RELEASE(pRC);

	pStartBtn->SetCallback(this, &StartScene::StartButtonCallback);

	SAFE_RELEASE(pStartBtn);

	UIButton* pEditBtn = Obj::CreateObj<UIButton>("EditButton",
		pLayer);

	pEditBtn->SetPos((float)(GETRESOLUTION.iW / 5.f * 2.f - 74.f),
		(float)(GETRESOLUTION.iH / 2 + 200));
	pEditBtn->SetSize(148, 116);
	pEditBtn->SetTexture("EditButton", L"UI/TitleButtons1.bmp");
	pEditBtn->SetImageOffset(148.f, 374.f);
	pEditBtn->SetColorKey(255, 255, 255);

	pRC = (ColliderRect*)pEditBtn->GetCollider("ButtonBody");

	pRC->SetRect(0.f, 0.f, 148, 116);

	SAFE_RELEASE(pRC);

	pEditBtn->SetCallback(this, &StartScene::EditButtonCallback);

	SAFE_RELEASE(pEditBtn);

	pEditBtn = Obj::CreateObj<UIButton>("EditAniButton",
		pLayer);

	pEditBtn->SetPos((float)GETRESOLUTION.iW / 5 * 3.f - 74.f,
		(float)GETRESOLUTION.iH / 2 + 200);
	pEditBtn->SetSize(148, 116);
	pEditBtn->SetTexture("EditAniButton", L"UI/TitleButtons1.bmp");
	pEditBtn->SetImageOffset(296.f, 374.f);
	pEditBtn->SetColorKey(255, 255, 255);

	pRC = (ColliderRect*)pEditBtn->GetCollider("ButtonBody");

	pRC->SetRect(0.f, 0.f, 148, 116);

	SAFE_RELEASE(pRC);

	pEditBtn->SetCallback(this, &StartScene::EditAniButtonCallback);

	SAFE_RELEASE(pEditBtn);

	UIButton* pEndBtn = Obj::CreateObj<UIButton>("EndButton",
		pLayer);

	pEndBtn->SetPos((float)GETRESOLUTION.iW / 5*4.f-74.f,
		(float)GETRESOLUTION.iH / 2 + 200);
	pEndBtn->SetSize(148, 116);
	pEndBtn->SetTexture("EndButton", L"UI/TitleButtons1.bmp");
	pEndBtn->SetImageOffset(444.f, 374.f);
	pEndBtn->SetColorKey(255, 255, 255);

	pRC = (ColliderRect*)pEndBtn->GetCollider("ButtonBody");

	pRC->SetRect(0.f, 0.f, 148, 116);

	SAFE_RELEASE(pRC);

	pEndBtn->SetCallback(this, &StartScene::EndButtonCallback);

	SAFE_RELEASE(pEndBtn);

	return true;
}

void StartScene::StartButtonCallback(float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);

	Player* pPlayer = (Player*)CreatePlayer();

	pPlayer->SetPos(32.f * 8.f, 32.f * 14.f);

	Layer* pHUDLayer = FindLayer("HUD");

	Layer* pLayer = FindLayer("Default");

	Arm* pArm = Obj::CreateObj<Arm>("Arm", pLayer);

	pPlayer->SetArm(pArm);

	SAFE_RELEASE(pArm);

	Pants* pPants = Obj::CreateObj<Pants>("Pants", pLayer);

	pPlayer->SetPants(pPants);

	SAFE_RELEASE(pPants);

	Hair* pHair = Obj::CreateObj<Hair>("Hair", pLayer);

	pPlayer->SetHair(pHair);

	SAFE_RELEASE(pHair);

	Shirt* pShirt = Obj::CreateObj<Shirt>("Shirt", pLayer);

	pPlayer->SetShirt(pShirt);

	SAFE_RELEASE(pShirt);

	Hat* pHat = Obj::CreateObj<Hat>("Hat", pLayer);

	pPlayer->SetHat(pHat);

	SAFE_RELEASE(pHat);

	UIBar* pEnergyBar = Obj::CreateObj<UIBar>("EnergyBar", pHUDLayer,
		POSITION(1006.f, 626.f), POSITION(12.f, 82.f));

	pEnergyBar->SetTexture("Mouse");
	pEnergyBar->SetImageOffset(160.f, 32.f);
	pEnergyBar->SetColorKey(255, 255, 255);
	pEnergyBar->SetMax(100.f);
	pEnergyBar->SetValue(100.f);

	UIBar* pHPBar = Obj::CreateObj<UIBar>("HPBar", pHUDLayer,
		POSITION(1030.f, 626.f), POSITION(12.f, 82.f));

	pHPBar->SetTexture("Mouse");
	pHPBar->SetImageOffset(172.f, 32.f);
	pHPBar->SetColorKey(255, 255, 255);
	pHPBar->SetMax(100.f);
	pHPBar->SetValue(100.f);

	pPlayer->SetHPBar(pHPBar);
	pPlayer->SetEnergyBar(pEnergyBar);

	SAFE_RELEASE(pHPBar);

	SAFE_RELEASE(pEnergyBar);

	UIInventory* pInven = Obj::CreateObj<UIInventory>("Inventory", pHUDLayer,
		POSITION(400.f, 700.f), POSITION(12.f, 82.f));

	GET_SINGLE(ObjManager)->SetInven(pInven);

	pInven->CreateInfoPanel(8, 8);

	CreateClockHand();

	SAFE_RELEASE(pPlayer);

	GET_SINGLE(SceneManager)->CreateScene<InGameScene>("Main", SC_NEXT);

	Item* pSword = (Item*)pInven->Obj::CreateCloneObj("Rusty Sword", "sword", pLayer);

	pInven->AddItem(pSword);

	Item* pHoe = (Item*)pInven->Obj::CreateCloneObj("Starter Hoe", "hoe", pLayer);

	pInven->AddItem(pHoe);

	Item* pPikEx = (Item*)pInven->Obj::CreateCloneObj("Starter Pikex", "Pikex", pLayer);

	pInven->AddItem(pPikEx);

	Item* pEx = (Item*)pInven->Obj::CreateCloneObj("Starter Ex", "Ex", pLayer);

	pInven->AddItem(pEx);

	Item* pWateringCan = (Item*)pInven->Obj::CreateCloneObj("Starter Watering Can", "Watering Can", pLayer);

	pInven->AddItem(pWateringCan);

	Item* pScythe = (Item*)pInven->Obj::CreateCloneObj("Starter Scythe", "Scythe", pLayer);

	pInven->AddItem(pScythe);

	Item* pScissor = (Item*)pInven->Obj::CreateCloneObj("Scissor", "Scissor", pLayer);

	pInven->AddItem(pScissor);

	SAFE_RELEASE(pScissor);
	SAFE_RELEASE(pScythe);
	SAFE_RELEASE(pWateringCan);
	SAFE_RELEASE(pSword);
	SAFE_RELEASE(pHoe);
	SAFE_RELEASE(pPikEx);
	SAFE_RELEASE(pEx);
	SAFE_RELEASE(pInven);
}

void StartScene::EndButtonCallback(float fTime)
{
	GET_SINGLE(Core)->DestroyGame();
}

void StartScene::EditButtonCallback(float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);
	GET_SINGLE(SceneManager)->CreateScene<MapEditScene>("Map", SC_NEXT);
}

void StartScene::EditAniButtonCallback(float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);
	GET_SINGLE(SceneManager)->CreateScene<CAnimationEditScene>("Ani", SC_NEXT);
}
