#pragma once
#include "Scene.h"
class SceneCoop :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneCoop();
	virtual ~SceneCoop();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();

public:
	void OutPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

