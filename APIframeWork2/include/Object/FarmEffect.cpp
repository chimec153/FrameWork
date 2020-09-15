#include "FarmEffect.h"
#include "../Animation/Animation.h"

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
	SetSize(32.f, 32.f);

	Animation* pAni = CreateAnimation("HoeAni");

	pAni->LoadFromPath("emote.sac");
	pAni->LoadFromPath("minigameeffect.sac");

	AddAnimationClip("HoeDirt", AT_ATLAS, AO_ONCE_DESTROY, 0.6f, 10, 52, 0, 12, 8, 1, 1.f, 0,
		"FarmAni", TEXT("TileSheets\\animations.bmp"));
	SetAnimationClipColorKey("HoeDirt", 255, 255, 255);

	AddAnimationClip("WaterSplashing", AT_ATLAS, AO_ONCE_DESTROY, 0.6f, 10, 52, 0, 13, 10, 1, 1.f, 0,
		"FarmAni", TEXT("TileSheets\\animations.bmp"));
	SetAnimationClipColorKey("WaterSplashing", 255, 255, 255);

	AddAnimationClip("HarvestEffect", AT_ATLAS, AO_ONCE_DESTROY, 0.6f, 10, 52, 0, 17, 7, 1, 1.f, 0,
		"FarmAni", TEXT("TileSheets\\animations.bmp"));
	SetAnimationClipColorKey("HarvestEffect", 255, 255, 255);

	AddAnimationClip("RockEffect", AT_ATLAS, AO_ONCE_DESTROY, 0.6f, 10, 52, 0, 5, 8, 1, 1.f, 0,
		"FarmAni", TEXT("TileSheets\\animations.bmp"));
	SetAnimationClipColorKey("RockEffect", 255, 255, 255);

	AddAnimationClip("Rain", AT_ATLAS, AO_ONCE_DESTROY, 0.5f, 4, 1, 0, 0, 4, 1, 1.f, 0,
		"rain", TEXT("TileSheets\\rain.bmp"));
	SetAnimationClipColorKey("Rain", 255, 255, 255);

	SAFE_RELEASE(pAni);

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
