#include "Effect.h"

Effect::Effect()	:
	m_fLimitDist(30.f),
	m_fDist(0.f),
	m_fTime(0.f),
	m_fLimitTime(4.f)
{
}

Effect::Effect(const Effect& effect)	:
	MoveObj(effect)
{
	m_fLimitDist = effect.m_fLimitDist;
	m_fDist = 0.f;
	m_fTime = 0.f;
	m_fLimitTime = effect.m_fLimitTime;
}

Effect::~Effect()
{
}

bool Effect::Init()
{
	return true;
}

int Effect::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	m_fDist += m_fSpeed * fDeltaTime;
	m_fTime += fDeltaTime;

	if (m_fDist < m_fLimitDist)
	{
		MoveAngle(fDeltaTime);
	}
	else
		m_blsPhysics = false;

	if (m_fTime >= m_fLimitTime)
		Die();

	return 0;
}

int Effect::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void Effect::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Effect::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

Effect* Effect::Clone()
{
	return new Effect(*this);
}
