#pragma once
#include "Scene.h"
class InGameScene	:
	public Scene
{
private:
	friend class SceneManager;

private:
	InGameScene();
	~InGameScene();

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
	void Cave(class Collider* pSrc, class Collider* pDest, float fTime);
	void HomePortalCol(class Collider* pSrc, class Collider* pDest, float fTime);
};
