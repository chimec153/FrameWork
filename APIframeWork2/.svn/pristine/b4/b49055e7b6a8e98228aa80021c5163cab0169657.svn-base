#pragma once

#include "../game.h"

class SceneCollision
{
	friend class Scene;

private:
	SceneCollision();
	~SceneCollision();

private:
	vector<class CollisionSection*>		m_vecSection;
	int									m_iCountX;
	int									m_iCountY;
	POSITION							m_tSectionSize;
	POSITION							m_tWorldSize;
	class Scene*						m_pScene;

public:
	void CreateCollisionSection(int iCountX, int iCountY, POSITION tSize);
	void AddCollider(class Collider* pCol);
	void Collision(float fTime);
	void ClearSection();
};

