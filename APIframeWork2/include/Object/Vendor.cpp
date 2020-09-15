#include "Vendor.h"
#include "../Animation/Animation.h"

Vendor::Vendor()
{
}

Vendor::Vendor(const Vendor& vendor)	:
	MoveObj(vendor)
{
}

Vendor::~Vendor()
{
}

bool Vendor::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);

	Animation* pAni = CreateAnimation("VecdorAni");

	pAni->LoadFromPath("vendor.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int Vendor::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);
	return 0;
}

int Vendor::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void Vendor::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Vendor::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

Vendor* Vendor::Clone()
{
	return new Vendor(*this);
}
