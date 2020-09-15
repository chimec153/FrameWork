#pragma once
#include "Scene.h"
class SceneBarn :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneBarn();
	virtual ~SceneBarn();

private:
	vector<class Texture*>	m_vecTileTex;

public:
	virtual bool Init();
	virtual int Update(float fTime);

public:
	void OutPortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};

