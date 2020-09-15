#include "Camera.h"
#include "../Object/Obj.h"

DEFINITION_SINGLE(Camera)
Camera::Camera()	:
	m_pTarget(NULL)
{
}

Camera::~Camera()
{
	SAFE_RELEASE(m_pTarget);
}

void Camera::SetTarget(Obj * pObj)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pObj;

	if (m_pTarget)
		m_pTarget->AddRef();
}

bool Camera::Init(const POSITION & tPos, 
	const RESOLUTION & tRS, 
	const RESOLUTION & tWorldRS)
{
	m_tPos = tPos;
	m_tClientRS = tRS;
	m_tWorldRS = tWorldRS;
	m_tPivot = POSITION(0.5f, 0.5f);

	return true;
}

void Camera::Input(float fDeltaTime)
{
	if (m_pTarget)
	{

	}
}

void Camera::Update(float fDeltaTime)
{
	if (m_pTarget)
	{
		POSITION tPos = m_pTarget->GetPos();
		POSITION tPivot = m_pTarget->GetPivot();
		_SIZE	tSize = m_pTarget->GetSize();

		float fL = tPos.x - tPivot.x * tSize.x;
		float fT = tPos.y - tPivot.y * tSize.y;
		float fR = fL + tSize.x;
		float fB = fT + tSize.y;

		float	fLeftArea = m_tClientRS.iW * m_tPivot.x;
		float	fRightArea = m_tClientRS.iW - fLeftArea;
		float	fTopArea = m_tClientRS.iH * m_tPivot.y;
		float	fBottomArea = m_tClientRS.iH - fTopArea;

		if (tPos.x <= fLeftArea)
			m_tPos.x = 0.f;

		else if (tPos.x >= m_tWorldRS.iW - fRightArea)
		{
			if (m_tWorldRS.iW >= m_tClientRS.iW)
				m_tPos.x = (float)(m_tWorldRS.iW - m_tClientRS.iW);

			else
				m_tPos.x = 0.f;
		}

		else
			m_tPos.x = tPos.x - m_tClientRS.iW * m_tPivot.x;


		if (tPos.y <= fTopArea)
			m_tPos.y = 0.f;

		else if (tPos.y >= m_tWorldRS.iH - fBottomArea)
		{
			if (m_tWorldRS.iH >= m_tClientRS.iH)
				m_tPos.y = (float)(m_tWorldRS.iH - m_tClientRS.iH);

			else
				m_tPos.y = 0.f;
		}

		else
			m_tPos.y = tPos.y - m_tClientRS.iH * m_tPivot.y;
	}
}

void Camera::Scroll(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;

	if (m_tPos.x < 0)
		m_tPos.x = 0;

	else if (m_tPos.x > m_tWorldRS.iW - m_tClientRS.iW)
		m_tPos.x = (float)(m_tWorldRS.iW - m_tClientRS.iW);

	if (m_tPos.y < 0)
		m_tPos.y = 0;

	else if (m_tPos.y > m_tWorldRS.iH - m_tClientRS.iH)
		m_tPos.y = (float)(m_tWorldRS.iH - m_tClientRS.iH);
}
