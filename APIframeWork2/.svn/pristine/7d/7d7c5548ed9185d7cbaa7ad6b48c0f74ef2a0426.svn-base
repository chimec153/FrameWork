#include "Cow.h"
#include "../Animation/Animation.h"

Cow::Cow()	:
	m_fTime(0.f),
	m_iLevel(0)
{
}

Cow::Cow(const Cow& cow)	:
	FightObj(cow)
{
	m_fTime = cow.m_fTime;
	m_iLevel = cow.m_iLevel;
}

Cow::~Cow()
{
}

bool Cow::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("CawAni");

	pAni->AddClip("CawWhiteIdleDown", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 0, 1, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteIdleRight", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 1, 1, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteIdleUp", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 2, 1, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteIdleLeft", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 3, 1, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));

	pAni->AddClip("CawWhiteWalkDown", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 0, 3, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteWalkRight", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 1, 3, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteWalkUp", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 2, 3, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));
	pAni->AddClip("CawWhiteWalkLeft", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 3, 3, 1,
		1.f,0, "Caw(white)", TEXT("Animals\\Caw(White).bmp"));

	pAni->AddClip("CalfWhiteIdleDown", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 0, 1, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteIdleRight", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 1, 1, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteIdleUp", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 2, 1, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteIdleLeft", AT_ATLAS, AO_LOOP, 0.4f, 4, 5, 0, 3, 1, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));

	pAni->AddClip("CalfWhiteWalkDown", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 0, 3, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteWalkRight", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 1, 3, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteWalkUp", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 2, 3, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));
	pAni->AddClip("CalfWhiteWalkLeft", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 5, 1, 3, 3, 1,
		1.f,0, "Calf(white)", TEXT("Animals\\Calf(White).bmp"));

	SAFE_RELEASE(pAni);

	return true;
}

int Cow::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);
	return 0;
}

int Cow::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Cow::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Cow::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Cow* Cow::Clone()
{
	return new Cow(*this);
}
