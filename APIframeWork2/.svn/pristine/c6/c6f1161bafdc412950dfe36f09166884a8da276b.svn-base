#include "FarmEffect.h"

FarmEffect::FarmEffect()
{
}

FarmEffect::FarmEffect(const FarmEffect& farmeffect)	:
	MoveObj(farmeffect)
{
}

FarmEffect::~FarmEffect()
{
}

bool FarmEffect::Init()
{
	return true;
}

int FarmEffect::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	return 0;
}

int FarmEffect::LateUpdate(float fDeltaTime)
{

	return 0;
}

void FarmEffect::Collision(float fDeltaTime)
{
}

void FarmEffect::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

FarmEffect* FarmEffect::Clone()
{
	return new FarmEffect(*this);
}
