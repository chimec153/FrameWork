#include "Mummy.h"

Mummy::Mummy()
{
	m_bTileEffect = true;
	m_eMonsterType = MMT_MUMMY;
}

Mummy::Mummy(const Mummy& mummy)	:
	MiniGameMonster(mummy)
{
}

Mummy::~Mummy()
{
}

bool Mummy::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	return true;
}

int Mummy::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);
	return 0;
}

int Mummy::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void Mummy::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Mummy::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

Mummy* Mummy::Clone()
{
	return new Mummy(*this);
}

void Mummy::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Mummy::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Mummy::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Mummy::CreateItem()
{
}
