#include "SceneManager.h"
#include "InGameScene.h"
#include "StartScene.h"
#include "SceneShop.h"
#include "../Collider/CollisionManager.h"
#include "..//Object/UIPanel.h"
#include "../Object/ObjManager.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Core/Camera.h"
#include "../Object/UIInventory.h"
#include "../Object/UIClockHand.h"
#include "../Object/Weapon.h"

DEFINITION_SINGLE(SceneManager)

SceneManager::SceneManager()	:
	m_pScene(NULL),
	m_pNextScene(NULL),
	m_pInGameScene(NULL)
{
}

SceneManager::~SceneManager()
{
	SAFE_DELETE_MAP(m_mapScene);
}

bool SceneManager::Init()
{
	m_pScene = CreateScene<StartScene>("Start", SC_CURRENT);

	return true;
}

void SceneManager::Input(float fDeltaTime)
{
	m_pScene->Input(fDeltaTime);
}

SCENE_CHANGE SceneManager::Update(float fDeltaTime)
{
	m_pScene->Update(fDeltaTime);
	return ChangeScene();
}

SCENE_CHANGE SceneManager::LateUpdate(float fDeltaTime)
{
	m_pScene->LateUpdate(fDeltaTime);
	return ChangeScene();
}

void SceneManager::Collision(float fDeltaTime)
{
	m_pScene->Collision(fDeltaTime);
}

void SceneManager::Render(HDC hDC, float fDeltaTime)
{
	m_pScene->Render(hDC, fDeltaTime);
}

SCENE_CHANGE SceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		m_pScene = m_pNextScene;
		m_pNextScene = NULL;

		GET_SINGLE(CollisionManager)->Clear();

		m_pScene->SetSceneType(SC_CURRENT);

		Layer* pLayer = m_pScene->FindLayer("Default");
		Layer* pHUDLayer = m_pScene->FindLayer("HUD");
		Layer* pUILayer = m_pScene->FindLayer("UI");
		Layer* pUTHUDLayer = m_pScene->FindLayer("UTHUD");

		Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();
		UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();
		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();
		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pClockHand)
		{
			pClockHand->SetScene(m_pScene);
			pClockHand->SetLayer(pUILayer);
		}

		if (pInven)
		{
			pInven->AddItemToLayer(pHUDLayer);
			pInven->AddInfoPanelToLayer(pUILayer);
			pInven->SetObjectLayer(pUTHUDLayer);
			pInven->SetInfoPanelLayer(pUILayer);
			pInven->SetScene(m_pScene);
			pInven->SetLayer(pLayer);
		}

		if (pPlayer)
		{
			pPlayer->SetScene(m_pScene);
			pPlayer->SetLayer(pLayer);
			pPlayer->SetBarLayer(pHUDLayer);
		}

		if (pWeapon)
		{
			pWeapon->SetScene(m_pScene);
			pWeapon->SetLayer(pLayer);
		}

		Stage* pStage = m_pScene->GetStage();

		if (pStage)
		{
			int iNumX = pStage->GetTileNumX();
			int iNumY = pStage->GetTileNumY();

			int iSizeX = pStage->GetTileSizeX();
			int iSizeY = pStage->GetTileSizeY();

			GET_SINGLE(Camera)->SetWorldResolution(iNumX * iSizeX, iNumY * iSizeY);
		}

		SAFE_RELEASE(pWeapon);
		SAFE_RELEASE(pPlayer);
		SAFE_RELEASE(pInven);

		if(!m_pScene->IsStarted())
			m_pScene->Start();

		return SC_CHANGE;
	}

	return SC_NONE;
}
