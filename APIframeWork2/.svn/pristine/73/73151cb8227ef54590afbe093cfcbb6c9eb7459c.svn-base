#include "Goat.h"
#include "../Animation/Animation.h"

Goat::Goat()	:
	m_fTime(0.f),
	m_iLevel(0)
{
}

Goat::Goat(const Goat& goat)	:
	FightObj(goat)
{
	m_fTime = goat.m_fTime;
	m_iLevel = goat.m_iLevel;
}

Goat::~Goat()
{
}

bool Goat::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("GoatAni");

	pAni->AddClip("GoatIdleDown", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 0, 1, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatIdleRight", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 1, 1, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatIdleUp", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 2, 1, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatIdleLeft", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 3, 1, 1,
		1.f, 0,"Goat", TEXT("Animals\\Goat.bmp"));

	pAni->AddClip("GoatBabyIdleDown", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 0, 1, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyIdleRight", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 1, 1, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyIdleUp", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 2, 1, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyIdleLeft", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 3, 1, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));

	pAni->AddClip("GoatWalkDown", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 0, 3, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatWalkRight", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 1, 3, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatWalkUp", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 2, 3, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));
	pAni->AddClip("GoatWalkLeft", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 3, 3, 1,
		1.f,0, "Goat", TEXT("Animals\\Goat.bmp"));

	pAni->AddClip("GoatBabyWalkDown", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 0, 3, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyWalkRight", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 1, 3, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyWalkUp", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 2, 3, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));
	pAni->AddClip("GoatBabyWalkLeft", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 3, 3, 1,
		1.f,0, "GoatBaby", TEXT("Animals\\Goat(Baby).bmp"));

	SAFE_RELEASE(pAni);
	return true;
}

int Goat::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);
	return 0;
}

int Goat::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Goat::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Goat::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Goat* Goat::Clone()
{
	return new Goat(*this);
}
