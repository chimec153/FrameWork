#include "ColliderSphere.h"
#include "../Object/Obj.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Core/Input.h"
#include "..//Core/Camera.h"

ColliderSphere::ColliderSphere()
{
	m_eCollType = CT_SPHERE;
}

ColliderSphere::ColliderSphere(const ColliderSphere & coll)	:
	Collider(coll)
{
	m_tInfo = coll.m_tInfo;
}

ColliderSphere::~ColliderSphere()
{
}

bool ColliderSphere::Init()
{
	return true;
}

void ColliderSphere::Input(float fDeltaTime)
{
	Collider::Input(fDeltaTime);
}

int ColliderSphere::Update(float fDeltaTime)
{
	Collider::Update(fDeltaTime);
	return 0;
}

int ColliderSphere::LateUpdate(float fDeltaTime)
{
	Collider::LateUpdate(fDeltaTime);

	POSITION	tPos = m_pObj->GetPos();
	m_tWorldInfo.tCenter = tPos + m_tInfo.tCenter;
	m_tWorldInfo.fRadius = m_tInfo.fRadius;

	return 0;
}

bool ColliderSphere::Collision(Collider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_SPHERE:
		return CollisionSphereToSphere(m_tWorldInfo, ((ColliderSphere*)pDest)->GetWorldInfo());
	case CT_RECT:
		return CollisionRectToSphere(((ColliderRect*)pDest)->GetWorldInfo(),m_tWorldInfo);
	case CT_POINT:
		return CollisionSphereToPoint(m_tWorldInfo,
			((ColliderPoint*)pDest)->GetPoint());
	}
	return false;
}

void ColliderSphere::Render(HDC hDC, float fDeltaTime)
{
	Collider::Render(hDC, fDeltaTime);
	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

#ifdef _DEBUG

	if (KEYPRESS("Debug"))
	{
		if (m_bColl)
		{
			HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, (int)(m_tWorldInfo.tCenter.x - m_tWorldInfo.fRadius - tCamPos.x),
				(int)(m_tWorldInfo.tCenter.y - m_tWorldInfo.fRadius - tCamPos.y),
				(int)(m_tWorldInfo.tCenter.x + m_tWorldInfo.fRadius - tCamPos.x),
				(int)(m_tWorldInfo.tCenter.y + m_tWorldInfo.fRadius - tCamPos.y));
			SelectObject(hDC, hPrevBrush);
			DeleteObject(hBrush);
		}
		else
		{
			HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, (int)(m_tWorldInfo.tCenter.x - m_tWorldInfo.fRadius - tCamPos.x),
				(int)(m_tWorldInfo.tCenter.y - m_tWorldInfo.fRadius - tCamPos.y),
				(int)(m_tWorldInfo.tCenter.x + m_tWorldInfo.fRadius - tCamPos.x),
				(int)(m_tWorldInfo.tCenter.y + m_tWorldInfo.fRadius - tCamPos.y));
			SelectObject(hDC, hPrevBrush);
			DeleteObject(hBrush);
		}
	}
#endif

}

ColliderSphere * ColliderSphere::Clone()
{
	return new ColliderSphere(*this);
}

void ColliderSphere::Save(FILE * pFile)
{
}

void ColliderSphere::Load(FILE * pFile)
{
}
