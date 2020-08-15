#pragma once
#include "Scene.h"
class SceneHome :
	public Scene
{
private:
	friend class SceneManager;

private:
	SceneHome();
	virtual ~SceneHome();

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

