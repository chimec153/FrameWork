#include "ColliderRect.h"
#include "../Object/Obj.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Core/Camera.h"
#include "..//Core/Input.h"

ColliderRect::ColliderRect()
{
	m_eCollType = CT_RECT;
}

ColliderRect::ColliderRect(const ColliderRect & coll)	:
	Collider(coll)
{
	m_tInfo = coll.m_tInfo;
}

ColliderRect::~ColliderRect()
{
}

void ColliderRect::SetRect(float l, float t, float r, float b)
{
	m_tInfo.l = l;
	m_tInfo.t = t;
	m_tInfo.r = r;
	m_tInfo.b = b;
}

bool ColliderRect::Init()
{
	return true;
}

void ColliderRect::Input(float fDeltaTime)
{
	Collider::Input(fDeltaTime);
}

int ColliderRect::Update(float fDeltaTime)
{
	Collider::Update(fDeltaTime);
	return 0;
}

int ColliderRect::LateUpdate(float fDeltaTime)
{
	Collider::LateUpdate(fDeltaTime);

	POSITION	tPos = m_pObj->GetPos();
	m_tWorldInfo.l = tPos.x + m_tInfo.l;
	m_tWorldInfo.t = tPos.y + m_tInfo.t;
	m_tWorldInfo.r = tPos.x + m_tInfo.r;
	m_tWorldInfo.b = tPos.y + m_tInfo.b;

	return 0;
}

bool ColliderRect::Collision(Collider* pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tWorldInfo, ((ColliderRect*)pDest)->GetWorldInfo());
	case CT_SPHERE:
		return CollisionRectToSphere(m_tWorldInfo, ((ColliderSphere*)pDest)->GetWorldInfo());
	case CT_PIXEL:
		return CollisionRectToPixel(m_tWorldInfo,
			((ColliderPixel*)pDest)->GetPixel(), ((ColliderPixel*)pDest)->GetWidth(),
			((ColliderPixel*)pDest)->GetHeight());
	case CT_POINT:
		return CollisionRectToPoint(m_tWorldInfo,
			((ColliderPoint*)pDest)->GetPoint());
	}
	return false;
}

void ColliderRect::Render(HDC hDC, float fDeltaTime)
{
#ifdef _DEBUG
	if (KEYPRESS("Debug"))
	{
	Collider::Render(hDC, fDeltaTime);
	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();

	RECTANGLE	tRC = m_tWorldInfo;

	if (!m_bUI)
	{
		tRC.l -= tCamPos.x;
		tRC.r -= tCamPos.x;
		tRC.t -= tCamPos.y;
		tRC.b -= tCamPos.y;
	}	

	MoveToEx(hDC, (int)tRC.l, (int)tRC.t, NULL);
	LineTo(hDC, (int)tRC.r, (int)tRC.t);
	LineTo(hDC, (int)tRC.r, (int)tRC.b);
	LineTo(hDC, (int)tRC.l, (int)tRC.b);
	LineTo(hDC, (int)tRC.l, (int)tRC.t);

		RECT tRect = { (LONG)tRC.l, (LONG)tRC.t, (LONG)tRC.r, (LONG)tRC.b };
		if (m_bColl)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hBrush);
			FrameRect(hDC, &tRect, hBrush);
			SelectObject(hDC, hPrevBrush);
			DeleteObject(hBrush);
		}
		else
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HBRUSH hPrevBrush = (HBRUSH)SelectObject(hDC, hBrush);
			FrameRect(hDC, &tRect, hBrush);
			SelectObject(hDC, hPrevBrush);
			DeleteObject(hBrush);
		}
	}
#endif
}

ColliderRect * ColliderRect::Clone()
{
	return new ColliderRect(*this);
}

void ColliderRect::Save(FILE * pFile)
{
	Collider::Save(pFile);

	fwrite(&m_tInfo, sizeof(m_tInfo), 1, pFile);
	fwrite(&m_tWorldInfo, sizeof(m_tWorldInfo), 1, pFile);
}

void ColliderRect::Load(FILE * pFile)
{
	Collider::Load(pFile);

	fread(&m_tInfo, sizeof(m_tInfo), 1, pFile);
	fread(&m_tWorldInfo, sizeof(m_tWorldInfo), 1, pFile);
}
