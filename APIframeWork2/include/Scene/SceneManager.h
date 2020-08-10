#pragma once
#include "../game.h"
class SceneManager
{
private:
	class Scene*	m_pScene;
	class Scene*	m_pNextScene;

public:
	class Scene* GetScene()	const
	{
		return m_pScene;
	}

	class Scene* GetNextScene()	const
	{
		return m_pNextScene;
	}

public:
	bool Init();
	void Input(float fDeltaTime);
	SCENE_CHANGE Update(float fDeltaTime);
	SCENE_CHANGE LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);

private:
	SCENE_CHANGE ChangeScene();

public:
	template <typename T>
	T* CreateScene(SCENE_CREATE sc = SC_CURRENT)
	{
		T*	pScene = new T;

		pScene->SetSceneType(sc);

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return NULL;
		}

		switch (sc)
		{
		case SC_CURRENT:
			SAFE_DELETE(m_pScene);
			m_pScene = pScene;
			break;
		case SC_NEXT:
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		}

		return pScene;
	}
	DECLARE_SINGLE(SceneManager)
};


