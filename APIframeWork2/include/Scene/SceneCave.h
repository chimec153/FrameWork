#pragma once
#include "Scene.h"
class SceneCave :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneCave();
	virtual ~SceneCave();

private:
	vector<class Texture*>	m_vecTileTex;
	int						m_iLevel;
	class UINum*			m_pLevelNum;

public:
	void SetLevel(int iLevel);

	int GetLevel()	const
	{
		return m_iLevel;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual int Update(float fTime);

public:
	void OutPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
	void NextLevel(class Collider* pSrc, class Collider* pDest, float fTime);

public:
	void CreateRocks();
	void CreateMonsters();
};

