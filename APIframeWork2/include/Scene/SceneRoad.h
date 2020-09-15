#pragma once
#include "Scene.h"
class SceneRoad :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneRoad();
	virtual ~SceneRoad();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void TownPortal(class Collider* pSrc, class Collider* pDest, float fTime);
	void FarmPortal(class Collider* pSrc, class Collider* pDest, float fTime);
};

