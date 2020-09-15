#include "Pixie.h"

Pixie::Pixie()
{
	m_bTileEffect = false;
	m_eMonsterType = MMT_PIXIE;
}

Pixie::Pixie(const Pixie& pixie)	:
	MiniGameMonster(pixie)
{
}

Pixie::~Pixie()
{
}

bool Pixie::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	return true;
}

int Pixie::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);
	return 0;
}

int Pixie::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void Pixie::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Pixie::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

Pixie* Pixie::Clone()
{
	return new Pixie(*this);
}

void Pixie::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Pixie::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Pixie::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Pixie::CreateItem()
{
}
