#include "Crop.h"

Crop::Crop()	:
	m_eType(CROP_APRICOT),
	m_iLevel(1)
{
}

Crop::Crop(const Crop& crop)	:
	MoveObj(crop)
{
	m_eType = crop.m_eType;
	m_iLevel = crop.m_iLevel;
}

Crop::~Crop()
{
}

bool Crop::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);
	SetTexture("crops", TEXT("TileSheets\\crops.bmp"));

	return true;
}

int Crop::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);
	return 0;
}

int Crop::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void Crop::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Crop::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

Crop* Crop::Clone()
{
	return new Crop(*this);
}
