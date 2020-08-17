#pragma once
#include "Scene.h"
class SceneShop :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneShop();
	virtual ~SceneShop();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();

public:
	void TownPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

