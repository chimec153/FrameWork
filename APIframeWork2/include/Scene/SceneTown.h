#pragma once
#include "Scene.h"
class SceneTown :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneTown();
	virtual ~SceneTown();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void RoadPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
	void ShopPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
	void WeaponShopPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
	void CarpentersShopPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

