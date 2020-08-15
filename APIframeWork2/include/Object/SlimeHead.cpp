#include "SlimeHead.h"
#include "../Animation/Animation.h"

SlimeHead::SlimeHead()
{
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

SlimeHead::SlimeHead(const SlimeHead& head)	:
	MoveObj(head)
{
}

SlimeHead::~SlimeHead()
{
}

bool SlimeHead::Init()
{
	SetSize(32.f, 48.f);
	SetPivot(0.5f, 0.5f);

	Animation* pHeadAni = CreateAnimation("SlimeHeadAni");

	AddAnimationClip("HeadIdleLeft", AT_ATLAS, AO_LOOP,
		1.f, 4, 11, 0, 8, 4, 1, 1.f, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	SetAnimationClipColorKey("HeadIdleLeft", 255, 255, 255);

	AddAnimationClip("HeadIdleRight", AT_ATLAS, AO_LOOP,
		1.f, 4, 11, 0, 7, 4, 1, 1.f, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	SetAnimationClipColorKey("HeadIdleRight", 255, 255, 255);

	SAFE_RELEASE(pHeadAni);

	return true;
}

int SlimeHead::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	POSITION tAngle = GetAngle();

	if (tAngle.x > 0.f)
	{
		m_pAnimation->SetCurrentClip("HeadIdleRight");
	}

	else if (tAngle.x < 0.f)
	{
		m_pAnimation->SetCurrentClip("HeadIdleLeft");
	}

	else if (tAngle.y > 0.f)
	{
		m_pAnimation->SetCurrentClip("HeadIdleRight");
	}

	else if (tAngle.y < 0.f)
	{
		m_pAnimation->SetCurrentClip("HeadIdleLeft");
	}

	return 0;
}

int SlimeHead::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void SlimeHead::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void SlimeHead::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

SlimeHead* SlimeHead::Clone()
{
	return new SlimeHead(*this);
}
