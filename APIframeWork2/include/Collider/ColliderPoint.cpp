#include "ColliderPoint.h"
#include "../Object/Obj.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "..//Core/Camera.h"

ColliderPoint::ColliderPoint()
{
	m_eCollType = CT_POINT;
}

ColliderPoint::ColliderPoint(const ColliderPoint & coll)	:
	Collider(coll)
{
	m_tDist = coll.m_tDist;
}

ColliderPoint::~ColliderPoint()
{
}

bool ColliderPoint::Init()
{
	return true;
}

void ColliderPoint::Input(float fDeltaTime)
{
	Collider::Input(fDeltaTime);
}

int ColliderPoint::Update(float fDeltaTime)
{
	Collider::Update(fDeltaTime);
	return 0;
}

int ColliderPoint::LateUpdate(float fDeltaTime)
{
	Collider::LateUpdate(fDeltaTime);

	POSITION	tPos = m_pObj->GetPos();

	m_tPos.x = tPos.x + m_tDist.x;
	m_tPos.y = tPos.y + m_tDist.y;

	SetSectionInfo(m_tPos.x - m_tDist.x, m_tPos.y - m_tDist.y,
		m_tPos.x + m_tDist.x, m_tPos.y + m_tDist.y);

	return 0;
}

bool ColliderPoint::Collision(Collider * pDest)
{
	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		if(pDest->IsUI())
			return CollisionRectToPoint(((ColliderRect*)pDest)->GetWorldInfo(), m_tPos);

		else
			return CollisionRectToPoint(((ColliderRect*)pDest)->GetWorldInfo(), m_tPos + tCamPos);
	case CT_SPHERE:   
		return CollisionSphereToPoint(((ColliderSphere*)pDest)->GetWorldInfo(), m_tPos);
	case CT_PIXEL:
		return CollisionPixelToPoint(((ColliderPixel*)pDest)->GetPixel(), ((ColliderPixel*)pDest)->GetWidth(),
			((ColliderPixel*)pDest)->GetHeight(), m_tPos);
	}
	return false;
}

void ColliderPoint::Render(HDC hDC, float fDeltaTime)
{
	Collider::Render(hDC, fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	auto iter = m_SectionList.begin();
	auto iterEnd = m_SectionList.end();

	for (; iter != iterEnd; ++iter)
	{
		TCHAR strSectionIndex[32] = {};

		swprintf_s(strSectionIndex, TEXT("Sec: %d"), (*iter));

		TextOut(hDC, (int)(m_tPos.x - tCamPos.x), (int)(m_tPos.y + 20.f * (*iter) - tCamPos.y), strSectionIndex, lstrlen(strSectionIndex));		
	}
}

ColliderPoint * ColliderPoint::Clone()
{
	return new ColliderPoint(*this);
}

void ColliderPoint::Save(FILE * pFile)
{
}

void ColliderPoint::Load(FILE * pFile)
{
}
