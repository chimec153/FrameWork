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
	class Stage* m_pStage;

public:
	class Stage* GetStage()	const
	{
		return m_pStage;
	}

public:
	virtual bool Init();

public:
	void OutPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

