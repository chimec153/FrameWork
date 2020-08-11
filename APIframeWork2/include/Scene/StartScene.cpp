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
	GET_SINGLE(SceneManager)->CreateScene<InGameScene>(SC_NEXT);
}

void StartScene::EndButtonCallback(float fTime)
{
	GET_SINGLE(Core)->DestroyGame();
}

void StartScene::EditButtonCallback(float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);
	GET_SINGLE(SceneManager)->CreateScene<MapEditScene>(SC_NEXT);
}

void StartScene::EditAniButtonCallback(float fTime)
{
	GET_SINGLE(SoundManager)->Stop(ST_BGM);
	GET_SINGLE(SceneManager)->CreateScene<CAnimationEditScene>(SC_NEXT);
}
