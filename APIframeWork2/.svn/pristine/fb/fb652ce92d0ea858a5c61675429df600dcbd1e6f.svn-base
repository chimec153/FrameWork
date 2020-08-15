#include "Etc.h"
#include "Player.h"
#include "UIInventory.h"

Etc::Etc()
{
	m_eType = IT_ETC;
}

Etc::Etc(const Etc& etc)	:
	Item(etc)
{
}

Etc::~Etc()
{
}

bool Etc::Init()
{
	SetTexture("items", TEXT("Item\\springobjects.bmp"));
	SetColorKey(255, 255, 255);

	if (!Item::Init())
		return false;

	return true;
}

int Etc::Update(float fDeltaTime)
{
	Item::Update(fDeltaTime);
	return 0;
}

int Etc::LateUpdate(float fDeltaTime)
{
	Item::LateUpdate(fDeltaTime);
	return 0;
}

void Etc::Collision(float fDeltaTime)
{
	Item::Collision(fDeltaTime);
}

void Etc::Render(HDC hDC, float fDeltaTime)
{
	Item::Render(hDC, fDeltaTime);
}

Etc* Etc::Clone()
{
	return new Etc(*this);
}
