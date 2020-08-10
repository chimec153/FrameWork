#include "CollisionManager.h"
#include "../Object/Obj.h"
#include "Collider.h"

DEFINITION_SINGLE(CollisionManager)

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::AddObject(Obj * pObj)
{
	if (pObj->CheckCollider())
	{
		m_CollisionList.push_back(pObj);
	}
}

void CollisionManager::Collision(float fDeltaTime)
{
	if (m_CollisionList.size() < 2)
	{
		m_CollisionList.clear();
		return;
	}
	list<Obj*>::iterator	iter;
	list<Obj*>::iterator	iterEnd = m_CollisionList.end();
	--iterEnd;

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		list<Obj*>::iterator	iter1 = iter;
		++iter1;
		list<Obj*>::iterator	iter1End = m_CollisionList.end();
		for (; iter1 != iter1End; ++iter1)
		{
			Collision(*iter, *iter1, fDeltaTime);
		}
	}

	m_CollisionList.clear();

}

bool CollisionManager::Collision(Obj * pSrc, Obj * pDest, float fDeltaTime)
{
	const list<Collider*>* pSrcList = pSrc->GetColliderList();
	const list<Collider*>* pDestList = pDest->GetColliderList();

	list<Collider*>::const_iterator	iterSrc;
	list<Collider*>::const_iterator	iterSrcEnd = pSrcList->end();

	list<Collider*>::const_iterator	iterDest;
	list<Collider*>::const_iterator	iterDestEnd = pDestList->end();

	bool	bCollision = false;

	for (iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; ++iterSrc)
	{
		for (iterDest = pDestList->begin(); iterDest != iterDestEnd; ++iterDest)
		{
			if ((*iterSrc)->Collision(*iterDest))
			{ 
				bCollision = true;

				(*iterDest)->SetHitPoint((*iterSrc)->GetHitPoint());

				if (!(*iterSrc)->CheckCollisionList(*iterDest))
				{
					(*iterSrc)->AddCollider(*iterDest);
					(*iterDest)->AddCollider(*iterSrc);

					(*iterSrc)->CallFunction(CS_ENTER, *iterDest,
						fDeltaTime); 
					(*iterDest)->CallFunction(CS_ENTER, *iterSrc,
						fDeltaTime);
				}
				else
				{
					(*iterSrc)->CallFunction(CS_STAY, *iterDest,
						fDeltaTime);
					(*iterDest)->CallFunction(CS_STAY, *iterSrc,
						fDeltaTime);
				}
			}
			else if((*iterSrc)->CheckCollisionList(*iterDest))
			{
				(*iterSrc)->EraseCollisionList(*iterDest);
				(*iterDest)->EraseCollisionList(*iterSrc);

				(*iterSrc)->CallFunction(CS_LEAVE, *iterDest,
					fDeltaTime);
				(*iterDest)->CallFunction(CS_LEAVE, *iterSrc,
					fDeltaTime);
			}
		}
	}
	return bCollision;
}

void CollisionManager::Clear()
{
	m_CollisionList.clear();
}
