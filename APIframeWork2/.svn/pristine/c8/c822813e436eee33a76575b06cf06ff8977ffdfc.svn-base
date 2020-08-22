#pragma once
#include "Scene.h"
class SceneCrapentersShop :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneCrapentersShop();
	virtual ~SceneCrapentersShop();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();

public:
	void TownPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
	void Shop2Col(class Collider* pSrc, class Collider* pDest, float fTime);
};

