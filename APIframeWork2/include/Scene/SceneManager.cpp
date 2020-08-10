#include "SceneManager.h"
#include "InGameScene.h"
#include "StartScene.h"
#include "../Collider/CollisionManager.h"

DEFINITION_SINGLE(SceneManager)

SceneManager::SceneManager()	:
	m_pScene(NULL),
	m_pNextScene(NULL)
{
}

SceneManager::~SceneManager()
{
	SAFE_DELETE(m_pScene);
}

bool SceneManager::Init()
{
	m_pScene = CreateScene<StartScene>(SC_CURRENT);


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
		SAFE_DELETE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = NULL;

		GET_SINGLE(CollisionManager)->Clear();

		m_pScene->SetSceneType(SC_CURRENT);

		Scene::ChangePrototype();

		return SC_CHANGE;
	}
	return SC_NONE;
}
