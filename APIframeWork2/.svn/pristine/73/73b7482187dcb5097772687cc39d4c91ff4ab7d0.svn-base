#pragma once
#include "../game.h"
#include "Collider.h"
class CollisionManager
{

public:
	list<class Obj*>	m_CollisionList;
public:
	void AddObject(class Obj* pObj);
	void Collision(float fDeltaTime);
	bool Collision(class Obj* pSrc, class Obj* pDest, float fDeltaTime);
	void Clear();

	DECLARE_SINGLE(CollisionManager)

};
