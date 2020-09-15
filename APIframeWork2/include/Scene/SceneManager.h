#pragma once
#include "../game.h"
class SceneManager
{
private:
	class Scene* m_pScene;
	class Scene* m_pNextScene;
	class Scene* m_pInGameScene;
	unordered_map<string, class Scene*>		m_mapScene;

public:
	class Scene* GetScene()	const
	{
		return m_pScene;
	}

	class Scene* GetNextScene()	const
	{
		return m_pNextScene;
	}

	class Scene* GetInGameScene()	const
	{
		return m_pInGameScene;
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
	T* CreateScene(const string& strName, SCENE_CREATE sc = SC_CURRENT)
	{
		T* pScene = (T*)FindScene(strName);

		if (pScene)
		{
			switch (sc)
			{
			case SC_CURRENT:
				m_pScene = pScene;
				break;
			case SC_NEXT:
				m_pNextScene = pScene;
				break;
			case SC_MINIGAME:
				m_pNextScene = pScene;

				m_pInGameScene = m_pScene;
				break;
			}

			return pScene;
		}

		pScene = new T;

		pScene->SetSceneType(sc);

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return NULL;
		}

		switch (sc)
		{
		case SC_CURRENT:
			m_pScene = pScene;
			break;
		case SC_NEXT:
			m_pNextScene = pScene;
			break;
		case SC_MINIGAME:
			m_pNextScene = pScene;

			m_pInGameScene = m_pScene;
			break;
		}

		m_mapScene.insert(make_pair(strName, pScene));

		return pScene;
	}

	void ReturnToInGameScene()
	{
		if (!m_pNextScene)
		{
			m_pNextScene = m_pInGameScene;

			m_pInGameScene = nullptr;
		}
	}

private:
	class Scene* FindScene(const string& strName)
	{
		auto iter = m_mapScene.find(strName);

		if (iter == m_mapScene.end())
			return nullptr;

		return iter->second;
	}

	DECLARE_SINGLE(SceneManager)
};


