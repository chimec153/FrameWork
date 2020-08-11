#include "PadeEffect.h"
#include "../Core.h"
#include "UI.h"
#include "../Core/Camera.h"

PadeEffect::PadeEffect()	:
	m_bPadeDir(false),
	m_fPadeRate(255.f)
{
}

PadeEffect::PadeEffect(const PadeEffect& padeeffect)	:
	MoveObj(padeeffect)
{
	m_bPadeDir = false;
	m_fPadeRate = 255.f;
}

PadeEffect::~PadeEffect()
{
}

bool PadeEffect::Init()
{
	RESOLUTION tRS = GETRESOLUTION;

	SetSize((float)tRS.iW, (float)tRS.iH);
	SetTexture("pade", TEXT("UI\\Pade.bmp"));
	EnableAlpha(true);

	return true;
}

int PadeEffect::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	m_tPos = tCamPos;

	if (m_bPadeDir && m_fPadeRate < 255.f)
	{
		m_fPadeRate += fDeltaTime * 255.f;
		SetAlpha((unsigned char)m_fPadeRate);
	}

	else if (m_fPadeRate > fDeltaTime * 255.f)
	{
		m_fPadeRate -= fDeltaTime * 255.f;
		SetAlpha((unsigned char)m_fPadeRate);
	}

	return 0;
}

int PadeEffect::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void PadeEffect::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void PadeEffect::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

PadeEffect* PadeEffect::Clone()
{
	return new PadeEffect(*this);
}
