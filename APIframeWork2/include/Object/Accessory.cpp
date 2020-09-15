#include "Accessory.h"

Accessory::Accessory()	:
	m_pPlayer(nullptr)
{
}

Accessory::Accessory(const Accessory& accessory)	:
	MoveObj(accessory)
{
	m_pPlayer = accessory.m_pPlayer;
}

Accessory::~Accessory()
{
}

bool Accessory::Init()
{

	return true;
}

int Accessory::Update(float fTime)
{
	MoveObj::Update(fTime);
	return 0;
}

void Accessory::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

Accessory* Accessory::Clone()
{
	return new Accessory(*this);
}
