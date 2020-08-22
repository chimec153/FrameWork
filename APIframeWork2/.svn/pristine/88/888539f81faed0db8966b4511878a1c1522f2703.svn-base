#pragma once
#include "Scene.h"
class SceneWeaponShop :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneWeaponShop();
	virtual ~SceneWeaponShop();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();

public:
	void TownPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

