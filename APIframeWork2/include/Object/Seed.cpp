#include "Seed.h"

Seed::Seed()
{
	m_eType = IT_SEED;
}

Seed::Seed(const Seed& seed)	:
	Item(seed)
{

}

Seed::~Seed()
{
}

bool Seed::Init()
{
	if (!Item::Init())
		return false;

	return true;
}

int Seed::Update(float fDeltaTime)
{
	Item::Update(fDeltaTime);
	return 0;
}

int Seed::LateUpdate(float fDeltaTime)
{
	Item::LateUpdate(fDeltaTime);
	return 0;
}

void Seed::Collision(float fDeltaTime)
{
	Item::Collision(fDeltaTime);
}

void Seed::Render(HDC hDC, float fDeltaTime)
{
	Item::Render(hDC, fDeltaTime);
}

Seed* Seed::Clone()
{
	return new Seed(*this);
}
