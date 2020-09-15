#include "SceneCollision.h"
#include "CollisionSection.h"
#include "../Collider/Collider.h"
#include "../Core/Camera.h"

SceneCollision::SceneCollision()	:
	m_iCountX(0),
	m_iCountY(0),
	m_pScene(nullptr)
{
}

SceneCollision::~SceneCollision()
{
	SAFE_DELETE_VECLIST(m_vecSection);
}

void SceneCollision::CreateCollisionSection(int iCountX, int iCountY, POSITION tSize)
{
	m_iCountX = iCountX;
	m_iCountY = iCountY;

	m_tSectionSize = tSize;
	m_tWorldSize = POSITION((float)m_iCountX, (float)m_iCountY) * tSize;

	m_vecSection.resize(iCountX * iCountY);

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			CollisionSection* pSection = new CollisionSection;

			pSection->Init(j, i, i * m_iCountX + j, tSize, POSITION((float)j, (float)i) * tSize);

			m_vecSection[i * m_iCountX + j] = pSection;
		}
	}
}

void SceneCollision::AddCollider(Collider* pCol)
{
	RECTANGLE tRect = {};

	if (pCol->IsUI())
	{
		tRect = pCol->GetSectionInfo();

		POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

		tRect.l += tCamPos.x;
		tRect.t += tCamPos.y;
		tRect.r += tCamPos.x;
		tRect.b += tCamPos.y;
	}		

	else
		tRect = pCol->GetSectionInfo();

	int iStartX = (int)(tRect.l / m_tSectionSize.x);
	int iEndX = (int)(tRect.r / m_tSectionSize.x);

	int iStartY = (int)(tRect.t / m_tSectionSize.y);
	int iEndY = (int)(tRect.b / m_tSectionSize.y);

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;

	iEndX = iEndX >= m_iCountX ? m_iCountX - 1 : iEndX;
	iEndY = iEndY >= m_iCountY ? m_iCountY - 1 : iEndY;

	for (int i = iStartY; i <= iEndY; ++i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			m_vecSection[i * m_iCountX + j]->AddCollider(pCol);
		}
	}
}

void SceneCollision::Collision(float fTime)
{
	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
			m_vecSection[i * m_iCountX + j]->Collision(fTime);
	}

	ClearSection();
}

void SceneCollision::ClearSection()
{
	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
			m_vecSection[i * m_iCountX + j]->ClearCollider();
	}
}
