#include "Imp.h"
#include "../Animation/Animation.h"

Imp::Imp()
{
	m_bTileEffect = false;
	m_eMonsterType = MMT_IMP;
}

Imp::Imp(const Imp& imp)	:
	MiniGameMonster(imp)
{
}

Imp::~Imp()
{
}

bool Imp::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	SetSpeed(70.f);
	m_iHP = 1;

	Animation* pAni = CreateAnimation("OrcAni");

	pAni->LoadFromPath("orc.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int Imp::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);

	return 0;
}

int Imp::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void Imp::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Imp::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

Imp* Imp::Clone()
{
	return new Imp(*this);
}

void Imp::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Imp::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Imp::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Imp::CreateItem()
{
}
