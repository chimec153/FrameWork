#include "Orge.h"

Orge::Orge()
{
	m_bTileEffect = true;
	m_eMonsterType = MMT_ORGE;
}

Orge::Orge(const Orge& orge)	:
	MiniGameMonster(orge)
{
}

Orge::~Orge()
{
}

bool Orge::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	return true;
}

int Orge::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);
	return 0;
}

int Orge::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void Orge::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Orge::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

Orge* Orge::Clone()
{
	return new Orge(*this);
}

void Orge::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orge::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orge::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orge::CreateItem()
{
}
