#include "CollisionSection.h"
#include "../Collider/Collider.h"

CollisionSection::CollisionSection()	:
	m_iIndexX(0),
	m_iIndexY(0),
	m_iIndex(0)
{
}

CollisionSection::~CollisionSection()
{
}

void CollisionSection::Init(int iIndexX, int iIndexY, int iIndex, POSITION tSize, POSITION tPos)
{
	m_iIndexX = iIndexX;
	m_iIndexY = iIndexY;
	m_iIndex = iIndex;

	m_tSize = tSize;
	m_tPos = tPos;

	m_vecCollider.reserve(1024);
}

void CollisionSection::AddCollider(Collider* pCol)
{
	pCol->AddCollisionSection(m_iIndex);
	m_vecCollider.push_back(pCol);
}

void CollisionSection::Collision(float fTime)
{
	auto iterCol = m_vecCollider.begin();
	auto iterColEnd = m_vecCollider.end();

	for (; iterCol != iterColEnd; ++iterCol)
	{
		const list<Collider*>* pPrevColList = (*iterCol)->GetPrevColliderList();

		list<Collider*>::const_iterator iter = pPrevColList->begin();
		list<Collider*>::const_iterator iterEnd = pPrevColList->end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->HasSameSection(*iterCol))
			{
				(*iterCol)->CallFunction(CS_LEAVE, (*iter), fTime);
				(*iter)->CallFunction(CS_LEAVE, (*iterCol), fTime);

				(*iter)->EraseCollisionList((*iterCol));

				iter = (*iterCol)->EraseCollisionList((*iter));
				iterEnd = pPrevColList->end();
			}
			else
				++iter;
		}
	}

	if (m_vecCollider.size() < 2)
		return;

	auto iter = m_vecCollider.begin();
	auto iterEnd = m_vecCollider.end();

	auto iter1 = iter;
	auto iter1End = iterEnd;

	--iterEnd;

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		iter1 = iter + 1;

		for (; iter1 != iter1End; ++iter1)
		{
			if (!(*iter1)->GetEnable())
				continue;

			if ((*iter)->Collision((*iter1)))
			{
				if ((*iter)->CheckCollisionList((*iter1)))
				{
					(*iter)->CallFunction(CS_STAY, (*iter1), fTime);
					(*iter1)->CallFunction(CS_STAY, (*iter), fTime);
				}

				else
				{
					(*iter)->CallFunction(CS_ENTER, (*iter1), fTime);
					(*iter1)->CallFunction(CS_ENTER, (*iter), fTime);

					(*iter)->AddCollider((*iter1));
					(*iter1)->AddCollider((*iter));
				}
			}

			else if((*iter)->CheckCollisionList((*iter1)))
			{
				(*iter)->CallFunction(CS_LEAVE, (*iter1), fTime);
				(*iter1)->CallFunction(CS_LEAVE, (*iter), fTime);

				(*iter)->EraseCollisionList((*iter1));
				(*iter1)->EraseCollisionList((*iter));
			}
		}
	}
}

void CollisionSection::ClearCollider()
{
	auto iter = m_vecCollider.begin();
	auto iterEnd = m_vecCollider.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearCollisionSection();
	}

	m_vecCollider.clear();
}
