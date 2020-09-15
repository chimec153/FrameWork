#include "Furniture.h"

Furniture::Furniture()
{
	m_eBlock = OB_FURNITURE;
	m_cAlpha = 255;
	m_bAlphaOn = true;
}

Furniture::Furniture(const Furniture& furniture)	:
	StaticObj(furniture)
{
}

Furniture::~Furniture()
{
}

bool Furniture::Init()
{
	return true;
}

int Furniture::Update(float fTime)
{
	StaticObj::Update(fTime);
	return 0;
}

void Furniture::Collision(float fTime)
{
	StaticObj::Collision(fTime);
}

void Furniture::Render(HDC hDC, float fTime)
{
	StaticObj::Render(hDC, fTime);
}

Furniture* Furniture::Clone()
{
	return new Furniture(*this);
}

void Furniture::Save(FILE* pFile)
{
	StaticObj::Save(pFile);
}

void Furniture::Load(FILE* pFile)
{
	StaticObj::Load(pFile);
}
