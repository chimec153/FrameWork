#pragma once

#include "../game.h"

class CollisionSection
{
	friend class SceneCollision;

private:
	CollisionSection();
	~CollisionSection();

private:
	int								m_iIndexX;
	int								m_iIndexY;
	int								m_iIndex;
	POSITION						m_tSize;
	POSITION						m_tPos;
	vector<class Collider*>			m_vecCollider;

public:
	void Init(int iIndexX, int iIndexY, int iIndex, POSITION tSize, POSITION tPos);
	void AddCollider(class Collider* pCol);
	void Collision(float fTime);
	void ClearCollider();
};

